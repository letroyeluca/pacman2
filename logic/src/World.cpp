//
// Created by Luca Letroye on 26/11/2025.
//

#include "logic/World.h"
#include "logic/models/AppleModel.h"
#include "logic/models/CoinModel.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/utils/TxtMapLoader.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace logic {

World::World(std::shared_ptr<AbstractFactory> factory) : m_factory(factory) {
    TxtMapLoader loader(*this);
    loader.loadMap("map.txt");
}

World::~World() {}

//alle data voor het starten van het level ophalen voor de juiste aantal levels etc
void World::initializeGameData(int score, int lives, int level) {
    m_startScore = score;
    m_startLives = lives;
    m_currentLevel = level;

    if (m_scoreModel) {
        m_scoreModel->setScore(score);
    }

    if (m_pacman) {
        m_pacman->setLives(lives);
    }
}

// ------------------------------------------------------------------
// HELPER: LEVEL DIFFICULTY CURVE
// ------------------------------------------------------------------
float World::getLevelMultiplier() const {
    float mult = 1.0f + ((m_currentLevel - 1) * 0.02f);
    if (mult > 1.30f)
        return 1.30f;
    return mult;
}


//--------------------------------------------------------------------------
// HELPER: functie om via de maploader entities toe toe voegen en te inialiseren
//--------------------------------------------------------------------------
void World::addWall(double x, double y, double w, double h) { m_walls.push_back(m_factory->createWall(x, y, w, h)); }

void World::addCoin(double x, double y, double w, double h) {
    auto coin = m_factory->createCoin(x, y, w, h);
    if (m_scoreModel) {
        coin->attach(m_scoreModel);
    }
    m_coins.push_back(std::move(coin));
}

void World::addApple(double x, double y, double w, double h) {
    auto apple = m_factory->createApple(x, y, w, h);
    m_apples.push_back(std::move(apple));
}

void World::addGate(double x, double y, double w, double h) { m_gates.push_back({x, y, w, h}); }

void World::resetGhosts() {
    for (auto& ghost : m_ghosts) {
        ghost->reset();
    }
}

bool World::isGateAt(double x, double y) const {
    for (const auto& gate : m_gates) {
        double halfW = gate.w / 2.0;
        double halfH = gate.h / 2.0;
        if (x >= gate.x - halfW && x <= gate.x + halfW && y >= gate.y - halfH && y <= gate.y + halfH) {
            return true;
        }
    }
    return false;
}

void World::addPacMan(double x, double y, double w, double h, int mapsize) {
    m_pacman = m_factory->createPacMan(x, y, w, h);
    float baseTPS = 3.0f;
    float levelMult = getLevelMultiplier();
    float finalSpeed = baseTPS * levelMult * m_tileSize;
    m_pacman->setSpeed(finalSpeed);
    m_pacman->setLives(m_startLives);
}

void World::addGhost(double x, double y, double w, double h, char type) {
    auto ghost = m_factory->createGhost(x, y, w, h, type);
    float baseTPS = 2.0f;
    float levelMult = getLevelMultiplier();
    float ghostRatio = 0.95f;
    if (m_currentLevel >= 5)
        ghostRatio = 1.00f;
    if (m_currentLevel >= 10)
        ghostRatio = 1.03f;
    float finalSpeed = baseTPS * levelMult * ghostRatio * m_tileSize;
    ghost->setSpeed(finalSpeed);
    if (m_scoreModel) {
        ghost->attach(m_scoreModel);
    }
    m_ghosts.push_back(std::move(ghost));
}

void World::createScore(double x, double y, double size) {
    m_scoreModel = m_factory->createScore(x, y, size);
    if (m_scoreModel) {
        // Gebruik setScore om de opgeslagen score te tonen
        m_scoreModel->setScore(m_startScore);
    }
}

// ------------------------------------------------------------------
// AFRAID HANDLER
// ------------------------------------------------------------------
void World::activateFrightenedMode() {
    // 1. Bereken duur
    float duration = std::max(2.0f, 7.0f - ((m_currentLevel - 1) * 0.5f));

    for (auto& ghost : m_ghosts) {
        // Zet eerst de status om (visueel effect)
        ghost->frighten(duration);

        // 2. Bepaal de tegenovergestelde richting
        Direction currentDir = ghost->getDirection();
        Direction oppositeDir = Direction::STOP;

        double checkX = ghost->getX();
        double checkY = ghost->getY();
        double ts = m_tileSize; // Gebruik de tile size van de wereld

        // 3. Kijk één stapje vooruit in de NIEUWE (omgekeerde) richting
        switch (currentDir) {
        case Direction::UP:
            oppositeDir = Direction::DOWN;
            checkY += ts;
            break;
        case Direction::DOWN:
            oppositeDir = Direction::UP;
            checkY -= ts;
            break;
        case Direction::LEFT:
            oppositeDir = Direction::RIGHT;
            checkX += ts;
            break;
        case Direction::RIGHT:
            oppositeDir = Direction::LEFT;
            checkX -= ts;
            break;
        default:
            break; // Bij STOP of onbekend doen we niets
        }

        // 4. De MAGIE: Check of die positie vrij is m.b.v. World functie
        // We checken ook !isGateAt zodat ze niet terug het huis in rennen
        if (oppositeDir != Direction::STOP && isMapPositionFree(checkX, checkY) && !isGateAt(checkX, checkY)) {
            ghost->queueDirection(oppositeDir);
            ghost->commitDirection();
        }
        // ELSE: Als de weg terug geblokkeerd is (muur), doen we NIETS.
        // Het spook wordt wel bang (blauw), maar blijft zijn huidige pad volgen.
    }
}

void World::setGridDimensions(double startX, double startY, double tileSize) {
    m_startX = startX;
    m_startY = startY;
    m_tileSize = tileSize;
}

void World::setWorldDimensions(double width, double height) {
    m_width = width;
    m_height = height;
}


//visitor patterns voor de collision
class CollisionVisitor : public Visitor {
public:
    CollisionVisitor(double x, double y, double w, double h): m_checkX(x), m_checkY(y), m_checkW(w), m_checkH(h), m_mode(Mode::WALL_CHECK) {}
    CollisionVisitor(std::shared_ptr<PacManModel> pacman, World& world): m_pacman(pacman), m_world(&world), m_mode(Mode::ENTITY_CHECK) {}

    //visit functie voor pacman
    void visit(PacManModel& pacman) override {}
    bool hasCollided() const { return m_hasCollided; }

    //visit wall en check collision
    void visit(WallModel& wall) override {
        if (m_mode == Mode::WALL_CHECK) {
            if (wall.collidesWithBox(m_checkX, m_checkY, m_checkW, m_checkH)) {
                m_hasCollided = true;
            }
        }
    }

    //visit voin en check collision
    void visit(CoinModel& coin) override {
        if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
            if (coin.isActive() && m_pacman->collidesWith(coin)) {
                coin.collect();
            }
        }
    }

    //check coliision voor appel
    void visit(AppleModel& apple) override {
        if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
            if (apple.isActive() && m_pacman->collidesWith(apple)) {
                apple.collect();
                if (m_world) {
                    m_world->activateFrightenedMode();
                }
            }
        }
    }

    //check collision voor ghost
    void visit(GhostModel& ghost) override {
        if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
            if (ghost.collidesWith(*m_pacman)) {
                if (ghost.isFrightened()) {
                    ghost.die();
                } else {
                    m_pacman->die();
                    if (m_world)
                        m_world->resetGhosts();
                }
            }
        }
    }

private:
    enum class Mode { WALL_CHECK, ENTITY_CHECK };
    Mode m_mode;
    double m_checkX = 0, m_checkY = 0, m_checkW = 0, m_checkH = 0;
    bool m_hasCollided = false;
    std::shared_ptr<PacManModel> m_pacman;
    World* m_world = nullptr;
};

//CHECK van mappositie check
bool World::isMapPositionFree(double x, double y) {
    double checkSize = m_tileSize * 0.9;
    CollisionVisitor visitor(x, y, checkSize, checkSize);
    for (const auto& wall : m_walls) {
        wall->accept(visitor);
        if (visitor.hasCollided())
            return false;
    }
    return true;
}

//MAIN UPDATE FUNCTIE
void World::update(float dt) {
    updatePacMan(dt);
    updateGhosts(dt);
    handleCollisions(dt);

    //als er een scoremodel bestaat update
    if (m_scoreModel)
        m_scoreModel->update(dt);

    //CLEANUP van opgegeten items
    std::erase_if(m_coins, [](const auto& coin) { return coin->isCollected(); });
    std::erase_if(m_apples, [](const auto& apple) { return apple->isCollected(); });


    //LEVEL COMPLETE CHECK
    if (m_coins.empty() && !m_levelCompleted && m_apples.empty()) {
        m_levelCompleted = true;
    }

    respawnDeadGhosts();
}

    void World::updatePacMan(float dt) {
        if (!m_pacman)
            return;

        // 1. Grid-positie berekenen:
        // We bepalen op welke 'tegel' (gridX, gridY) Pac-Man zich bevindt en waar het exacte midden is.
        // Dit is essentieel voor soepele bochten; je mag alleen draaien als je precies in het midden bent.
        int gridX = std::round((m_pacman->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
        int gridY = std::round((m_pacman->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);
        double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
        double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);

        // Hoe ver is Pac-Man verwijderd van het perfecte midden van de tegel?
        double distToCenter = std::sqrt(std::pow(m_pacman->getX() - centerX, 2) + std::pow(m_pacman->getY() - centerY, 2));

        Direction nextDir = m_pacman->getNextDirection(); // De richting die de speler wil (buffer)
        Direction currentDir = m_pacman->getDirection();  // De richting die hij nu gaat

        // 2. Directe ommekeer (180 graden):
        // Als de speler direct de andere kant op wil (bv. links naar rechts), hoeft hij niet te wachten
        // tot het midden van de tegel. Dit mag direct.
        bool isOpposite = (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
                          (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
                          (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
                          (currentDir == Direction::RIGHT && nextDir == Direction::LEFT);

        if (isOpposite) {
            m_pacman->commitDirection();
        }
            // 3. Bocht nemen (90 graden):
            // Dit mag ALLEEN als Pac-Man dicht genoeg bij het midden van de tegel is ('distToCenter').
        else if (nextDir != Direction::STOP && nextDir != currentDir) {
            double moveStep = m_pacman->getSpeed() * dt;
            // Marge om te bepalen of we 'in' het kruispunt zijn
            double turnThreshold = std::max(moveStep + 0.005, m_tileSize * 0.15);

            double targetX = centerX;
            double targetY = centerY;

            // Kijk naar de tegel waar we heen willen draaien
            if (nextDir == Direction::UP) targetY -= m_tileSize;
            else if (nextDir == Direction::DOWN) targetY += m_tileSize;
            else if (nextDir == Direction::LEFT) targetX -= m_tileSize;
            else if (nextDir == Direction::RIGHT) targetX += m_tileSize;

            // Als de doel-tegel vrij is EN we zijn dicht bij het midden:
            if (isMapPositionFree(targetX, targetY) && !isGateAt(targetX, targetY) && distToCenter <= turnThreshold) {
                // "Snap" positie naar exact het midden om vastlopen in muren te voorkomen
                m_pacman->setPosition(centerX, centerY);
                m_pacman->commitDirection();
            }
        }

        // 4. Botsing met muur (Rechtdoor):
        // Controleren of Pac-Man tegen een muur aanloopt in zijn huidige richting.
        currentDir = m_pacman->getDirection();
        if (currentDir != Direction::STOP) {
            double forwardX = centerX;
            double forwardY = centerY;

            // Bepaal de coördinaten van de volgende tegel
            if (currentDir == Direction::UP) forwardY -= m_tileSize;
            else if (currentDir == Direction::DOWN) forwardY += m_tileSize;
            else if (currentDir == Direction::LEFT) forwardX -= m_tileSize;
            else if (currentDir == Direction::RIGHT) forwardX += m_tileSize;

            // Als de volgende tegel een muur of hek is...
            if (!isMapPositionFree(forwardX, forwardY) || isGateAt(forwardX, forwardY)) {
                // ... controleren of we het midden al voorbij zijn.
                bool pastCenter = false;
                if (currentDir == Direction::UP && m_pacman->getY() < centerY) pastCenter = true;
                if (currentDir == Direction::DOWN && m_pacman->getY() > centerY) pastCenter = true;
                if (currentDir == Direction::LEFT && m_pacman->getX() < centerX) pastCenter = true;
                if (currentDir == Direction::RIGHT && m_pacman->getX() > centerX) pastCenter = true;

                // Zo ja: Stop exact op het midden.
                if (pastCenter) {
                    m_pacman->setPosition(centerX, centerY);
                    m_pacman->stop();
                }
            }
        }

        // Fysieke positie updaten op basis van snelheid en tijd
        m_pacman->update(dt);

        // 5. Screen Wrapping (Teleport links/rechts):
        // Als Pac-Man buiten het scherm gaat, komt hij aan de andere kant terug.
        if (m_pacman->getX() < -1.0 - m_tileSize)
            m_pacman->setPosition(1.0 + m_tileSize, m_pacman->getY());
        else if (m_pacman->getX() > 1.0 + m_tileSize)
            m_pacman->setPosition(-1.0 - m_tileSize, m_pacman->getY());
    }

    void World::updateGhosts(float dt) {
        for (auto& ghost : m_ghosts) {
            // Grid positie berekenen (zelfde logica als Pac-Man)
            int gridX = std::round((ghost->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
            int gridY = std::round((ghost->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);
            double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
            double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);

            double moveStep = ghost->getSpeed() * dt;
            double distToCenter = std::sqrt(std::pow(ghost->getX() - centerX, 2) + std::pow(ghost->getY() - centerY, 2));

            // Is het spook "precies" (binnen marge) op het midden van een kruispunt?
            bool atCenter = distToCenter <= (moveStep + 0.005);

            // Reset lock zodra we het midden verlaten hebben, zodat we bij het volgende kruispunt weer mogen kiezen.
            if (!atCenter) {
                ghost->setLocked(false);
            }

            // AI Besluitvorming: Alleen als we op het midden staan en nog geen keuze hebben gemaakt (locked)
            if (atCenter && !ghost->isLocked()) {
                // Check welke kanten op kunnen
                bool freeUP = isMapPositionFree(centerX, centerY - m_tileSize);
                bool freeDOWN = isMapPositionFree(centerX, centerY + m_tileSize);
                bool freeLEFT = isMapPositionFree(centerX - m_tileSize, centerY);
                bool freeRIGHT = isMapPositionFree(centerX + m_tileSize, centerY);

                // Tel aantal uitgangen
                int exits = (freeUP ? 1 : 0) + (freeDOWN ? 1 : 0) + (freeLEFT ? 1 : 0) + (freeRIGHT ? 1 : 0);
                bool isIntersection = (exits > 2);

                // Check of we tegen een muur aanlopen
                Direction curr = ghost->getDirection();
                bool hitWall = false;
                if (curr == Direction::UP && !freeUP) hitWall = true;
                if (curr == Direction::DOWN && !freeDOWN) hitWall = true;
                if (curr == Direction::LEFT && !freeLEFT) hitWall = true;
                if (curr == Direction::RIGHT && !freeRIGHT) hitWall = true;

                // Spoken denken alleen na op kruispunten of als ze vastlopen
                if (isIntersection || hitWall) {
                    ghost->setPosition(centerX, centerY); // Snap naar midden
                    ghost->think(*this);                  // AI berekent nieuwe richting
                    ghost->commitDirection();             // Bevestig richting
                    ghost->setLocked(true);               // Voorkom dat we elke frame opnieuw nadenken op hetzelfde punt
                }
            }

            ghost->update(dt);

            // Teleport logica (screen wrap) ook voor spoken
            if (ghost->getX() < -1.0 - m_tileSize)
                ghost->setPosition(1.0 + m_tileSize, ghost->getY());
            else if (ghost->getX() > 1.0 + m_tileSize)
                ghost->setPosition(-1.0 - m_tileSize, ghost->getY());
        }
    }

    void World::handleCollisions(float dt) {
        if (!m_pacman)
            return;

        // Visitor Pattern:
        // We maken een bezoeker aan die weet wie de 'agressor' is (Pac-Man).
        // Deze bezoeker gaat langs alle objecten.
        CollisionVisitor entityVisitor(m_pacman, *this);

        // Check botsingen met munten
        for (auto& coin : m_coins) {
            coin->update(dt); // Animatie update van de munt
            coin->accept(entityVisitor); // De munt accepteert de bezoeker -> triggers logic
        }

        // Check botsingen met spoken
        for (auto& ghost : m_ghosts) {
            ghost->accept(entityVisitor);
        }

        // Check botsingen met fruit (appels)
        for (auto& apple : m_apples) {
            apple->accept(entityVisitor);
        }
    }

void World::respawnDeadGhosts() {
    struct GhostSpawnData {
        double x, y, w, h;
        char type;
    };
    std::vector<GhostSpawnData> toRespawn;
    for (const auto& ghost : m_ghosts) {
        if (ghost->isDead()) {
            toRespawn.push_back(
                {ghost->getStartX(), ghost->getStartY(), ghost->getWidth(), ghost->getHeight(), ghost->getType()});
        }
    }
    std::erase_if(m_ghosts, [](const auto& ghost) { return ghost->isDead(); });
    for (const auto& data : toRespawn) {
        addGhost(data.x, data.y, data.w, data.h, data.type);
    }
}

} // namespace logic
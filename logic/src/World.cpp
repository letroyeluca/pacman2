//
// Created by Luca Letroye on 26/11/2025.
//

#include "logic/World.h"
#include "logic/models/CoinModel.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/AppleModel.h"
#include "logic/models/WallModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/utils/TxtMapLoader.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace logic {

    World::World(AbstractFactory* factory) : m_factory(factory) {
        TxtMapLoader loader(*this);
        loader.loadMap("map.txt");
    }

    World::~World() {}

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
        if (mult > 1.30f) return 1.30f;
        return mult;
    }

    void World::addWall(double x, double y, double w, double h) {
        m_walls.push_back(m_factory->createWall(x, y, w, h));
    }

    void World::addCoin(double x, double y, double w, double h) {
        auto coin = m_factory->createCoin(x, y, w, h);
        if (m_scoreModel) {
            coin->attach(m_scoreModel.get());
        }
        m_coins.push_back(std::move(coin));
    }

    void World::addApple(double x, double y, double w, double h) {
        auto apple = m_factory->createApple(x, y, w, h);
        m_apples.push_back(std::move(apple));
    }

    void World::addGate(double x, double y, double w, double h) {
        m_gates.push_back({x, y, w, h});
    }

    void World::resetGhosts() {
        for (auto& ghost : m_ghosts) {
            ghost->reset();
        }
    }

    bool World::isGateAt(double x, double y) const {
        for (const auto& gate : m_gates) {
            double halfW = gate.w / 2.0;
            double halfH = gate.h / 2.0;
            if (x >= gate.x - halfW && x <= gate.x + halfW &&
                y >= gate.y - halfH && y <= gate.y + halfH) {
                return true;
            }
        }
        return false;
    }

// ------------------------------------------------------------------
// BALANS UPDATE: PACMAN SNELHEID
// ------------------------------------------------------------------
    void World::addPacMan(double x, double y, double w, double h, int mapsize) {
        m_pacman = m_factory->createPacMan(x, y, w, h);

        // 1. Tiles Per Second (TPS)
        // 6.0 TPS is een fijne arcade snelheid.
        float baseTPS = 3.0f;

        // 2. Level Multiplier (wordt sneller in hogere levels)
        float levelMult = getLevelMultiplier();

        // 3. Berekening: TPS * LevelFactor * GrootteVan1Tegel
        // m_tileSize is in coordinate space (-1 tot 1), dus dit klopt altijd!
        float finalSpeed = baseTPS * levelMult * m_tileSize;

        m_pacman->setSpeed(finalSpeed);

        // Zet levens correct (doorgegeven vanuit vorig level)
        m_pacman->setLives(m_startLives);
    }

// ------------------------------------------------------------------
// BALANS UPDATE: SPOOK SNELHEID
// ------------------------------------------------------------------
    void World::addGhost(double x, double y, double w, double h, char type) {
        auto ghost = m_factory->createGhost(x, y, w, h, type);

        // 1. Basis TPS (Zelfde referentie als Pacman)
        float baseTPS = 2.0f;
        float levelMult = getLevelMultiplier();

        // 2. Ghost Ratio (Relatief t.o.v. Pacman)
        // Level 1-4:   95% (Iets trager, speler kan ontsnappen)
        // Level 5-9:  100% (Even snel)
        // Level 10+:  103% (Sneller, speler moet slimme bochten maken)
        float ghostRatio = 0.95f;
        if (m_currentLevel >= 5) ghostRatio = 1.00f;
        if (m_currentLevel >= 10) ghostRatio = 1.03f;

        // 3. Finale Snelheid
        float finalSpeed = baseTPS * levelMult * ghostRatio * m_tileSize;

        ghost->setSpeed(finalSpeed);

        if (m_scoreModel) {
            ghost->attach(m_scoreModel.get());
        }
        m_ghosts.push_back(std::move(ghost));
    }

// ------------------------------------------------------------------
// SCORE UPDATE: OVERNEMEN
// ------------------------------------------------------------------
    void World::createScore(double x, double y, double size) {
        m_scoreModel = m_factory->createScore(x, y, size);
        if(m_scoreModel) {
            // Gebruik setScore om de opgeslagen score te tonen
            m_scoreModel->setScore(m_startScore);
        }
    }

// ------------------------------------------------------------------
// AFRAID TIME UPDATE
// ------------------------------------------------------------------
    void World::activateFrightenedMode() {
        // Basis tijd is 7 seconden.
        // Elk level 0.5 sec eraf, minimum 2 seconden.
        float duration = std::max(2.0f, 7.0f - ((m_currentLevel - 1) * 0.5f));

        for (auto& ghost : m_ghosts) {
            ghost->frighten(duration);
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

// ... (Rest van CollisionVisitor code blijft exact hetzelfde) ...

// --- DE REST VAN DE FILE (CollisionVisitor, isMapPositionFree, update, etc.) ---
// --- HIERONDER STAAT GEEN NIEUWE LOGICA MEER, ALLEEN DE STANDAARD UPDATE LOOP ---

    class CollisionVisitor : public Visitor {
    public:
        CollisionVisitor(double x, double y, double w, double h)
                : m_checkX(x), m_checkY(y), m_checkW(w), m_checkH(h), m_mode(Mode::WALL_CHECK) {}
        CollisionVisitor(PacManModel& pacman, World& world)
                : m_pacman(&pacman), m_world(&world), m_mode(Mode::ENTITY_CHECK) {}

        void visit(PacManModel& pacman) override {}
        bool hasCollided() const { return m_hasCollided; }

        void visit(WallModel& wall) override {
            if (m_mode == Mode::WALL_CHECK) {
                if (wall.collidesWithBox(m_checkX, m_checkY, m_checkW, m_checkH)) {
                    m_hasCollided = true;
                }
            }
        }

        void visit(CoinModel& coin) override {
            if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
                if (coin.isActive() && m_pacman->collidesWith(coin)) {
                    coin.collect();
                }
            }
        }

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

        void visit(GhostModel& ghost) override {
            if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
                if (ghost.collidesWith(*m_pacman)) {
                    if (ghost.isFrightened()) {
                        ghost.die();
                    } else {
                        m_pacman->die();
                        if (m_world) m_world->resetGhosts();
                    }
                }
            }
        }

    private:
        enum class Mode { WALL_CHECK, ENTITY_CHECK };
        Mode m_mode;
        double m_checkX = 0, m_checkY = 0, m_checkW = 0, m_checkH = 0;
        bool m_hasCollided = false;
        PacManModel* m_pacman = nullptr;
        World* m_world = nullptr;
    };

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

    void World::update(float dt) {
        updatePacMan(dt);
        updateGhosts(dt);
        handleCollisions(dt);

        if (m_scoreModel) m_scoreModel->update(dt);
        for (auto& wall : m_walls) wall->update(dt);

        std::erase_if(m_coins, [](const auto& coin) { return coin->isCollected(); });
        std::erase_if(m_apples, [](const auto& apple) { return apple->isCollected(); });

        if (m_coins.empty() && !m_levelCompleted && m_apples.empty()) {
            m_levelCompleted = true;
        }

        respawnDeadGhosts();
    }

    void World::updatePacMan(float dt) {
        if (!m_pacman) return;

        // ... (Grid berekeningen blijven hetzelfde) ...
        int gridX = std::round((m_pacman->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
        int gridY = std::round((m_pacman->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);
        double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
        double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);
        double distToCenter = std::sqrt(std::pow(m_pacman->getX() - centerX, 2) + std::pow(m_pacman->getY() - centerY, 2));

        Direction nextDir = m_pacman->getNextDirection();
        Direction currentDir = m_pacman->getDirection();

        // ... (Opposite direction logic blijft hetzelfde) ...
        bool isOpposite = (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
                          (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
                          (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
                          (currentDir == Direction::RIGHT && nextDir == Direction::LEFT);

        if (isOpposite) {
            m_pacman->commitDirection();
        }
        else if (nextDir != Direction::STOP && nextDir != currentDir) {
            // ... (Target berekening blijft hetzelfde) ...
            double moveStep = m_pacman->getSpeed() * dt;
            // Gebruik hier de fix voor responsiveness uit mijn vorige antwoord als je wilt:
            double turnThreshold = std::max(moveStep + 0.005, m_tileSize * 0.15);

            double targetX = centerX;
            double targetY = centerY;

            if (nextDir == Direction::UP) targetY -= m_tileSize;
            else if (nextDir == Direction::DOWN) targetY += m_tileSize;
            else if (nextDir == Direction::LEFT) targetX -= m_tileSize;
            else if (nextDir == Direction::RIGHT) targetX += m_tileSize;

            // --- AANPASSING 1: Check ook !isGateAt(...) ---
            // Pac-Man mag alleen draaien als het GEEN muur is EN GEEN poortje is.
            if (isMapPositionFree(targetX, targetY) && !isGateAt(targetX, targetY) && distToCenter <= turnThreshold) {
                m_pacman->setPosition(centerX, centerY);
                m_pacman->commitDirection();
            }
        }

        currentDir = m_pacman->getDirection();
        if (currentDir != Direction::STOP) {
            double forwardX = centerX;
            double forwardY = centerY;

            if (currentDir == Direction::UP) forwardY -= m_tileSize;
            else if (currentDir == Direction::DOWN) forwardY += m_tileSize;
            else if (currentDir == Direction::LEFT) forwardX -= m_tileSize;
            else if (currentDir == Direction::RIGHT) forwardX += m_tileSize;

            // --- AANPASSING 2: Check of de volgende tegel een Gate is ---
            // Als het volgende vakje een muur is OF een gate, moet hij stoppen (pastCenter check).
            if (!isMapPositionFree(forwardX, forwardY) || isGateAt(forwardX, forwardY)) {

                bool pastCenter = false;
                if (currentDir == Direction::UP && m_pacman->getY() < centerY) pastCenter = true;
                if (currentDir == Direction::DOWN && m_pacman->getY() > centerY) pastCenter = true;
                if (currentDir == Direction::LEFT && m_pacman->getX() < centerX) pastCenter = true;
                if (currentDir == Direction::RIGHT && m_pacman->getX() > centerX) pastCenter = true;

                if (pastCenter) {
                    m_pacman->setPosition(centerX, centerY);
                    m_pacman->stop();
                }
            }
        }
        m_pacman->update(dt);

        // ... (Wrap around logic blijft hetzelfde) ...
        if (m_pacman->getX() < -1.0 - m_tileSize)
            m_pacman->setPosition(1.0 + m_tileSize, m_pacman->getY());
        else if (m_pacman->getX() > 1.0 + m_tileSize)
            m_pacman->setPosition(-1.0 - m_tileSize, m_pacman->getY());
    }

    void World::updateGhosts(float dt) {
        // (Zelfde logica als je al had, alleen ingekort voor leesbaarheid hier)
        for (auto& ghost : m_ghosts) {
            int gridX = std::round((ghost->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
            int gridY = std::round((ghost->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);
            double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
            double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);

            double moveStep = ghost->getSpeed() * dt;
            double distToCenter = std::sqrt(std::pow(ghost->getX() - centerX, 2) + std::pow(ghost->getY() - centerY, 2));
            bool atCenter = distToCenter <= (moveStep + 0.005);

            if (!atCenter) {
                ghost->setLocked(false);
            }

            if (atCenter && !ghost->isLocked()) {
                bool freeUP = isMapPositionFree(centerX, centerY - m_tileSize);
                bool freeDOWN = isMapPositionFree(centerX, centerY + m_tileSize);
                bool freeLEFT = isMapPositionFree(centerX - m_tileSize, centerY);
                bool freeRIGHT = isMapPositionFree(centerX + m_tileSize, centerY);

                int exits = (freeUP ? 1 : 0) + (freeDOWN ? 1 : 0) + (freeLEFT ? 1 : 0) + (freeRIGHT ? 1 : 0);
                bool isIntersection = (exits > 2);

                Direction curr = ghost->getDirection();
                bool hitWall = false;
                if (curr == Direction::UP && !freeUP) hitWall = true;
                if (curr == Direction::DOWN && !freeDOWN) hitWall = true;
                if (curr == Direction::LEFT && !freeLEFT) hitWall = true;
                if (curr == Direction::RIGHT && !freeRIGHT) hitWall = true;

                if (isIntersection || hitWall) {
                    ghost->setPosition(centerX, centerY);
                    ghost->think(*this);
                    ghost->commitDirection();
                    ghost->setLocked(true);
                }
            }

            ghost->update(dt);

            if (ghost->getX() < -1.0 - m_tileSize)
                ghost->setPosition(1.0 + m_tileSize, ghost->getY());
            else if (ghost->getX() > 1.0 + m_tileSize)
                ghost->setPosition(-1.0 - m_tileSize, ghost->getY());
        }
    }

    void World::handleCollisions(float dt) {
        if (!m_pacman) return;
        CollisionVisitor entityVisitor(*m_pacman, *this);
        for (auto& coin : m_coins) {
            coin->update(dt);
            coin->accept(entityVisitor);
        }
        for (auto& ghost : m_ghosts) {
            ghost->accept(entityVisitor);
        }
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
                toRespawn.push_back({
                                            ghost->getStartX(), ghost->getStartY(),
                                            ghost->getWidth(), ghost->getHeight(),
                                            ghost->getType()
                                    });
            }
        }
        std::erase_if(m_ghosts, [](const auto& ghost) { return ghost->isDead(); });
        for (const auto& data : toRespawn) {
            addGhost(data.x, data.y, data.w, data.h, data.type);
        }
    }

} // namespace logic
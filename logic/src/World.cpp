//
// Created by Luca Letroye on 26/11/2025.
//

#include "logic/World.h"
#include "logic/models/CoinModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/utils/TxtMapLoader.h"
#include <cmath>

namespace logic {

World::World(AbstractFactory* factory) : m_factory(factory) {
    TxtMapLoader loader(*this);
    loader.loadMap("map.txt");
}

World::~World() {}

void World::addWall(double x, double y, double w, double h) { m_walls.push_back(m_factory->createWall(x, y, w, h)); }

void World::addCoin(double x, double y, double w, double h) {
    auto coin = m_factory->createCoin(x, y, w, h);
    if (m_scoreModel) {
        coin->attach(m_scoreModel.get());
    }

    m_coins.push_back(std::move(coin));
}

void World::addPacMan(double x, double y, double w, double h) { m_pacman = m_factory->createPacMan(x, y, w, h); }

void World::createScore(double x, double y, double size) { m_scoreModel = m_factory->createScore(x, y, size); }

void World::setGridDimensions(double startX, double startY, double tileSize) {
    m_startX = startX;
    m_startY = startY;
    m_tileSize = tileSize;
}

void World::setWorldDimensions(double width, double height) {
    m_width = width;
    m_height = height;
}

class CollisionVisitor : public Visitor {
public:
    CollisionVisitor(double x, double y, double w, double h)
        : m_checkX(x), m_checkY(y), m_checkW(w), m_checkH(h), m_mode(Mode::WALL_CHECK) {}
    CollisionVisitor(PacManModel& pacman) : m_pacman(&pacman), m_mode(Mode::COIN_CHECK) {}

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
        if (m_mode == Mode::COIN_CHECK && m_pacman) {
            if (coin.isActive() && m_pacman->collidesWith(coin)) {
                coin.collect();
            }
        }
    }

private:
    enum class Mode { WALL_CHECK, COIN_CHECK };
    Mode m_mode;
    double m_checkX = 0, m_checkY = 0, m_checkW = 0, m_checkH = 0;
    bool m_hasCollided = false;
    PacManModel* m_pacman = nullptr;
};

bool World::isMapPositionFree(double x, double y) {
    double checkSize = m_tileSize * 0.5;
    CollisionVisitor visitor(x, y, checkSize, checkSize);
    for (const auto& wall : m_walls) {
        wall->accept(visitor);
        if (visitor.hasCollided())
            return false;
    }
    return true;
}

void World::update(float dt) {
    // -------------------------------------------------------------------------
    // 1. PACMAN UPDATE LOGICA
    // -------------------------------------------------------------------------
    if (m_pacman) {
        // -- A. Grid Positie Berekeningen --
        // We bepalen op welke grid-tegel Pac-Man zich bevindt en waar het
        // absolute midden van die tegel is.
        int gridX = std::round((m_pacman->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
        int gridY = std::round((m_pacman->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);

        double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
        double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);

        // Bereken de afstand van Pac-Man tot het exacte midden van de tegel.
        double distToCenter =
            std::sqrt(std::pow(m_pacman->getX() - centerX, 2) + std::pow(m_pacman->getY() - centerY, 2));

        Direction nextDir = m_pacman->getNextDirection();
        Direction currentDir = m_pacman->getDirection();

        // -- B. Richting Veranderen (Bochten & Omdraaien) --

        // Check 1: Wil de speler 180 graden draaien? (Direct omkeren)
        // Dit mag altijd direct gebeuren, zonder te wachten op het midden.
        bool isOpposite = (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
                          (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
                          (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
                          (currentDir == Direction::RIGHT && nextDir == Direction::LEFT);

        if (isOpposite) {
            m_pacman->commitDirection();
        } else if (nextDir != Direction::STOP && nextDir != currentDir) {
            // Check 2: Normale bocht (90 graden)
            // Dit mag alleen als we dicht genoeg bij het midden van de tegel zijn.

            double moveStep = m_pacman->getSpeed() * dt;

            // De threshold is de afstand die we in 1 frame afleggen + een kleine
            // marge. Dit zorgt voor een vloeiende overgang zonder zichtbare
            // "teleport".
            double turnThreshold = moveStep + 0.005;

            double targetX = centerX;
            double targetY = centerY;

            if (nextDir == Direction::UP)
                targetY -= m_tileSize;
            else if (nextDir == Direction::DOWN)
                targetY += m_tileSize;
            else if (nextDir == Direction::LEFT)
                targetX -= m_tileSize;
            else if (nextDir == Direction::RIGHT)
                targetX += m_tileSize;

            // Als de doeltegel vrij is EN we zijn op het "draaipunt":
            if (isMapPositionFree(targetX, targetY) && distToCenter <= turnThreshold) {
                m_pacman->setPosition(centerX, centerY); // Corrigeer positie miniem
                m_pacman->commitDirection();             // Voer draai uit
            }
        }

        // -- C. Botsing Detectie (Stoppen tegen muren) --

        // Update de currentDir, want die kan hierboven net gewijzigd zijn.
        currentDir = m_pacman->getDirection();

        if (currentDir != Direction::STOP) {
            double forwardX = centerX;
            double forwardY = centerY;

            // Kijk één tegel vooruit in de huidige richting
            if (currentDir == Direction::UP)
                forwardY -= m_tileSize;
            else if (currentDir == Direction::DOWN)
                forwardY += m_tileSize;
            else if (currentDir == Direction::LEFT)
                forwardX -= m_tileSize;
            else if (currentDir == Direction::RIGHT)
                forwardX += m_tileSize;

            if (!isMapPositionFree(forwardX, forwardY)) {
                // De weg voor ons is geblokkeerd.
                // Check of we per ongeluk voorbij het midden zijn geschoten.
                bool pastCenter = false;
                if (currentDir == Direction::UP && m_pacman->getY() < centerY)
                    pastCenter = true;
                if (currentDir == Direction::DOWN && m_pacman->getY() > centerY)
                    pastCenter = true;
                if (currentDir == Direction::LEFT && m_pacman->getX() < centerX)
                    pastCenter = true;
                if (currentDir == Direction::RIGHT && m_pacman->getX() > centerX)
                    pastCenter = true;

                if (pastCenter) {
                    m_pacman->setPosition(centerX, centerY); // Zet terug op midden
                    m_pacman->stop();                        // Stop beweging
                }
            }
        }

        // -- D. Fysieke Update --
        m_pacman->update(dt);

        // -- E. Tunnel Logica (Warp) --
        if (m_pacman->getX() < -1.0 - m_tileSize) {
            m_pacman->setPosition(1.0 + m_tileSize, m_pacman->getY());
        } else if (m_pacman->getX() > 1.0 + m_tileSize) {
            m_pacman->setPosition(-1.0 - m_tileSize, m_pacman->getY());
        }
    }

    if (m_scoreModel) {
        m_scoreModel->update(dt);
    }
    // -------------------------------------------------------------------------
    // 2. MUNTEN EN SCORE
    // -------------------------------------------------------------------------
    if (m_pacman) {
        CollisionVisitor coinVisitor(*m_pacman);
        for (auto& coin : m_coins) {
            coin->update(dt);
            coin->accept(coinVisitor);
        }
    }

    // -------------------------------------------------------------------------
    // 3. OVERIGE ENTITEITEN
    // -------------------------------------------------------------------------
    for (auto& wall : m_walls) {
        wall->update(dt);
    }
}

} // namespace logic

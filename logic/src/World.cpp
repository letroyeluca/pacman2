//
// Created by Luca Letroye on 26/11/2025.
//

#include "logic/models/PacManModel.h"
#include "logic/models/CoinModel.h"
#include "logic/models/WallModel.h"
#include "logic/World.h"
#include "logic/patterns/Visitor.h"
#include <cmath>
#include "logic/utils/txtMapLoader.h"

namespace logic{

    World::World(AbstractFactory* factory)
            : m_factory(factory)
    {
        txtMapLoader loader(*this);
        loader.loadMap("map.txt");
    }

    World::~World() {}

    void World::addWall(double x, double y, double w, double h){
        m_walls.push_back(m_factory->createWall(x, y, w, h));
    }

    void World::addCoin(double x, double y, double w, double h){
        m_coins.push_back(m_factory->createCoin(x, y, w, h));
    }

    void World::addPacMan(double x, double y, double w, double h){
        m_pacman = m_factory->createPacMan(x, y, w, h);
    }


    class CollisionVisitor : public Visitor {
    public:

        CollisionVisitor(double x, double y, double w, double h) : m_checkX(x), m_checkY(y), m_checkW(w), m_checkH(h), m_mode(Mode::WALL_CHECK) {}
        CollisionVisitor(PacManModel& pacman)
                : m_pacman(&pacman), m_mode(Mode::COIN_CHECK) {}

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
                    coin.setActive(false);
                    //TODO: SCORE TOEVEGEN
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
            if (visitor.hasCollided()) return false;
        }
        return true;
    }

    void World::update(float dt) {
        if (m_pacman) {
            int gridX = std::round((m_pacman->getX() - m_startX - (m_tileSize/2.0)) / m_tileSize);
            int gridY = std::round((m_pacman->getY() - m_startY - (m_tileSize/2.0)) / m_tileSize);

            double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
            double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);
            double distToCenter = std::sqrt(std::pow(m_pacman->getX() - centerX, 2) + std::pow(m_pacman->getY() - centerY, 2));

            double snapThreshold = m_tileSize * 0.2;

            Direction nextDir = m_pacman->getNextDirection();

            if (nextDir != Direction::STOP && nextDir != m_pacman->getDirection()) {
                double targetX = centerX;
                double targetY = centerY;

                if (nextDir == Direction::UP) targetY -= m_tileSize;
                else if (nextDir == Direction::DOWN) targetY += m_tileSize;
                else if (nextDir == Direction::LEFT) targetX -= m_tileSize;
                else if (nextDir == Direction::RIGHT) targetX += m_tileSize;

                if (isMapPositionFree(targetX, targetY) && distToCenter < snapThreshold) {
                    m_pacman->setPosition(centerX, centerY);
                    m_pacman->commitDirection();
                }
            }

            Direction currentDir = m_pacman->getDirection();
            if (currentDir != Direction::STOP) {
                double forwardX = centerX;
                double forwardY = centerY;

                if (currentDir == Direction::UP) forwardY -= m_tileSize;
                else if (currentDir == Direction::DOWN) forwardY += m_tileSize;
                else if (currentDir == Direction::LEFT) forwardX -= m_tileSize;
                else if (currentDir == Direction::RIGHT) forwardX += m_tileSize;

                if (!isMapPositionFree(forwardX, forwardY)) {

                    bool pastCenter = false;
                    if (currentDir == Direction::UP && m_pacman->getY() < centerY - 0.001) pastCenter = true;
                    if (currentDir == Direction::DOWN && m_pacman->getY() > centerY + 0.001) pastCenter = true;
                    if (currentDir == Direction::LEFT && m_pacman->getX() < centerX - 0.001) pastCenter = true;
                    if (currentDir == Direction::RIGHT && m_pacman->getX() > centerX + 0.001) pastCenter = true;

                    if (pastCenter) {

                        m_pacman->setPosition(centerX, centerY);
                        m_pacman->stop();
                    }
                }
            }
            m_pacman->update(dt);

            //TUNNEL LOGICA
            if (m_pacman->getX() < -1.0 - m_tileSize) {
                m_pacman->setPosition(1.0 + m_tileSize, m_pacman->getY());
            }
            else if (m_pacman->getX() > 1.0 + m_tileSize) {
                m_pacman->setPosition(-1.0 - m_tileSize, m_pacman->getY());
            }
        }

        //MUNTEN
        if (m_pacman) {
            CollisionVisitor coinVisitor(*m_pacman);
            for (auto& coin : m_coins) {
                coin->update(dt);
                coin->accept(coinVisitor);

            }
        }

        for (auto& wall : m_walls) {
            wall->update(dt);
        }
    }

}

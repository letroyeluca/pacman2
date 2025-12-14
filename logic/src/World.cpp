//
// Created by Luca Letroye on 26/11/2025.
//

#include "logic/World.h"
#include "logic/models/CoinModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "logic/models/GhostModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/utils/TxtMapLoader.h"
#include <cmath>
#include <iostream> // Voor debug prints
#include <algorithm>

namespace logic {

    World::World(AbstractFactory* factory) : m_factory(factory) {
        TxtMapLoader loader(*this);
        loader.loadMap("map.txt");
    }

    World::~World() {}
//
    void World::addWall(double x, double y, double w, double h) { m_walls.push_back(m_factory->createWall(x, y, w, h)); }

    void World::addCoin(double x, double y, double w, double h) {
        auto coin = m_factory->createCoin(x, y, w, h);
        if (m_scoreModel) {
            coin->attach(m_scoreModel.get());
        }
        m_coins.push_back(std::move(coin));
    }

    void World::addGate(double x, double y, double w, double h) {
        m_gates.push_back({x, y, w, h});
    }

// Update isGateAt: Check of positie X/Y BINNEN de rechthoek van de gate valt
    bool World::isGateAt(double x, double y) const {
        for (const auto& gate : m_gates) {
            double halfW = gate.w / 2.0;
            double halfH = gate.h / 2.0;

            double left   = gate.x - halfW;
            double right  = gate.x + halfW;
            double top    = gate.y - halfH;
            double bottom = gate.y + halfH;

            // De Box Check: Is de 'x' van het spook groter dan links EN kleiner dan rechts? (enz.)
            if (x >= left && x <= right && y >= top && y <= bottom) {
                return true;
            }
        }
        return false;
    }

    void World::addPacMan(double x, double y, double w, double h, int mapsize) {
        m_pacman = m_factory->createPacMan(x, y, w, h);
        // FIX: Gebruik floats om integer deling te voorkomen
        m_pacman->setSpeed((3.0f / static_cast<float>(mapsize)) * 2.0f);
    }

    void World::addGhost(double x, double y, double w, double h, char type) {
        m_ghosts.push_back(m_factory->createGhost(x, y, w, h, type));
    }

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

// ----------------------------------------------------------------------------
// COLLISION VISITOR
// ----------------------------------------------------------------------------
    class CollisionVisitor : public Visitor {
    public:
        CollisionVisitor(double x, double y, double w, double h)
                : m_checkX(x), m_checkY(y), m_checkW(w), m_checkH(h), m_mode(Mode::WALL_CHECK) {}
        CollisionVisitor(PacManModel& pacman) : m_pacman(&pacman), m_mode(Mode::ENTITY_CHECK) {}

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

        void visit(GhostModel& ghost) override {
            if (m_mode == Mode::ENTITY_CHECK && m_pacman) {
                if (ghost.collidesWith(*m_pacman)) {
                    // Simpele logica: PacMan gaat dood bij aanraking (tenzij ghost bang is, maar AI is weggehaald)
                    // m_pacman->die();
                    // Of reset positie:
                    // m_pacman->setPosition(0,0); // Voorbeeld
                }
            }
        }

    private:
        enum class Mode { WALL_CHECK, ENTITY_CHECK };
        Mode m_mode;
        double m_checkX = 0, m_checkY = 0, m_checkW = 0, m_checkH = 0;
        bool m_hasCollided = false;
        PacManModel* m_pacman = nullptr;
    };

// ----------------------------------------------------------------------------
// HELPER: Check of positie vrij is (geen muur)
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// UPDATE LOGICA
// ----------------------------------------------------------------------------

    void World::update(float dt) {

        // --- 1. PACMAN UPDATE (Ongewijzigd) ---
        if (m_pacman) {
            int gridX = std::round((m_pacman->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
            int gridY = std::round((m_pacman->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);

            double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
            double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);
            double distToCenter = std::sqrt(std::pow(m_pacman->getX() - centerX, 2) + std::pow(m_pacman->getY() - centerY, 2));

            Direction nextDir = m_pacman->getNextDirection();
            Direction currentDir = m_pacman->getDirection();

            bool isOpposite = (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
                              (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
                              (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
                              (currentDir == Direction::RIGHT && nextDir == Direction::LEFT);

            if (isOpposite) {
                m_pacman->commitDirection();
            } else if (nextDir != Direction::STOP && nextDir != currentDir) {
                double moveStep = m_pacman->getSpeed() * dt;
                double turnThreshold = moveStep + 0.005;
                double targetX = centerX;
                double targetY = centerY;

                if (nextDir == Direction::UP) targetY -= m_tileSize;
                else if (nextDir == Direction::DOWN) targetY += m_tileSize;
                else if (nextDir == Direction::LEFT) targetX -= m_tileSize;
                else if (nextDir == Direction::RIGHT) targetX += m_tileSize;

                if (isMapPositionFree(targetX, targetY) && distToCenter <= turnThreshold) {
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

                if (!isMapPositionFree(forwardX, forwardY)) {
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

            // Tunnel Wrap PacMan
            if (m_pacman->getX() < -1.0 - m_tileSize) m_pacman->setPosition(1.0 + m_tileSize, m_pacman->getY());
            else if (m_pacman->getX() > 1.0 + m_tileSize) m_pacman->setPosition(-1.0 - m_tileSize, m_pacman->getY());
        }

        // --- 2. GHOST UPDATE (MANUAL + STOP @ INTERSECTION) ---
        for (auto& ghost : m_ghosts) {

            // A. Grid & Center
            int gridX = std::round((ghost->getX() - m_startX - (m_tileSize / 2.0)) / m_tileSize);
            int gridY = std::round((ghost->getY() - m_startY - (m_tileSize / 2.0)) / m_tileSize);
            double centerX = m_startX + (gridX * m_tileSize) + (m_tileSize / 2.0);
            double centerY = m_startY + (gridY * m_tileSize) + (m_tileSize / 2.0);

            double moveStep = ghost->getSpeed() * dt;
            double distToCenter = std::sqrt(std::pow(ghost->getX() - centerX, 2) + std::pow(ghost->getY() - centerY, 2));

            // Ben ik op het beslismoment (het center)?
            bool atCenter = distToCenter <= (moveStep + 0.015);

            // Reset Lock als we center verlaten
            if (!atCenter) {
                ghost->setLocked(false);
            }

            // B. INTERSECTION CHECK & AI TRIGGER
            // We kijken alleen als we op center zijn en nog niet besloten hebben (not locked)
            if (atCenter && !ghost->isLocked()) {

                // Check uitgangen
                bool freeUP    = isMapPositionFree(centerX, centerY - m_tileSize);
                bool freeDOWN  = isMapPositionFree(centerX, centerY + m_tileSize);
                bool freeLEFT  = isMapPositionFree(centerX - m_tileSize, centerY);
                bool freeRIGHT = isMapPositionFree(centerX + m_tileSize, centerY);

                int exits = (freeUP?1:0) + (freeDOWN?1:0) + (freeLEFT?1:0) + (freeRIGHT?1:0);

                // Wanneer moet AI nadenken?
                // 1. Meer dan 2 uitgangen (Echt kruispunt)
                // 2. Precies 1 uitgang (Doodlopend, we moeten draaien)
                // 3. Precies 2 uitgangen MAAR het is een bocht (niet rechtdoor) -> AI moet sturen

                // Simpele check voor nu (zoals je vroeg): Meer dan 1 uitgang is een trigger.
                // + We voegen toe: als we NIET rechtdoor kunnen (muur), moeten we ook nadenken.

                bool isIntersection = (exits > 2);

                Direction curr = ghost->getDirection();
                bool hitWall = false;
                if (curr == Direction::UP && !freeUP) hitWall = true;
                if (curr == Direction::DOWN && !freeDOWN) hitWall = true;
                if (curr == Direction::LEFT && !freeLEFT) hitWall = true;
                if (curr == Direction::RIGHT && !freeRIGHT) hitWall = true;

                // TRIGGER AI
                if (isIntersection || hitWall) {
                    // Snap naar center zodat we netjes de bocht om kunnen
                    ghost->setPosition(centerX, centerY);

                    // Vraag het brein om een richting
                    ghost->think(*this);

                    // Voer de keuze direct door
                    ghost->commitDirection();

                    // Lock zodat hij niet 60x per seconde nadenkt op dezelfde tegel
                    ghost->setLocked(true);

                    // Debug Print
                    std::cout << "Ghost AI Decision @ [" << gridX << "," << gridY << "] -> ";
                    if(ghost->getDirection() == Direction::UP) std::cout << "UP";
                    else if(ghost->getDirection() == Direction::DOWN) std::cout << "DOWN";
                    else if(ghost->getDirection() == Direction::LEFT) std::cout << "LEFT";
                    else if(ghost->getDirection() == Direction::RIGHT) std::cout << "RIGHT";
                    std::cout << std::endl;
                }
            }

            // C. Update Physics (Bewegen)
            // Omdat we hierboven al commitDirection doen, hoeven we geen moeilijke "bocht logica"
            // meer te hebben in de loop zelf, dat regelt de 'think' en 'setPosition' combinatie.

            ghost->update(dt);

            // Wrap
            if (ghost->getX() < -1.0 - m_tileSize) ghost->setPosition(1.0 + m_tileSize, ghost->getY());
            else if (ghost->getX() > 1.0 + m_tileSize) ghost->setPosition(-1.0 - m_tileSize, ghost->getY());
        }

        // --- 3. COLLISIONS ---
        if (m_pacman) {
            CollisionVisitor entityVisitor(*m_pacman);

            for (auto& coin : m_coins) {
                coin->update(dt);
                coin->accept(entityVisitor);
            }
            for (auto& ghost : m_ghosts) {
                ghost->accept(entityVisitor);
            }
        }

        // --- 4. OVERIG ---
        if (m_scoreModel) m_scoreModel->update(dt);
        for (auto& wall : m_walls) wall->update(dt);
    }

} // namespace logic
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/strategies/GhostStrategy.h"
#include <iostream>

namespace logic {

    GhostModel::GhostModel(double x, double y, double width, double height, char type)
            : EntityModel(x, y, width, height),
              m_direction(Direction::UP),
              m_nextDirection(Direction::UP),
              m_speed(0.25f), // Dit wordt direct overschreven door World::addGhost
              m_type(type),
              m_locked(false),
              m_startX(x),
              m_startY(y),
              m_isDead(false),
              m_hasExitedHouse(false)
    {
        setHitboxScale(0.2);

        if (type == 'R') {
            setSpawnDelay(0.0f);
            m_normalStrategy = std::make_unique<DirectChaseStrategy>();
        } else if (type == 'S') {
            setSpawnDelay(0.0f);
            m_normalStrategy = std::make_unique<FrontChaseStrategy>();
        } else if (type == 'B') {
            setSpawnDelay(5.0f);
            m_normalStrategy = std::make_unique<FrontChaseStrategy>();
        } else if (type == 'O') {
            setSpawnDelay(10.0f);
            m_normalStrategy = std::make_unique<DirectChaseStrategy>();
        } else {
            setSpawnDelay(0.0f);
            m_normalStrategy = std::make_unique<RandomStrategy>();
        }

        m_frightenedStrategy = std::make_unique<FrightenedStrategy>();
    }

    GhostModel::~GhostModel() = default;

    void GhostModel::frighten(float duration) {
        if (m_isDead || !m_hasExitedHouse || !m_isActive) return;

        m_isFrightened = true;
        m_frightenedTimer = duration;

        if (m_direction == Direction::UP) queueDirection(Direction::DOWN);
        else if (m_direction == Direction::DOWN) queueDirection(Direction::UP);
        else if (m_direction == Direction::LEFT) queueDirection(Direction::RIGHT);
        else if (m_direction == Direction::RIGHT) queueDirection(Direction::LEFT);

        commitDirection();
        notify(logic::Event::GhostVulnerable);
    }

    void GhostModel::setSpawnDelay(float delay) {
        m_spawnDelay = delay;
        m_spawnTimer = delay;
        m_isActive = (delay <= 0.0f);
    }

    void GhostModel::die(){
        reset();
        notify(logic::Event::GhostEaten);
    }

    void GhostModel::reset() {
        m_x = m_startX;
        m_y = m_startY;
        m_direction = Direction::UP;
        m_nextDirection = Direction::UP;
        m_locked = false;
        m_isFrightened = false;
        m_isActive = (m_spawnDelay <= 0.0f);
        notify(logic::Event::GhostNormal);
        notify(logic::Event::DEFAULT);

    }

    void GhostModel::think(World& world) {
        // -----------------------------------------------------------
        // STAP A: SLIMME EXIT LOGICA
        // -----------------------------------------------------------
        if (m_isActive && !m_hasExitedHouse) {
            double ts = world.getTileSize();
            double exitY = m_startY - (1.0 * ts);

            if (m_y <= exitY) {
                m_hasExitedHouse = true;
            }
            else {
                bool freeUP = world.isMapPositionFree(m_x, m_y - ts);
                if (freeUP) {
                    queueDirection(Direction::UP);
                    double centerX = std::round(m_x / ts) * ts + (ts / 2.0);
                    if (std::abs(m_x - centerX) > 1.0) {
                        if (m_x < centerX) m_x += m_speed * 0.016;
                        else m_x -= m_speed * 0.016;
                    }
                } else {
                    if (m_type == 'B') queueDirection(Direction::LEFT);
                    else if (m_type == 'O') queueDirection(Direction::RIGHT);
                    else {
                        bool freeRight = world.isMapPositionFree(m_x + ts, m_y);
                        if (freeRight) queueDirection(Direction::RIGHT);
                        else queueDirection(Direction::LEFT);
                    }
                }
                return;
            }
        }

        // -----------------------------------------------------------
        // STAP B: STRATEGIE
        // -----------------------------------------------------------
        GhostStrategy* activeStrategy = nullptr;
        if (m_isFrightened) {
            activeStrategy = m_frightenedStrategy.get();
        } else {
            activeStrategy = m_normalStrategy.get();
        }

        if (activeStrategy) {
            Direction newDir = activeStrategy->calculateNextMove(*this, world);
            queueDirection(newDir);
        }
    }

    void GhostModel::queueDirection(Direction newDir) {
        m_nextDirection = newDir;
        if (m_direction == Direction::STOP)
            m_direction = newDir;
    }
    void GhostModel::commitDirection() { m_direction = m_nextDirection; }
    void GhostModel::stop() { m_direction = Direction::STOP; }
    void GhostModel::setPosition(double x, double y) {
        m_x = x;
        m_y = y;
        notify(logic::Event::DEFAULT);
    }
    void GhostModel::accept(Visitor& visitor) { visitor.visit(*this); }

// ----------------------------------------------------------
// DE UPDATE FUNCTIE
// ----------------------------------------------------------
    void GhostModel::update(float dt) {

        // 1. SPAWN TIMER
        if (!m_isActive) {
            m_spawnTimer -= dt;
            if (m_spawnTimer <= 0.0f) {
                m_isActive = true;
            }
        }

        // 2. FRIGHTENED TIMER
        if (m_isFrightened) {
            m_frightenedTimer -= dt;
            if (m_frightenedTimer <= 0.0f) {
                m_isFrightened = false;

                // Omkeren als ze weer normaal worden (klassiek gedrag)
                if (m_direction == Direction::UP) queueDirection(Direction::DOWN);
                else if (m_direction == Direction::DOWN) queueDirection(Direction::UP);
                else if (m_direction == Direction::LEFT) queueDirection(Direction::RIGHT);
                else if (m_direction == Direction::RIGHT) queueDirection(Direction::LEFT);

                notify(logic::Event::GhostNormal);
            }
        }

        // 3. BEWEGING
        if (!m_isActive) return;

        // SNELHEIDS CALC: 60% als bang, anders normaal
        float currentSpeed = m_isFrightened ? (m_speed * 0.6f) : m_speed;
        double delta = currentSpeed * dt;
        std::cout <<  getType()  << "  " << currentSpeed << std::endl;


        switch (m_direction) {
            case Direction::UP:
                m_y -= delta;
                notify(logic::Event::GhostUP);
                break;
            case Direction::DOWN:
                m_y += delta;
                notify(logic::Event::GhostDOWN);
                break;
            case Direction::LEFT:
                m_x -= delta;
                notify(logic::Event::GhostLEFT);
                break;
            case Direction::RIGHT:
                m_x += delta;
                notify(logic::Event::GhostRIGHT);
                break;
            case Direction::STOP:
                break;
        }
    }

} // namespace logic
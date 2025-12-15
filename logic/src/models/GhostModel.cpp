#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/strategies/GhostStrategy.h" // <--- Includeer de strategy
#include <iostream>

namespace logic {

GhostModel::GhostModel(double x, double y, double width, double height, char type)
    : EntityModel(x, y, width, height), m_direction(Direction::LEFT), // Begin bewegend (niet STOP)
      m_nextDirection(Direction::LEFT), m_speed(0.25f), m_type(type), m_locked(false), m_startX(x), m_startY(y),
      m_isDead(false) {

    setHitboxScale(0.2);
    m_frightenedStrategy = std::make_unique<RandomStrategy>();
    if (type == 'R') {
        setSpawnDelay(0.0f); // Direct
        m_strategy = std::make_unique<LockedStrategy>();
    } else if (type == 'S') { // Pinky
        setSpawnDelay(0.0f);  // Direct (eerste twee spoken)
        m_strategy = std::make_unique<FrontChaseStrategy>();
    } else if (type == 'B') { // Inky (Blue)
        setSpawnDelay(5.0f);  // Na 5 seconden
        m_strategy = std::make_unique<FrontChaseStrategy>();
    } else if (type == 'O') { // Clyde (Orange)
        setSpawnDelay(10.0f); // Na 10 seconden
        m_strategy = std::make_unique<DirectChaseStrategy>();
    } else {
        setSpawnDelay(0.0f);
        m_strategy = std::make_unique<RandomStrategy>();
    }

    // Geef elk spook een RandomStrategy
}

void GhostModel::frighten(float duration) {
    m_isFrightened = true;
    m_frightenedTimer = duration;

    notify(logic::Event::GhostVulnerable);
}

// Destructor implementatie
GhostModel::~GhostModel() = default;

void GhostModel::setSpawnDelay(float delay) {
    m_spawnDelay = delay;
    m_spawnTimer = delay;
    m_isActive = (delay <= 0.0f); // Als delay 0 is, direct actief
}

void GhostModel::die() {
    m_isDead = true;
    if (m_isActive) {
        m_isActive = false;               // 1. Zet inactief
        notify(logic::Event::GhostEaten); // 2. Stuur bericht (Mag hier wel!)
    }
}
// Pas de reset aan
void GhostModel::reset() {
    m_x = m_startX;
    m_y = m_startY;

    // Reset beweging
    m_direction = Direction::UP;
    m_nextDirection = Direction::UP;
    m_locked = false;

    // NIEUW: Reset de timer
    m_spawnTimer = m_spawnDelay;
    m_isActive = (m_spawnDelay <= 0.0f); // Reset active status
}

// --- DE THINK FUNCTIE ---
void GhostModel::think(World& world) {
    if (m_strategy) {
        Direction newDir = m_strategy->calculateNextMove(*this, world);
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

void GhostModel::update(float dt) {

    if (!m_isActive) {
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.0f) {
            m_isFrightened = false;
            m_isActive = true;
        } else {
            // Nog niet actief? Doe niks (return) of update alleen animatie
            return;
        }
    }

    if (m_isFrightened) {
        m_frightenedTimer -= dt;
        if (m_frightenedTimer <= 0.0f) {
            m_isFrightened = false;
            notify(logic::Event::GhostNormal);
        }
    }
    float currentSpeed = m_isFrightened ? (m_speed * 0.6f) : m_speed;
    double delta = currentSpeed * dt;
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
#include "logic/models/GhostModel.h"
#include "logic/patterns/Visitor.h"
#include "logic/models/PacManModel.h"
#include "logic/strategies/GhostStrategy.h" // <--- Includeer de strategy

namespace logic {

    GhostModel::GhostModel(double x, double y, double width, double height, char type)
            : EntityModel(x, y, width, height),
              m_direction(Direction::LEFT), // Begin bewegend (niet STOP)
              m_nextDirection(Direction::LEFT),
              m_speed(0.35f),
              m_type(type),
              m_locked(false) {

        setHitboxScale(0.9);

        // Geef elk spook een RandomStrategy
        m_strategy = std::make_unique<RandomStrategy>();
    }

    // Destructor implementatie
    GhostModel::~GhostModel() = default;

    // --- DE THINK FUNCTIE ---
    void GhostModel::think(World& world) {
        if (m_strategy) {
            Direction newDir = m_strategy->calculateNextMove(*this, world);
            queueDirection(newDir);
        }
    }

    // ... (De rest blijft hetzelfde als voorheen) ...
    void GhostModel::queueDirection(Direction newDir) {
        m_nextDirection = newDir;
        if (m_direction == Direction::STOP) m_direction = newDir;
    }
    void GhostModel::commitDirection() { m_direction = m_nextDirection; }
    void GhostModel::stop() { m_direction = Direction::STOP; }
    void GhostModel::setPosition(double x, double y) { m_x = x; m_y = y; notify(logic::Event::DEFAULT); }
    void GhostModel::accept(Visitor& visitor) { visitor.visit(*this); }

    void GhostModel::update(float dt) {
        double delta = m_speed * dt;
        switch (m_direction) {
            case Direction::UP:    m_y -= delta; notify(logic::Event::GhostUP); break;
            case Direction::DOWN:  m_y += delta; notify(logic::Event::GhostDOWN); break;
            case Direction::LEFT:  m_x -= delta; notify(logic::Event::GhostLEFT); break;
            case Direction::RIGHT: m_x += delta; notify(logic::Event::GhostRIGHT); break;
            case Direction::STOP:  break;
        }
    }
}
//
// Created by Luca Letroye on 28/11/2025.
//

#include "logic/models/PacManModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {

PacManModel::PacManModel(double x, double y, double width, double height)
    : EntityModel(x, y, width, height), m_direction(Direction::STOP), m_nextDirection(Direction::STOP), m_speed(0.35f) {

    setHitboxScale(1.0);
}

void PacManModel::queueDirection(Direction newDir) {
    m_nextDirection = newDir;
    if (m_direction == Direction::STOP) {
        m_direction = newDir;
    }
}

void PacManModel::commitDirection() { m_direction = m_nextDirection; }

void PacManModel::setPosition(double x, double y) {
    m_x = x;
    m_y = y;
    notify(logic::Event::DEFAULT);
}

void PacManModel::stop() { m_direction = Direction::STOP; }

std::pair<double, double> PacManModel::getPositionAt(float dt, Direction dir) const {
    double nextX = m_x.asFloat();
    double nextY = m_y.asFloat();
    double delta = m_speed * dt;

    switch (dir) {
    case Direction::UP:
        nextY -= delta;
        break;
    case Direction::DOWN:
        nextY += delta;
        break;
    case Direction::LEFT:
        nextX -= delta;
        break;
    case Direction::RIGHT:
        nextX += delta;
        break;
    case Direction::STOP:
        break;
    }
    return {nextX, nextY};
}

void PacManModel::accept(Visitor& visitor) { visitor.visit(*this); }

void PacManModel::update(float dt) {
    double delta = m_speed * dt;

    switch (m_direction) {
    case Direction::UP:
        m_y -= delta;
        notify(logic::Event::PacManUP);
        break;
    case Direction::DOWN:
        m_y += delta;
        notify(logic::Event::PacManDOWN);
        break;
    case Direction::LEFT:
        m_x -= delta;
        notify(logic::Event::PacManLEFT);
        break;
    case Direction::RIGHT:
        m_x += delta;
        notify(logic::Event::PacManRIGHT);
        break;
    case Direction::STOP:
        break;
    }
}

} // namespace logic
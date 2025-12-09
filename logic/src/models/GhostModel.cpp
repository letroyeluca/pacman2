//
// Created by Luca Letroye on 28/11/2025.
//
#include "logic/models/GhostModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {

GhostModel::GhostModel(double x, double y, double width, double height, char type)
    : EntityModel(x, y, width, height), m_direction(Direction::STOP), m_nextDirection(Direction::STOP), m_speed(0.35f),
      m_type(type) {

    setHitboxScale(1.0);
}

void GhostModel::queueDirection(Direction newDir) {
    m_nextDirection = newDir;
    if (m_direction == Direction::STOP) {
        m_direction = newDir;
    }
}

void GhostModel::commitDirection() { m_direction = m_nextDirection; }

void GhostModel::setPosition(double x, double y) {
    m_x = x;
    m_y = y;
    notify(logic::Event::DEFAULT);
}

void GhostModel::stop() { m_direction = Direction::STOP; }

std::pair<double, double> GhostModel::getPositionAt(float dt, Direction dir) const {
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

void GhostModel::accept(Visitor& visitor) { /*visitor.visit(*this); */ }

void GhostModel::update(float dt) {
    double delta = m_speed * dt;

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
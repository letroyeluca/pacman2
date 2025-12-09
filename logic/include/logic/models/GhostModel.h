//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_GHOSTMODEL_H
#define PACMAN_GHOSTMODEL_H

#include "logic/models/EntityModel.h"
#include "logic/models/PacManModel.h"
#include "logic/patterns/Visitor.h"
#include <utility>

namespace logic {

class GhostModel : public EntityModel {
public:
    GhostModel(double x, double y, double width, double height, char type);
    void update(float dt) override;
    void accept(Visitor& visitor) override;
    void queueDirection(Direction dir);
    void commitDirection();
    void stop();
    void setPosition(double x, double y);
    std::pair<double, double> getPositionAt(float dt, Direction dir) const;
    Direction getDirection() const { return m_direction; }
    Direction getNextDirection() const { return m_nextDirection; }
    float getSpeed() const { return m_speed; }
    char getType() const { return m_type; }

private:
    Direction m_direction;
    Direction m_nextDirection;
    float m_speed;
    char m_type;
};

} // namespace logic

#endif // PACMAN_GHOSTMODEL_H

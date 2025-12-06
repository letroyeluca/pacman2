//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_PACMANMODEL_H
#define PACMAN_PACMANMODEL_H

#include "logic/models/EntityModel.h"
#include "logic/patterns/Visitor.h"
#include <utility>

namespace logic {
    enum class Direction {STOP,UP,DOWN,LEFT,RIGHT};

    class PacManModel : public EntityModel {
    public:
        PacManModel(double x, double y, double width, double height);
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

    private:
        Direction m_direction;
        Direction m_nextDirection;
        float m_speed;
    };

}

#endif //PACMAN_PACMANMODEL_H

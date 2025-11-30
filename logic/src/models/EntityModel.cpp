//
// Created by Luca Letroye on 28/11/2025.
//

#include "logic/models/EntityModel.h"
#include <cmath>

namespace logic{
    EntityModel::~EntityModel() = default;

    bool EntityModel::collidesWithBox(double otherX, double otherY, double otherW, double otherH) const {
        double distanceX = std::abs(this->getX() - otherX);
        double distanceY = std::abs(this->getY() - otherY);

        double myHalfW = (this->getWidth() / 2.0) * m_hitboxScale;
        double myHalfH = (this->getHeight() / 2.0) * m_hitboxScale;

        double otherHalfW = otherW / 2.0;
        double otherHalfH = otherH / 2.0;

        double minDistanceX = myHalfW + otherHalfW;
        double minDistanceY = myHalfH + otherHalfH;

        if (distanceX < minDistanceX && distanceY < minDistanceY) {
            return true;
        }
        return false;
    }

    bool EntityModel::collidesWith(const EntityModel& other) const {
        return collidesWithBox(other.getX(),other.getY(),other.getWidth() * other.getHitboxScale(),other.getHeight() * other.getHitboxScale()
        );
    }
}


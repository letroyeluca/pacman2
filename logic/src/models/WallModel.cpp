//
// Created by Luca Letroye on 28/11/2025.
//

#include "logic/models/WallModel.h"
#include "logic/patterns/Visitor.h"

namespace logic{
    WallModel::WallModel(double x, double y, double width, double height) : EntityModel(x, y, width, height)
    {
        m_isActive = true;
        m_hitboxScale = 1;
    }

    void WallModel::update(float dt) {}

    void WallModel::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

}
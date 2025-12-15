//
// Created by Luca Letroye on 15/12/2025.
//
#include "logic/models/AppleModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {

AppleModel::AppleModel(double x, double y, double width, double height) : EntityModel(x, y, width, height) {
    m_isActive = true;
    setHitboxScale(0.2);
}

void AppleModel::accept(Visitor& visitor) { visitor.visit(*this); }

void AppleModel::update(float dt) {}
void AppleModel::collect() {
    if (m_isActive) {
        m_isActive = false;
        notify(logic::Event::AppleEaten);
    }
}

} // namespace logic
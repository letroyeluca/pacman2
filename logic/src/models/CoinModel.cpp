//
// Created by Luca Letroye on 28/11/2025.
//

#include "logic/models/CoinModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {

CoinModel::CoinModel(double x, double y, double width, double height)
    : EntityModel(x, y, width, height) {
  m_isActive = true;
  setHitboxScale(0.2);
}

void CoinModel::accept(Visitor &visitor) { visitor.visit(*this); }

void CoinModel::update(float dt) {}
void CoinModel::collect() {
  if (m_isActive) {
    m_isActive = false;              // 1. Zet inactief
    notify(logic::Event::CoinEaten); // 2. Stuur bericht (Mag hier wel!)
  }
}

} // namespace logic
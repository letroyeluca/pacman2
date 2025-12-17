//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_COINMODEL_H
#define PACMAN_COINMODEL_H

#include "logic/models/EntityModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {
class CoinModel : public EntityModel {
public:
    CoinModel(double x, double y, double width, double height);
    void collect();
    void update(float dt) override;
    void accept(Visitor& visitor) override;
    bool isCollected( ){return !m_isActive;}
};
} // namespace logic

#endif // PACMAN_COINMODEL_H

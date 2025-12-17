//
// Created by Luca Letroye on 15/12/2025.
//

#ifndef PACMAN_APPLEMODEL_H
#define PACMAN_APPLEMODEL_H

#include "logic/models/EntityModel.h"
#include "logic/patterns/Visitor.h"

namespace logic {
class AppleModel : public EntityModel {
public:
    AppleModel(double x, double y, double width, double height);
    void collect();
    void update(float dt) override;
    void accept(Visitor& visitor) override;
};
} // namespace logic
#endif // PACMAN_APPLEMODEL_H

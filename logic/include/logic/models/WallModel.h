//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_WALLMODEL_H
#define PACMAN_WALLMODEL_H

#include "EntityModel.h"

namespace logic{
    class WallModel : public EntityModel {
        public:
            WallModel(double x, double y, double width, double height);
            void update(float dt) override;
            void accept(Visitor& visitor) override;
        };
}

#endif //PACMAN_WALLMODEL_H

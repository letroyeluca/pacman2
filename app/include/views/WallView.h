//
// Created by Luca Letroye on 17/11/2025.
//

#ifndef PACMAN_WALLVIEW_H
#define PACMAN_WALLVIEW_H
#include "views/EntityView.h"

namespace logic {
    class WallModel; // Voorwaartse declaratie
}

class WallView : public EntityView {
public:
    WallView(logic::WallModel& model, Camera& camera);
    // We hoeven onNotify, draw, of updateAnimation
    // niet te overschrijven. De basis 'EntityView'
    // is perfect voor een statisch object.
private:
    sf::Texture m_texture;
};
#endif //PACMAN_WALLVIEW_H

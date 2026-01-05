//
// Created by Luca Letroye on 17/11/2025.
//

#ifndef PACMAN_WALLVIEW_H
#define PACMAN_WALLVIEW_H
#include "views/EntityView.h"
#include <memory>

namespace logic {
class WallModel;
}

class WallView : public EntityView {
public:
    WallView(std::shared_ptr<logic::WallModel> model, Camera& camera, sf::Texture& texture);

private:
    //texture van de muur
    sf::Texture m_texture;
};
#endif // PACMAN_WALLVIEW_H
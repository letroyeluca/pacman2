//
// Created by Luca Letroye on 15/12/2025.
//

#ifndef PACMAN_APPLEVIEW_H
#define PACMAN_APPLEVIEW_H

#include "views/EntityView.h"
#include <memory>

namespace logic {
class AppleModel;
}

class AppleView : public EntityView {
public:
    AppleView(std::shared_ptr<logic::AppleModel> model, Camera& camera, sf::Texture& texture);
    void draw(sf::RenderWindow& window) override;
    void onNotify(const logic::Subject& subject, logic::Event event) override;
    void onWindowResize() override;

private:
    sf::Texture m_texture;
    bool m_isHidden;
};

#endif // PACMAN_APPLEVIEW_H

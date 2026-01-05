#ifndef PACMAN_LIVESVIEW_H
#define PACMAN_LIVESVIEW_H

#include "logic/models/PacManModel.h"
#include "views/EntityView.h"
#include <SFML/Graphics.hpp>
#include <vector>

// UI component: Toont resterende levens linksboven in beeld
class LivesView : public EntityView {
public:
    LivesView(std::shared_ptr<logic::PacManModel> model, Camera& camera);

    void updateAnimation(float dt) override;
    void draw(sf::RenderWindow& window) override; // Tekent meerdere icoontjes in een loop
    void onWindowResize() override;               // Zorgt dat UI op zijn plek blijft bij resize

private:
    std::weak_ptr<logic::PacManModel> m_model;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // UI Positionering
    float m_startX = 20.0f;
    float m_startY = 20.0f;
    float m_spacing = 35.0f;
};

#endif // PACMAN_LIVESVIEW_H
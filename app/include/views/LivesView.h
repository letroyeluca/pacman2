#ifndef PACMAN_LIVESVIEW_H
#define PACMAN_LIVESVIEW_H

#include "views/EntityView.h" // Pas aan naar jouw base view header
#include "logic/models/PacManModel.h"
#include <SFML/Graphics.hpp>
#include <vector>

class LivesView : public EntityView {
public:
    // We hebben het model nodig om de levens te tellen
    LivesView(std::shared_ptr<logic::PacManModel> model, Camera& camera);

    void updateAnimation(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onWindowResize() override; // Om te zorgen dat ze in de hoek blijven

private:
    std::weak_ptr<logic::PacManModel> m_model;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // UI Instellingen
    float m_startX = 20.0f; // Afstand van links
    float m_startY = 20.0f; // Afstand van boven
    float m_spacing = 35.0f; // Ruimte tussen de icoontjes
};

#endif //PACMAN_LIVESVIEW_H
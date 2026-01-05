#ifndef PACMAN_PACMANVIEW_H
#define PACMAN_PACMANVIEW_H
#pragma once
#include "views/EntityView.h"
#include <memory>
#include <vector>

namespace logic {
class PacManModel;
}
class Camera;

// Visuele weergave van de speler met directionele animaties
class PacManView : public EntityView {
public:
    PacManView(std::shared_ptr<logic::PacManModel> model, Camera& camera);

    void updateAnimation(float dt) override; // Wisselt frames op basis van looprichting
    void onWindowResize() override;
    void onNotify(const logic::Subject& subject, logic::Event event) override;

private:
    sf::Texture m_texture;
    // Opslag voor animatieframes per richting
    std::vector<sf::IntRect> m_animFrames; // Huidige actieve set
    std::vector<sf::IntRect> m_animUpFrames;
    std::vector<sf::IntRect> m_animDownFrames;
    std::vector<sf::IntRect> m_animLeftFrames;
    std::vector<sf::IntRect> m_animRightFrames;

    int m_currentFrame;
    float m_animationTimer;
    const float m_animationSpeed = 0.15f; // Snelheid van "happen"
};
#endif // PACMAN_PACMANVIEW_H
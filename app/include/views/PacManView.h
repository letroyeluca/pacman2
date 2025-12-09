//
// Created by Luca Letroye on 26/10/2025.
//
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

class PacManView : public EntityView {
public:
    // Constructor neemt shared_ptr
    PacManView(std::shared_ptr<logic::PacManModel> model, Camera& camera);

    void updateAnimation(float dt) override;
    void onWindowResize() override;
    void onNotify(const logic::Subject& subject, logic::Event event) override;

private:
    sf::Texture m_texture;
    std::vector<sf::IntRect> m_animFrames;
    std::vector<sf::IntRect> m_animUpFrames;
    std::vector<sf::IntRect> m_animDownFrames;
    std::vector<sf::IntRect> m_animLeftFrames;
    std::vector<sf::IntRect> m_animRightFrames;
    int m_currentFrame;
    float m_animationTimer;
    const float m_animationSpeed = 0.1f;
};
#endif // PACMAN_PACMANVIEW_H

//
// Created by Luca Letroye on 26/10/2025.
//

#ifndef PACMAN_GHOSTVIEW_H
#define PACMAN_GHOSTVIEW_H

#include "views/EntityView.h"
#include <memory>
#include <vector>

namespace logic {
class GhostModel;
}
class Camera;

class GhostView : public EntityView {
public:
    // Constructor neemt shared_ptr
    GhostView(std::shared_ptr<logic::GhostModel> model, Camera& camera);

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
    std::vector<sf::IntRect> m_animFramesScared;
    std::vector<sf::IntRect> m_animUpFramesScared;
    std::vector<sf::IntRect> m_animDownFramesScared;
    std::vector<sf::IntRect> m_animLeftFramesScared;
    std::vector<sf::IntRect> m_animRightFramesScared;
    bool m_scared;
    int m_currentFrame;
    float m_animationTimer;
    const float m_animationSpeed = 0.1f;
};

#endif // PACMAN_GHOSTVIEW_H

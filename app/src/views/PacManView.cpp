//
// Created by Luca Letroye on 9/11/2025.
//
#include "views/PacManView.h"
#include "Camera.h"
#include "logic/models/PacManModel.h"
#include <iostream>
#include <stdexcept>

PacManView::PacManView(std::shared_ptr<logic::PacManModel> model, Camera& camera)
    : EntityView(model, camera), // Geef pointer door aan base
      m_currentFrame(0), m_animationTimer(0.f) {
    if (!m_texture.loadFromFile("assets/sprite.png")) {
        throw std::runtime_error("Failed to load assets/sprite.png");
    }
    m_sprite.setTexture(m_texture);

    // ... (frames setup blijft hetzelfde als je origineel) ...
    m_animRightFrames.push_back(sf::IntRect(851, 4, 35, 35));
    m_animRightFrames.push_back(sf::IntRect(851, 54, 35, 35));
    m_animRightFrames.push_back(sf::IntRect(851, 104, 35, 35));
    m_animDownFrames.push_back(sf::IntRect(851, 154, 35, 35));
    m_animDownFrames.push_back(sf::IntRect(851, 204, 35, 35));
    m_animDownFrames.push_back(sf::IntRect(851, 254, 35, 35));
    m_animLeftFrames.push_back(sf::IntRect(851, 304, 35, 35));
    m_animLeftFrames.push_back(sf::IntRect(851, 354, 35, 35));
    m_animLeftFrames.push_back(sf::IntRect(851, 404, 35, 35));
    m_animUpFrames.push_back(sf::IntRect(851, 454, 35, 35));
    m_animUpFrames.push_back(sf::IntRect(851, 504, 35, 35));
    m_animUpFrames.push_back(sf::IntRect(851, 554, 35, 35));

    m_animFrames = m_animRightFrames;
    m_sprite.setTextureRect(m_animRightFrames[0]);
    m_sprite.setOrigin(17.5f, 17.5f); // Iets preciezer voor 35x35
    m_sprite.scale(0.8f, 0.8f);

    EntityView::onNotify(*model, logic::Event::DEFAULT);
}

void PacManView::onNotify(const logic::Subject& subject, logic::Event event) {
    switch (event) {
    case logic::Event::PacManUP:
        m_animFrames = m_animUpFrames;
        break;
    case logic::Event::PacManDOWN:
        m_animFrames = m_animDownFrames;
        break;
    case logic::Event::PacManLEFT:
        m_animFrames = m_animLeftFrames;
        break;
    case logic::Event::PacManRIGHT:
        m_animFrames = m_animRightFrames;
        break;
    default:
        break;
    }
    EntityView::onNotify(subject, event);
}

void PacManView::updateAnimation(float dt) {
    // Cast de base shared_ptr naar PacManModel shared_ptr
    auto pacmanModel = std::dynamic_pointer_cast<logic::PacManModel>(m_model);

    if (!pacmanModel)
        return;

    if (pacmanModel->getDirection() == logic::Direction::STOP) {
        m_animationTimer = 0.f;
        m_currentFrame = 0;
        m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
        return;
    }

    m_animationTimer += dt;
    if (m_animationTimer >= m_animationSpeed) {
        m_currentFrame++;
        if (m_currentFrame >= m_animFrames.size()) {
            m_currentFrame = 0;
        }
        m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
        m_animationTimer -= m_animationSpeed;
    }
}

void PacManView::onWindowResize() {
    EntityView::onWindowResize();
    m_sprite.scale(0.8f, 0.8f);
}
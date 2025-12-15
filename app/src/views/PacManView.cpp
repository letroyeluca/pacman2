//
// Created by Luca Letroye on 9/11/2025.
//
#include "views/PacManView.h"
#include "AssetManager.h"
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
    auto& assets = Asset::AssetManager::getInstance();
    m_sprite.setTexture(assets.getTexture());
    m_animRightFrames = Asset::AssetManager::getAnimationFrames(17, 0);
    m_animDownFrames = Asset::AssetManager::getAnimationFrames(17, 3);
    m_animLeftFrames = Asset::AssetManager::getAnimationFrames(17, 6);
    m_animUpFrames = Asset::AssetManager::getAnimationFrames(17, 9);

    m_animFrames = m_animRightFrames;
    m_sprite.setTextureRect(m_animFrames[0]);
    m_sprite.setOrigin(25.0f, 25.0f);
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
    auto pacmanModel = std::dynamic_pointer_cast<logic::PacManModel>(m_model);
    if (!pacmanModel)
        return;

    // Handle STOP state
    if (pacmanModel->getDirection() == logic::Direction::STOP) {
        m_animationTimer = 0.f;
        m_currentFrame = 0; // Of 1, als je een 'open mond' wilt bij stop
        if (!m_animFrames.empty()) {
            m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
        }
        return;
    }

    // Handle Moving Animation
    m_animationTimer += dt;
    if (m_animationTimer >= m_animationSpeed) {
        m_currentFrame++;
        if (m_currentFrame >= m_animFrames.size()) {
            m_currentFrame = 0;
        }
        m_animationTimer -= m_animationSpeed;
    }

    // --- FIX: Update de sprite ALTIJD, niet alleen als de timer tikt ---
    // Hierdoor pakt hij direct de nieuwe richting (m_animFrames) zodra die verandert.
    if (!m_animFrames.empty()) {
        m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
    }
}

void PacManView::onWindowResize() {
    EntityView::onWindowResize();
    // m_sprite.scale(0.8f, 0.8f);
}
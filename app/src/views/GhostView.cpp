//
// Created by Luca Letroye on 8/12/2025.
//
#include "views/GhostView.h"
#include "AssetManager.h"
#include "Camera.h"
#include "logic/models/GhostModel.h"
#include <iostream>
#include <stdexcept>

GhostView::GhostView(std::shared_ptr<logic::GhostModel> model, Camera& camera)
    : EntityView(model, camera), m_currentFrame(0), m_animationTimer(0.f) {

    // 1. Get AssetManager instance
    auto& assets = Asset::AssetManager::getInstance();

    // 2. Set Texture (Only once, from Manager)
    m_sprite.setTexture(assets.getTexture());

    // 3. Determine Sprite Coordinates based on Ghost Type
    // Assuming standard Pacman spritesheet layout.
    // You might need to adjust these 'y' or 'x' values to match your specific png image.
    int startX = 0; // Column index

    char type = model->getType(); // Make sure GhostModel has this getter!
    std::cout << type << std::endl;
    switch (type) {
    case 'R': // Red (Blinky)
        startX = 0;
        break;
    case 'S':       // Pink (Pinky)
        startX = 1; // Example: 2 columns to the right
        break;
    case 'B': // Blue (Inky)
        startX = 2;
        break;
    case 'O': // Orange (Clyde)
        startX = 3;
        break;
    default:
        startX = 0; // Default to Red
        break;
    }

    // 4. Load Frames
    // We pass startX and startY.
    // The 4 directions usually follow a specific pattern in the sheet relative to the start position.
    // Adjust the "offset" (e.g., +0, +2, +4, +6) based on your specific PNG layout.
    m_animRightFrames = assets.getAnimationFrames(startX, 0, 2); // Row 0
    m_animDownFrames = assets.getAnimationFrames(startX, 2, 2);  // Row 2
    m_animLeftFrames = assets.getAnimationFrames(startX, 4, 2);    // Row 4
    m_animUpFrames = assets.getAnimationFrames(startX, 6, 2);  // Row 6

    // 5. Init
    m_animFrames = m_animRightFrames;
    if (!m_animFrames.empty()) {
        m_sprite.setTextureRect(m_animFrames[0]);
    }
    m_sprite.setOrigin(25.0f, 25.0f);

    EntityView::onNotify(*model, logic::Event::DEFAULT);
}

void GhostView::onNotify(const logic::Subject& subject, logic::Event event) {
    // Determine which animation list to use based on event
    switch (event) {
    case logic::Event::GhostUP:
        m_animFrames = m_animUpFrames;
        break;
    case logic::Event::GhostDOWN:
        m_animFrames = m_animDownFrames;
        break;
    case logic::Event::GhostLEFT:
        m_animFrames = m_animLeftFrames;
        break;
    case logic::Event::GhostRIGHT:
        m_animFrames = m_animRightFrames;
        break;
    default:
        break;
    }
    EntityView::onNotify(subject, event);
}

void GhostView::updateAnimation(float dt) {
    auto ghostModel = std::dynamic_pointer_cast<logic::GhostModel>(m_model);
    if (!ghostModel)
        return;

    // Use TextureRect only to avoid resetting position
    m_animationTimer += dt;
    if (m_animationTimer >= m_animationSpeed) {
        m_currentFrame++;
        if (m_currentFrame >= m_animFrames.size()) {
            m_currentFrame = 0;
        }
        if (!m_animFrames.empty()) {
            m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
        }
        m_animationTimer -= m_animationSpeed;
    }
}

void GhostView::onWindowResize() { EntityView::onWindowResize(); }
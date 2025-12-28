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
    m_animRightFrames = assets.getAnimationFrames(startX, 0, 2);   // Row 0
    m_animDownFrames = assets.getAnimationFrames(startX, 2, 2);    // Row 2
    m_animLeftFrames = assets.getAnimationFrames(startX, 4, 2);    // Row 4
    m_animUpFrames = assets.getAnimationFrames(startX, 6, 2);      // Row 6
    m_animRightFramesScared = assets.getAnimationFrames(0, 11, 2); // Row 0
    m_animDownFramesScared = assets.getAnimationFrames(0, 13, 2);  // Row 2
    m_animLeftFramesScared = assets.getAnimationFrames(0, 15, 2);  // Row 4
    m_animUpFramesScared = assets.getAnimationFrames(0, 17, 2);    // Row 6
    m_animRightFramesScaredEnd = assets.getAnimationFrames(1, 11, 2);
    m_animDownFramesScaredEnd = assets.getAnimationFrames(1, 13, 2); // Row 2
    m_animLeftFramesScaredEnd = assets.getAnimationFrames(1, 15, 2); // Row 4
    m_animUpFramesScaredEnd = assets.getAnimationFrames(1, 17, 2);   // Row 6

    m_animRightFramesScaredEnd.insert(m_animRightFramesScaredEnd.end(), // Plak ze achteraan
                                      m_animRightFramesScared.begin(),  // Kopieer vanaf begin...
                                      m_animRightFramesScared.end()     // ...tot einde van de bange frames
    );

    // 2. DOWN
    m_animDownFramesScaredEnd.insert(m_animDownFramesScaredEnd.end(), m_animDownFramesScared.begin(),
                                     m_animDownFramesScared.end());

    // 3. LEFT
    m_animLeftFramesScaredEnd.insert(m_animLeftFramesScaredEnd.end(), m_animLeftFramesScared.begin(),
                                     m_animLeftFramesScared.end());

    // 4. UP
    m_animUpFramesScaredEnd.insert(m_animUpFramesScaredEnd.end(), m_animUpFramesScared.begin(),
                                   m_animUpFramesScared.end());

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
    case logic::Event::GhostVulnerable:
        m_scared = 1;
        break;
    case logic::Event::GhostVulnerableEnd:
        m_scared = 2;
        break;
    case logic::Event::GhostNormal:
        m_scared = 0;
        break;
    case logic::Event::GhostUP:
        if (m_scared == 0) {
            m_animFrames = m_animUpFrames;
        } else if (m_scared == 1) {
            m_animFrames = m_animUpFramesScared;
        } else {
            m_animFrames = m_animUpFramesScaredEnd;
        }

        break;
    case logic::Event::GhostDOWN:
        if (m_scared == 0) {
            m_animFrames = m_animDownFrames;
        } else if (m_scared == 1) {
            m_animFrames = m_animDownFramesScared;
        } else {
            m_animFrames = m_animDownFramesScaredEnd;
        }
        break;
    case logic::Event::GhostLEFT:
        if (m_scared == 0) {
            m_animFrames = m_animLeftFrames;
        } else if (m_scared == 1) {
            m_animFrames = m_animLeftFramesScared;
        } else {
            m_animFrames = m_animLeftFramesScaredEnd;
        }
        break;
    case logic::Event::GhostRIGHT:
        if (m_scared == 0) {
            m_animFrames = m_animRightFrames;
        } else if (m_scared == 1) {
            m_animFrames = m_animRightFramesScared;
        } else {
            m_animFrames = m_animRightFramesScaredEnd;
        }
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
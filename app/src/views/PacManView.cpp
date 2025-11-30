//
// Created by Luca Letroye on 9/11/2025.
//
#include "views/PacManView.h"
#include "logic/models/PacManModel.h" // Hier wel nodig
#include <iostream>
#include <stdexcept>
#include "Camera.h"

PacManView::PacManView(logic::PacManModel& model, Camera& camera)
        : EntityView(model, camera),
          m_currentFrame(0),         // <-- Initialiseren
          m_animationTimer(0.f)      // <-- Initialiseren
{
    // Laad de spritesheet
    if (!m_texture.loadFromFile("/Users/lucaletroye/Documents/PROJECTEN/pacman/assets/sprite.png")) { // [cite: 62]
        std::cerr << "Error loading sprites.png in PacManView" << std::endl;
        throw std::runtime_error("Failed to load sprites.png");
    }
    m_sprite.setTexture(m_texture);
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
    // Zet het startframe
    m_sprite.setTextureRect(m_animRightFrames[0]);

    m_sprite.setOrigin(16.5, 16.5);
    m_sprite.scale(0.8f, 0.8f);
    onNotify(model, logic::Event::DEFAULT);
}

void PacManView::onNotify(const logic::Subject& subject, logic::Event event) {
    // Dit is de magie: we reageren op de specifieke events
    // die het PacManModel stuurt.

    switch(event) {
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

    // BELANGRIJK: Roep de basis-implementatie aan!
    // Deze 'EntityView::onNotify' zorgt ervoor dat de
    // sprite ook daadwerkelijk van positie verandert.
    EntityView::onNotify(subject, event);
}

void PacManView::updateAnimation(float dt) {

    // 0. Check of Pac-Man beweegt
    // We hebben het model nodig, dus we moeten casten
    auto* pacmanModel = dynamic_cast<logic::PacManModel*>(&m_model);
    if (!pacmanModel) return;

    // Als Pac-Man stilstaat, reset de animatie
    if (pacmanModel->getDirection() == logic::Direction::STOP) {
        m_animationTimer = 0.f;
        m_currentFrame = 0; // Ga terug naar "mond dicht"
        m_sprite.setTextureRect(m_animFrames[m_currentFrame]);
        return;
    }

    // 1. Tel de delta time op bij de timer
    m_animationTimer += dt;

    // 2. Is het tijd om naar het volgende frame te gaan?
    if (m_animationTimer >= m_animationSpeed) {

        // 3. Ga naar het volgende frame
        m_currentFrame++;

        // 4. Als we aan het einde van de animatie zijn, begin opnieuw
        if (m_currentFrame >= m_animFrames.size()) {
            m_currentFrame = 0;
        }

        // 5. Update de sprite op de spritesheet
        m_sprite.setTextureRect(m_animFrames[m_currentFrame]);

        // 6. Trek de frame-tijd van de timer af
        //    (NIET op 0 zetten, anders verlies je 'rest'-tijd)
        m_animationTimer -= m_animationSpeed;
    }
}

// --- NIEUW: De resize functie met marge ---
void PacManView::onWindowResize() {

    EntityView::onWindowResize();
    m_sprite.scale(0.8f, 0.8f);
}
//
// Created by Luca Letroye.
//

#include "states/VictoryState.h"
#include "states/GameState.h"
#include "states/StateManager.h"
#include <iostream>

VictoryState::VictoryState(StateManager& manager, sf::RenderWindow& window, int score, int lives, int nextLevelIndex)
    : State(manager, window), m_score(score), m_lives(lives), m_nextLevelIndex(nextLevelIndex) {
    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Could not load font for VictoryState" << std::endl;
    }

    // 1. Titel
    m_titleText.setFont(m_font);
    m_titleText.setString("LEVEL COMPLETED!");
    m_titleText.setFillColor(sf::Color::Green);
    m_titleText.setStyle(sf::Text::Bold);

    // 2. Score
    m_scoreText.setFont(m_font);
    m_scoreText.setString("Score: " + std::to_string(score));
    m_scoreText.setFillColor(sf::Color::White);

    // 3. Instructie
    m_instructionText.setFont(m_font);
    m_instructionText.setString("Press ENTER for Next Level");
    m_instructionText.setFillColor(sf::Color::Yellow);

    setupText();
}

void VictoryState::setupText() {
    sf::Vector2u windowSize = m_window.getSize();
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;

    // Reset eerst de schaal (belangrijk als het scherm wordt geresized)
    m_titleText.setScale(1.f, 1.f);

    // Zet een basisgrootte (mag best groot zijn, we schalen hem toch omlaag als het moet)
    m_titleText.setCharacterSize(static_cast<unsigned int>(windowSize.y * 0.1f));

    // Haal de afmetingen op
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();

    // Bepaal de maximale breedte die de tekst mag hebben (bv. 80% van het scherm)
    float maxWidth = windowSize.x * 0.8f;

    // Als de tekst breder is dan de limiet, bereken dan de verkleiningsfactor
    if (titleBounds.width > maxWidth) {
        float scaleFactor = maxWidth / titleBounds.width;
        m_titleText.setScale(scaleFactor, scaleFactor);
    }

    // Nu pas de origin zetten (SFML gebruikt de localBounds voor origin, dit blijft correct)
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);

    m_titleText.setPosition(centerX, windowSize.y / 3.0f);

    // --- 2. SCORE
    m_scoreText.setCharacterSize(windowSize.x / 30);
    sf::FloatRect scoreBounds = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(scoreBounds.width / 2.0f, scoreBounds.height / 2.0f);
    m_scoreText.setPosition(centerX, centerY);

    // --- 3. INSTRUCTIE
    m_instructionText.setCharacterSize(windowSize.x / 50);
    sf::FloatRect instBounds = m_instructionText.getLocalBounds();
    m_instructionText.setOrigin(instBounds.width / 2.0f, instBounds.height / 2.0f);
    m_instructionText.setPosition(centerX, centerY + 150);
}

void VictoryState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            // LOGICA VOOR VOLGEND LEVEL
            // We ruimen de oude states op (level 1 & victory scherm)
            m_manager.resetToMenu();

            // En we starten direct een NIEUW level object
            // (Dit zorgt voor Menu -> Level 2 in de stack)
            m_manager.pushState(std::make_unique<GameState>(m_manager, m_window, m_score, m_lives, m_nextLevelIndex));
        } else if (event.key.code == sf::Keyboard::Escape) {
            // Gewoon terug naar menu zonder nieuw level
            m_manager.resetToMenu();
        }
    }
}

void VictoryState::update(float dt) {}

void VictoryState::render() const{
    // We tekenen over de vorige state heen (transparantie zou kunnen),
    // maar hier clearen we voor een schoon scherm.
    m_window.clear(sf::Color::Black); // Donkergrijs
    m_window.draw(m_titleText);
    m_window.draw(m_scoreText);
    m_window.draw(m_instructionText);
}

void VictoryState::handleResize(sf::Event::SizeEvent size) {
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    setupText();
}
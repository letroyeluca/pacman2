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
    m_titleText.setFillColor(sf::Color::Green); // Groen voor winst
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

    // Titel
    m_titleText.setCharacterSize(windowSize.x / 15);
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    m_titleText.setPosition(centerX, windowSize.y / 3.0f);

    // Score
    m_scoreText.setCharacterSize(windowSize.x / 20);
    sf::FloatRect scoreBounds = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(scoreBounds.width / 2.0f, scoreBounds.height / 2.0f);
    m_scoreText.setPosition(centerX, centerY);

    // Instructie
    m_instructionText.setCharacterSize(windowSize.x / 35);
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
            m_manager.pushState(std::make_unique<GameState>(m_manager, m_window,
                                                            m_score,         // Score meenemen
                                                            m_lives,         // Levens meenemen
                                                            m_nextLevelIndex // Difficulty verhogen
                                                            ));
        } else if (event.key.code == sf::Keyboard::Escape) {
            // Gewoon terug naar menu zonder nieuw level
            m_manager.resetToMenu();
        }
    }
}

void VictoryState::update(float dt) {
    // Eventuele animaties
}

void VictoryState::render() {
    // We tekenen over de vorige state heen (transparantie zou kunnen),
    // maar hier clearen we voor een schoon scherm.
    m_window.clear(sf::Color(20, 20, 20)); // Donkergrijs
    m_window.draw(m_titleText);
    m_window.draw(m_scoreText);
    m_window.draw(m_instructionText);
}

void VictoryState::handleResize(sf::Event::SizeEvent size) {
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    setupText();
}
//
// Created by Luca Letroye on 14/12/2025.
//
#include "states/GameOverState.h"
#include "states/StateManager.h"
#include <iostream>
#include <string>

GameOverState::GameOverState(StateManager& manager, sf::RenderWindow& window, int finalScore) : State(manager, window) {

    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }

    // 1. Game Over Titel
    m_titleText.setFont(m_font);
    m_titleText.setString("GAME OVER");
    m_titleText.setFillColor(sf::Color::Red); // Rood is klassiek voor Game Over
    m_titleText.setStyle(sf::Text::Bold);

    // 2. Score
    m_scoreText.setFont(m_font);
    m_scoreText.setString("Final Score: " + std::to_string(finalScore));
    m_scoreText.setFillColor(sf::Color::Yellow);

    // 3. Instructie
    m_instructionText.setFont(m_font);
    m_instructionText.setString("Press ANY KEY to continue");
    m_instructionText.setFillColor(sf::Color::White);

    // Alles positioneren
    setupText();
}

void GameOverState::setupText() {
    sf::Vector2u windowSize = m_window.getSize();
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;

    // Titel instellen (Groot)
    m_titleText.setCharacterSize(windowSize.x / 15);
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    m_titleText.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 3.0f);

    // Score instellen (Middel)
    m_scoreText.setCharacterSize(windowSize.x / 20);
    sf::FloatRect scoreBounds = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(scoreBounds.width / 2.0f, scoreBounds.height / 2.0f);
    m_scoreText.setPosition(centerX, centerY + 20);

    // Instructie instellen (Klein)
    m_instructionText.setCharacterSize(windowSize.x / 40);
    sf::FloatRect instBounds = m_instructionText.getLocalBounds();
    m_instructionText.setOrigin(instBounds.width / 2.0f, instBounds.height / 2.0f);
    m_instructionText.setPosition(centerX, centerY + 150);
}

void GameOverState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        m_manager.resetToMenu();
    }
}

void GameOverState::update(float dt) {}

// toon alles op het scherm
void GameOverState::render() const {
    m_window.clear(sf::Color::Black);
    m_window.draw(m_titleText);
    m_window.draw(m_scoreText);
    m_window.draw(m_instructionText);
}

// bij resize alles opnieuw verschuiven en plaatsen
void GameOverState::handleResize(sf::Event::SizeEvent size) {
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    setupText();
}
//
// Created by Luca Letroye on 27/11/2025.
//
#include "states/MenuState.h"
#include "states/GameState.h"
#include <iostream>

MenuState::MenuState(StateManager& manager, sf::RenderWindow& window) : State(manager, window) {

    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }

    // 1. Titel
    m_titleText.setFont(m_font);
    m_titleText.setString("PAC-MAN");
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setCharacterSize(60);

    // 2. Play knop instructie
    m_playText.setFont(m_font);
    m_playText.setString("Press ENTER to Play");
    m_playText.setFillColor(sf::Color::White);
    m_playText.setCharacterSize(30);

    // 3. Leaderboard Setup
    m_leaderboardTitle.setFont(m_font);
    m_leaderboardTitle.setString("TOP 5 SCORES");
    m_leaderboardTitle.setFillColor(sf::Color::Cyan); // Blauw/Cyaan zoals arcade
    m_leaderboardTitle.setCharacterSize(35);

    // 4. Data laden
    // We maken een tijdelijk model aan puur om de 'highscore.txt' te parsen
    m_tempScoreModel = std::make_unique<logic::ScoreModel>(0, 0, 0);

    initLeaderboard();

    // Zet alles direct goed neer
    sf::Event::SizeEvent size = {static_cast<unsigned int>(window.getSize().x),
                                 static_cast<unsigned int>(window.getSize().y)};
    handleResize(size);
}

void MenuState::resume(){
    initLeaderboard();
}

void MenuState::initLeaderboard() {
    // BELANGRIJK: Maak de oude lijst leeg, anders krijg je dubbele scores
    m_scoreTexts.clear();

    // Data opnieuw laden
    m_tempScoreModel = std::make_unique<logic::ScoreModel>(0, 0, 0);
    const auto& scores = m_tempScoreModel->getTopScores();

    for (size_t i = 0; i < scores.size(); ++i) {
        sf::Text text;
        text.setFont(m_font);
        text.setString(std::to_string(i + 1) + ".  " + std::to_string(scores[i]));
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(25);
        m_scoreTexts.push_back(text);
    }

    // Forceer een resize update om de nieuwe teksten direct goed te positioneren
    sf::Event::SizeEvent size = {
            static_cast<unsigned int>(m_window.getSize().x),
            static_cast<unsigned int>(m_window.getSize().y)
    };
    handleResize(size);
}

void MenuState::handleResize(sf::Event::SizeEvent size) {
    // Reset de view naar de volledige window grootte (UI space)
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));

    float centerX = size.width / 2.0f;
    float centerY = size.height / 2.0f;

    // Titel (Bovenin)
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    m_titleText.setPosition(centerX, centerY - 250);

    // Leaderboard Titel
    sf::FloatRect lbTitleBounds = m_leaderboardTitle.getLocalBounds();
    m_leaderboardTitle.setOrigin(lbTitleBounds.width / 2.0f, lbTitleBounds.height / 2.0f);
    m_leaderboardTitle.setPosition(centerX, centerY - 150);

    // Scores (Onder elkaar)
    float startY = centerY - 100;
    float gap = 40.0f;
    for (size_t i = 0; i < m_scoreTexts.size(); ++i) {
        sf::FloatRect bounds = m_scoreTexts[i].getLocalBounds();
        m_scoreTexts[i].setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        m_scoreTexts[i].setPosition(centerX, startY + (i * gap));
    }

    // Play knop (Onderaan)
    sf::FloatRect playBounds = m_playText.getLocalBounds();
    m_playText.setOrigin(playBounds.width / 2.0f, playBounds.height / 2.0f);
    m_playText.setPosition(centerX, centerY + 200);
}

void MenuState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_manager.popState();
        } else {
            m_manager.pushState(std::make_unique<GameState>(m_manager, m_window));
        }
    }
}

void MenuState::update(float dt) {
    static float timer = 0;
    timer += dt;
    if (timer > 0.5f) {
        if (m_playText.getFillColor() == sf::Color::White)
            m_playText.setFillColor(sf::Color::Transparent);
        else
            m_playText.setFillColor(sf::Color::White);
        timer = 0;
    }
}

void MenuState::render() {
    // Renderen in normale UI volgorde
    m_window.draw(m_titleText);
    m_window.draw(m_leaderboardTitle);

    for (const auto& text : m_scoreTexts) {
        m_window.draw(text);
    }

    m_window.draw(m_playText);
}
//
// Created by Luca Letroye on 4/11/2025.
//

#include "states/GameState.h"
#include "factories/ConcreteFactory.h"
#include "logic/models/CoinModel.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/models/ScoreModel.h"
#include "logic/models/WallModel.h"
#include "states/GameOverState.h"
#include "states/PausedState.h"
#include "states/VictoryState.h"
#include <algorithm>
#include <iostream>
#include <memory>

GameState::GameState(StateManager& manager, sf::RenderWindow& window, int currentScore, int currentLives,
                     int currentLevel)
    : State(manager, window), m_levelIndex(currentLevel) {
    // maak een camera aan voor alles te kunnen renderen
    m_camera = std::make_unique<Camera>(m_window.getSize().x, m_window.getSize().y);

    // maak de factory aan zodat je alle entities kan aanmaken en ontvangen
    auto factory = std::make_shared<ConcreteFactory>(*m_camera);
    m_factory = factory;

    // maak de world aan ahv de factory
    m_world = std::make_unique<logic::World>(m_factory);
    m_world->initializeGameData(currentScore, currentLives, m_levelIndex);

    // gebruik de camera om de dimensies door te geven
    m_camera->setWorldDimensions(m_world->getWidth(), m_world->getHeight());
    m_views = m_factory->getCreatedViews();

    // herschik alle views zodat alles juist staat volgens de render layer
    std::sort(m_views.begin(), m_views.end(),
              [](const auto& viewA, const auto& viewB) { return viewA->getRenderLayer() < viewB->getRenderLayer(); });

    // op voorhand alle views al is resizing zodat ze zeker goed staan.
    for (auto& view : m_views) {
        view->onWindowResize();
    }

    std::cout << "GameState setup compleet. Aantal views: " << m_views.size() << std::endl;
}

GameState::~GameState() {}

// --- Handle Resize ---
void GameState::handleResize(sf::Event::SizeEvent size) {
    // 1. Update View en Camera
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    m_camera->onWindowResize(size.width, size.height);

    // update alle views
    for (auto& view : m_views) {
        view->onWindowResize();
    }
}

// --- Handle Input ---
void GameState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {

        // indien escape geduwt slaag op en pauseer
        if (event.key.code == sf::Keyboard::Escape) {
            if (m_world->getScoreModel()) {
                m_world->getScoreModel()->saveScoreIfPersonalBest();
            }
            m_manager.pushState(std::make_unique<PausedState>(m_manager, m_window));
            return; // Stop verdere input verwerking als we pauzeren
        }
        // bij de bewingen knoppen queue de volgende beweging
        auto pacman = m_world->getPacMan();
        if (pacman) {
            if (event.key.code == sf::Keyboard::Up)
                pacman->queueDirection(logic::Direction::UP);
            else if (event.key.code == sf::Keyboard::Down)
                pacman->queueDirection(logic::Direction::DOWN);
            else if (event.key.code == sf::Keyboard::Left)
                pacman->queueDirection(logic::Direction::LEFT);
            else if (event.key.code == sf::Keyboard::Right)
                pacman->queueDirection(logic::Direction::RIGHT);
        }
    }
}

// --- Update ---
void GameState::update(float dt) {
    m_world->update(dt);

    // 1. CHECK LEVEL COMPLETION
    // De World checkt of munten op zijn en zet m_levelCompleted op true.
    if (m_world->isLevelCompleted()) {

        // Data ophalen uit de huidige wereld
        int currentScore = 0;
        if (m_world->getScoreModel()) {
            currentScore = m_world->getScoreModel()->getScore();
        }

        int currentLives = m_world->getPacMan()->getLives();

        // Naar VictoryState, en zeg dat het volgende level (index + 1) moet komen
        m_manager.pushState(
            std::make_unique<VictoryState>(m_manager, m_window, currentScore, currentLives, m_levelIndex + 1));
        return;
    }

    // 2. CHECK GAME OVER
    if (m_world->getPacMan()->getLives() <= 0) {
        int finalScore = 0;
        if (m_world->getScoreModel()) {
            finalScore = m_world->getScoreModel()->getScore();
            m_world->getScoreModel()->saveScoreIfPersonalBest();
        }
        m_manager.pushState(std::make_unique<GameOverState>(m_manager, m_window, finalScore));
        return;
    }

    // 3. Animaties updaten
    for (auto& view : m_views) {
        view->updateAnimation(dt);
    }
}

void GameState::render() const {
    m_window.clear(sf::Color::Black);
    for (const auto& view : m_views) {
        view->draw(m_window);
    }
}
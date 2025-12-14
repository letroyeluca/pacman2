//
// Created by Luca Letroye on 4/11/2025.
//

#include "states/GameState.h"
#include "factories/ConcreteFactory.h"
#include "logic/models/PacManModel.h"
#include "logic/models/GhostModel.h" // <--- BELANGRIJK: Vergeet deze include niet!
#include "states/PausedState.h"
#include "states/GameOverState.h"
#include <iostream>

// --- Constructor ---
GameState::GameState(StateManager& manager, sf::RenderWindow& window)
        : State(manager, window)
// m_score wordt automatisch geinitialiseerd op 0
{
    std::cout << "GameState initialiseren..." << std::endl;
    m_camera = std::make_unique<Camera>(m_window.getSize().x, m_window.getSize().y);
    m_factory = std::make_unique<ConcreteFactory>(*m_camera);
    m_world = std::make_unique<logic::World>(m_factory.get());
    m_camera->setWorldDimensions(m_world->getWidth(), m_world->getHeight());
    m_views = m_factory->getCreatedViews();

    if (!m_views.empty()) {
        auto scoreView = std::move(m_views.front()); // Pak de eerste
        m_views.erase(m_views.begin());              // Verwijder vooraan
        m_views.push_back(std::move(scoreView));     // Zet achteraan
    }

    // Eenmalige resize triggeren om alles goed te zetten
    for (auto& view : m_views) {
        view->onWindowResize();
    }

    std::cout << "GameState setup compleet. Aantal views: " << m_views.size() << std::endl;
}

// --- Destructor (MOET bestaan voor unique_ptr) ---
GameState::~GameState() {}

// --- Handle Resize ---
void GameState::handleResize(sf::Event::SizeEvent size) {
    // 1. Update View en Camera
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
    m_camera->onWindowResize(size.width, size.height);

    // 2. Update alle views (Pacman, Muren, Coins EN Score zitten hierin)
    for (auto& view : m_views) {
        view->onWindowResize();
    }
}

// --- Handle Input ---
void GameState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {

        // --- PAUSE MENU ---
        if (event.key.code == sf::Keyboard::Escape) {
            if (m_world->getScoreModel()) {
                m_world->getScoreModel()->saveScoreIfPersonalBest();
            }

            m_manager.pushState(std::make_unique<PausedState>(m_manager, m_window));
            return; // Stop verdere input verwerking als we pauzeren
        }

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

    if (m_world->getPacMan()->getLives() <= 0) {

        // Haal de score op
        int finalScore = 0;
        if (m_world->getScoreModel()) {
            finalScore = m_world->getScoreModel()->getScore();
            // Sla eventueel de highscore op
            m_world->getScoreModel()->saveScoreIfPersonalBest();
        }

        // Wissel naar Game Over State
        m_manager.pushState(std::make_unique<GameOverState>(m_manager, m_window, finalScore));
        return;
    }

    // 2. Animaties updaten
    for (auto& view : m_views) {
        view->updateAnimation(dt);
    }
}

// --- Render ---
void GameState::render() {
    m_window.clear(sf::Color::Black);
    for (const auto& view : m_views) {
        view->draw(m_window);
    }
}
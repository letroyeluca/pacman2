//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/GameState.h"
#include "states/PausedState.h"

GameState::GameState(StateManager& manager, sf::RenderWindow& window) : State(manager,window){
    //m_camera = std::make_unique<Camera>(m_window.getsize)
    //m_world =

}

GameState::~GameState() {
}

void GameState::handleResize(sf::Event::SizeEvent size) {
    // 1. Update de SFML View
    m_manager.pushState(std::make_unique<PausedState>(m_manager,m_window));
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.width, size.height)));
/*
    // 2. Camera herberekenen
    m_camera->onWindowResize(size.width, size.height);

    // 3. Alle sprites updaten
    for (auto& view : m_views) {
        view->onWindowResize();
    }
    */
}

// --- Handle Input ---
void GameState::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_manager.pushState(std::make_unique<PausedState>(m_manager, m_window));
        }

        /*auto pacman = m_world->getPacMan();
        if (pacman) {
            if (event.key.code == sf::Keyboard::Up) pacman->queueDirection(logic::Direction::UP);
            else if (event.key.code == sf::Keyboard::Down) pacman->queueDirection(logic::Direction::DOWN);
            else if (event.key.code == sf::Keyboard::Left) pacman->queueDirection(logic::Direction::LEFT);
            else if (event.key.code == sf::Keyboard::Right) pacman->queueDirection(logic::Direction::RIGHT);
        }
         */
    }
}

// --- Update ---
void GameState::update(float dt) {

}

// --- Render ---
void GameState::render() {
    m_window.clear(sf::Color::Yellow);


}

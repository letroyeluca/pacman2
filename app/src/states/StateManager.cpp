//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/StateManager.h"

void StateManager::pushState(std::unique_ptr<State> newState) { m_states.push(std::move(newState)); }

void StateManager::popState() {
    if (!m_states.empty()) {
        m_states.pop();
    }
    if (!m_states.empty()) {
        m_states.top()->resume();
    }
}

// --- NIEUWE FUNCTIE ---
// Zorgt dat we veilig teruggaan naar de onderste state (Menu)
void StateManager::resetToMenu() {
    // Verwijder states tot er nog maar 1 over is (MenuState)
    while (m_states.size() > 1) {
        m_states.pop();
    }
    // Resume het menu als het er nog is
    if (!m_states.empty()) {
        m_states.top()->resume();
    }
}
// ----------------------

void StateManager::handleInput(sf::Event& event) {
    if (!m_states.empty()) {
        m_states.top()->handleInput(event);
    }
}

void StateManager::update(float dt) {
    if (!m_states.empty()) {
        m_states.top()->update(dt);
    }
}

void StateManager::render() {
    if (!m_states.empty()) {
        m_states.top()->render();
    }
}

void StateManager::handleResize(sf::Event::SizeEvent size) {
    if (!m_states.empty()) {
        m_states.top()->handleResize(size);
    }
}
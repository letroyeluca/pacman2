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

void StateManager::handleInput(sf::Event& event) {
    if (!m_states.empty()) {
        m_states.top()->handleInput(event);
    }
}

void StateManager::resetToMenu() {
    // Zet vlaggetje
    m_isResetting = true;
}

void StateManager::processStateChanges() {
    // 1. Verwijderen (Pop)
    if (m_isRemoving && !m_states.empty()) {
        m_states.pop();
        if (!m_states.empty()) {
            m_states.top()->resume();
        }
        m_isRemoving = false;
    }

    // 2. Resetten naar Menu
    if (m_isResetting) {
        while (m_states.size() > 1) {
            m_states.pop();
        }
        if (!m_states.empty()) {
            m_states.top()->resume();
        }
        m_isResetting = false;
    }

    // 3. Toevoegen (Push)
    if (m_pendingState) {
        // Pauzeer de huidige
        if (!m_states.empty()) {
            // m_states.top()->pause(); // Optioneel als je pause() hebt
        }
        m_states.push(std::move(m_pendingState));
        m_pendingState = nullptr; // Reset pointer
    }
}

void StateManager::update(float dt) {
    // EERST wijzigingen verwerken (veilig moment)
    processStateChanges();

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

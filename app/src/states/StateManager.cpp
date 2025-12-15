//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/StateManager.h"

void StateManager::pushState(std::unique_ptr<State> newState) {
    // Sla hem op, voeg later toe (Safe Push)
    m_pendingState = std::move(newState);
}

void StateManager::popState() {
    // Markeer voor verwijdering (Safe Pop)
    m_isRemoving = true;
}

void StateManager::resetToMenu() {
    // Markeer voor reset
    m_isResetting = true;
}

void StateManager::processStateChanges() {
    // 1. Verwijderen (Pop)
    if (m_isRemoving && !m_states.empty()) {
        m_states.pop();

        // Resume de vorige state als die er is
        if (!m_states.empty()) {
            m_states.top()->resume();
        }
        m_isRemoving = false;
    }

    // 2. Resetten naar Menu (Alles weg behalve onderste)
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
        // Pauzeer de huidige state als die er is
        if (!m_states.empty()) {
            // m_states.top()->pause(); // Optioneel
        }
        m_states.push(std::move(m_pendingState));
        m_pendingState = nullptr;
    }
}

void StateManager::handleInput(sf::Event& event) {
    if (!m_states.empty()) {
        m_states.top()->handleInput(event);
    }
}

void StateManager::update(float dt) {
    // EERST de wijzigingen verwerken (voordat we updaten)
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
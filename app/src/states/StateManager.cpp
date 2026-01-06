//
// Created by Luca Letroye on 27/11/2025.
//

#include "states/StateManager.h"

// een staat bovenop de stack steken
void StateManager::pushState(std::unique_ptr<State> newState) { m_pendingState = std::move(newState); }

// de bovenste staat van de stack pushen
void StateManager::popState() { m_isRemoving = true; }
// reset van de stack
void StateManager::resetToMenu() { m_isResetting = true; }

// het verwerken van elke verandering aan de stack
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

// de input handle doorgeven naar de bovenste staat
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

// render doorgeven naar de bovenste stack
void StateManager::render() const{
    if (!m_states.empty()) {
        m_states.top()->render();
    }
}

// resize doorgeven naar de bovenste stack
void StateManager::handleResize(sf::Event::SizeEvent size) {
    if (!m_states.empty()) {
        m_states.top()->handleResize(size);
    }
}
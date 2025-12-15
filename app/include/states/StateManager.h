#ifndef PACMAN_STATEMANAGER_H // <--- FIX 1: INCLUDE GUARDS
#define PACMAN_STATEMANAGER_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    void pushState(std::unique_ptr<State> newState);
    void popState();

    // Veilige reset functie (voor Game Over -> Menu)
    void resetToMenu();

    void handleInput(sf::Event& event);
    void update(float dt);
    void render();
    void handleResize(sf::Event::SizeEvent size);

    // FIX 2: De functie die Game.cpp zoekt
    bool isRunning() const { return !m_states.empty(); }

    // De functie die de wijzigingen veilig doorvoert (voor Valgrind fix)
    void processStateChanges();

private:
    std::stack<std::unique_ptr<State>> m_states;

    // Buffer variabelen voor safe deletion/transition
    std::unique_ptr<State> m_pendingState = nullptr;
    bool m_isRemoving = false;
    bool m_isResetting = false;
};

#endif // PACMAN_STATEMANAGER_H
#ifndef PACMAN_STATEMANAGER_H
#define PACMAN_STATEMANAGER_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

// Beheert de actieve states via een Stack-systeem
class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    void pushState(std::unique_ptr<State> newState); // Voegt een state toe
    void popState();                                 // Verwijdert de bovenste state

    // Verwijdert alle states en keert terug naar het hoofdmenu
    void resetToMenu();

    // Delegeert de game-loop aanroepen naar de bovenste (actieve) state
    void handleInput(sf::Event& event);
    void update(float dt);
    void render();
    void handleResize(sf::Event::SizeEvent size);

    // Controleert of het spel nog draait (states heeft)
    bool isRunning() const { return !m_states.empty(); }

    // Voert aangevraagde wijzigingen (push/pop) veilig door aan het begin van de frame
    void processStateChanges();

private:
    std::stack<std::unique_ptr<State>> m_states; // De stapel met actieve states

    // Buffer variabelen om crashes te voorkomen bij het wisselen van states tijdens updates
    std::unique_ptr<State> m_pendingState = nullptr;
    bool m_isRemoving = false;
    bool m_isResetting = false;
};

#endif // PACMAN_STATEMANAGER_H
// states/StateManager.h

#include "State.h"
#include <memory>
#include <stack>

class StateManager {
public:
    // ... bestaande functies ...
    void pushState(std::unique_ptr<State> newState);
    void popState();
    void resetToMenu();

    void handleInput(sf::Event& event);
    void update(float dt);
    void render();
    void handleResize(sf::Event::SizeEvent size);

    // NIEUW: Functie om de wijzigingen echt uit te voeren
    void processStateChanges();

private:
    std::stack<std::unique_ptr<State>> m_states;

    // NIEUW: Buffer voor nieuwe state en flags voor acties
    std::unique_ptr<State> m_pendingState = nullptr;
    bool m_isRemoving = false;
    bool m_isResetting = false;
};
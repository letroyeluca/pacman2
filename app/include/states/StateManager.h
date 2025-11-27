//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_STATEMANAGER_H
#define PACMAN_STATEMANAGER_H

#include <stack>
#include "State.h"
#include <memory>

class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    void pushState(std::unique_ptr<State> newState);
    void popState();

    void handleInput(sf::Event& event);
    void update(float dt);
    void render();
    void handleResize(sf::Event::SizeEvent size);

    bool isRunning() const {return !m_states.empty();}

private:
    std::stack<std::unique_ptr<State>> m_states;
};


#endif //PACMAN_STATEMANAGER_H

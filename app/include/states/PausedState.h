//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_PAUSEDSTATE_H
#define PACMAN_PAUSEDSTATE_H

#include "State.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include "Camera.h"

class PausedState : public State {
private:
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_instruction;
    std::unique_ptr<Camera> m_camera;

public:
    PausedState(StateManager& manager,sf::RenderWindow& window);

    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void render() override;
    void handleResize(sf::Event::SizeEvent size) override;
    void setupText();
};


#endif //PACMAN_PAUSEDSTATE_H

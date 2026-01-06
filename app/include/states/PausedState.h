#ifndef PACMAN_PAUSEDSTATE_H
#define PACMAN_PAUSEDSTATE_H

#include "Camera.h"
#include "State.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>

// Overlay-state die de game-loop eronder bevriest
class PausedState : public State {
private:
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_instruction;
    std::unique_ptr<Camera> m_camera; // Camera nodig om tekst correct te positioneren over de game

public:
    PausedState(StateManager& manager, sf::RenderWindow& window);

    void handleInput(sf::Event& event) override; // ESC om te hervatten
    void update(float dt) override;              // Doet meestal niets (tijd staat stil)
    void render() const override;                  // Tekent "PAUSED" over het huidige beeld heen
    void handleResize(sf::Event::SizeEvent size) override;
    void setupText();
};

#endif // PACMAN_PAUSEDSTATE_H
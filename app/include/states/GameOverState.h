#ifndef PACMAN_GAMEOVERSTATE_H
#define PACMAN_GAMEOVERSTATE_H

#include "states/State.h"
#include <SFML/Graphics.hpp>

class GameOverState : public State {
public:
    // We geven de score mee aan de constructor
    GameOverState(StateManager& manager, sf::RenderWindow& window, int finalScore);
    ~GameOverState() override = default;

    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void render() override;
    void handleResize(sf::Event::SizeEvent size) override;

private:
    void setupText();

    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_scoreText;
    sf::Text m_instructionText;
};

#endif // PACMAN_GAMEOVERSTATE_H
#ifndef PACMAN_GAMEOVERSTATE_H
#define PACMAN_GAMEOVERSTATE_H

#include "states/State.h"
#include <SFML/Graphics.hpp>

// Scherm dat getoond wordt als de speler alle levens kwijt is
class GameOverState : public State {
public:
    // Constructor ontvangt finalScore om deze nog één keer te tonen
    GameOverState(StateManager& manager, sf::RenderWindow& window, int finalScore);
    ~GameOverState() override = default;

    void handleInput(sf::Event& event) override; // Wacht op input om te herstarten of stoppen
    void update(float dt) override;
    void render() override;
    void handleResize(sf::Event::SizeEvent size) override;

private:
    void setupText(); // Configureert posities en stijl van de teksten

    sf::Font m_font;
    sf::Text m_titleText;      // "GAME OVER"
    sf::Text m_scoreText;      // Behaalde score
    sf::Text m_instructionText;// "Press Space to menu"
};

#endif // PACMAN_GAMEOVERSTATE_H
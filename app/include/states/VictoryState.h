#ifndef PACMAN_VICTORYSTATE_H
#define PACMAN_VICTORYSTATE_H

#include "states/State.h"
#include <SFML/Graphics.hpp>

// Tussenscherm na het voltooien van een level
class VictoryState : public State {
public:
    // Ontvangt stats (score/levens) en geeft deze door aan het volgende level
    VictoryState(StateManager& manager, sf::RenderWindow& window, int score, int lives, int nextLevelIndex);
    ~VictoryState() override = default;

    void handleInput(sf::Event& event) override; // Enter drukken om volgend level te starten
    void update(float dt) override;
    void render() override;
    void handleResize(sf::Event::SizeEvent size) override;

private:
    void setupText();

    sf::Font m_font;
    sf::Text m_titleText;       // "Level Complete!"
    sf::Text m_scoreText;
    sf::Text m_instructionText;

    // Data om mee te nemen naar de volgende GameState
    int m_score;
    int m_lives;
    int m_nextLevelIndex;
};

#endif // PACMAN_VICTORYSTATE_H
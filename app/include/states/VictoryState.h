//
// Created by Luca Letroye.
//

#ifndef PACMAN_VICTORYSTATE_H
#define PACMAN_VICTORYSTATE_H

#include "states/State.h"
#include <SFML/Graphics.hpp>

class VictoryState : public State {
public:
    VictoryState(StateManager& manager, sf::RenderWindow& window,int score, int lives, int nextLevelIndex);
    ~VictoryState() override = default;

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

    int m_score;
    int m_lives;
    int m_nextLevelIndex;
};

#endif //PACMAN_VICTORYSTATE_H
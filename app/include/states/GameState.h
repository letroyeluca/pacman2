//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_GAMESTATE_H
#define PACMAN_GAMESTATE_H

#include "logic/World.h"
#include "State.h"
#include "Camera.h"
#include <memory>
#include <vector>

class GameState : public State {
public:
    GameState(StateManager& manager, sf::RenderWindow& window);
    ~GameState();

    void handleInput(sf::Event& event) override;
    void update(float dt) override;
    void render() override;
    void handleResize(sf::Event::SizeEvent size) override;
private:
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<logic::World> m_world;
};


#endif //PACMAN_GAMESTATE_H

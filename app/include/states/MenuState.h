//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_MENUSTATE_H
#define PACMAN_MENUSTATE_H

#include "states/State.h"
#include "states/StateManager.h"
#include "Camera.h"

class MenuState : public State {
public:
    MenuState(StateManager& manager, sf::RenderWindow& window);

     void handleInput(sf::Event& event) override;
     void update(float dt) override;
     void render() override;
     void handleResize(sf::Event::SizeEvent size) override;
private:
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_instruction;
    std::unique_ptr<Camera> m_camera;

};


#endif //PACMAN_MENUSTATE_H

//
// Created by Luca Letroye on 26/11/2025.
//

#include "Game.h"
#include "states/StateManager.h"
#include "states/MenuState.h"
#include <SFML/Graphics.hpp>
#include <memory>

void Game::run(int width, int height){

    // initialiseer de window
    sf::RenderWindow window(sf::VideoMode(width, height), "PACMAN");
    //zet de framerate limiet
    window.setFramerateLimit(60);

    StateManager stateManager;
    stateManager.pushState(std::make_unique<MenuState>(stateManager,window));

    //window loop
    while (window.isOpen()){
        //verwerk mogelijke events
        sf::Event event;
        while(window.pollEvent(event)){
            //sluiten van window
            if(event.type == sf::Event::Closed){
                window.close();
            }
            //resizing
            if(event.type == sf::Event::Resized){
                stateManager.handleResize(event.size);
            }
            stateManager.handleInput(event);
        }

        //stateManager.update(dt);
        window.clear();
        stateManager.render();
        window.display();

        if(!stateManager.isRunning()){
            window.close();
        }
    }
}

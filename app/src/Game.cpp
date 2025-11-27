//
// Created by Luca Letroye on 26/11/2025.
//

#include "Game.h"
#include <SFML/Graphics.hpp>

void Game::run(int width, int height){

    // initialiseer de window
    sf::RenderWindow window(sf::VideoMode(width, height), "PACMAN");
    //zet de framerate limiet
    window.setFramerateLimit(60);

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
                //statemanager verwerkt resize
            }
            // statemanager
        }

        //rendering logica
        window.clear();
        //statemamanager Render
        window.display();
    }
}

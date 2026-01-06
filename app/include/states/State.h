#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H

#include <SFML/Graphics.hpp>

class StateManager;

// Abstracte basisklasse voor alle speltoestanden (Menu, Game, Pause, etc.)
class State {
public:
    virtual ~State() = default;

    // Pure virtual functies: elke child-klasse MOET deze implementeren
    virtual void handleInput(sf::Event& event) = 0; // Verwerkt toetsenbord/muis events
    virtual void update(float dt) = 0;              // Update spellogica (tijdsgebonden)
    virtual void render() const = 0;                      // Tekent de state op het scherm

    // Optionele functies (hooks)
    virtual void resume() {} // Wordt aangeroepen als deze state weer actief wordt (na een pop)
    virtual void handleResize(sf::Event::SizeEvent size) {} // Schermgrootte aanpassingen

protected:
    // Constructor is protected: je mag geen 'State' aanmaken, alleen afgeleide klasses
    State(StateManager& manager, sf::RenderWindow& window) : m_manager(manager), m_window(window) {};

    StateManager& m_manager;    // Referentie om van state te wisselen
    sf::RenderWindow& m_window; // Referentie naar het tekenvenster
};

#endif // PACMAN_STATE_H
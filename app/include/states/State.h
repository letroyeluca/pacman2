//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H

#include <SFML/Graphics.hpp>

class StateManager;

class State {
public:
  virtual ~State() = default;
  virtual void handleInput(sf::Event &event) = 0;
  virtual void update(float dt) = 0;
  virtual void render() = 0;
  virtual void handleResize(sf::Event::SizeEvent size) {}

protected:
  State(StateManager &manager, sf::RenderWindow &window)
      : m_manager(manager), m_window(window) {};
  StateManager &m_manager;
  sf::RenderWindow &m_window;
};

#endif // PACMAN_STATE_H

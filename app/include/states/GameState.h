//
// Created by Luca Letroye on 4/11/2025.
//

#ifndef PACMAN_GAMESTATE_H
#define PACMAN_GAMESTATE_H

#pragma once
#include "Camera.h"
#include "State.h"
#include "logic/World.h"
#include "views/EntityView.h"
#include "views/ScoreView.h"
#include <memory>
#include <vector>

class GameState : public State {
private:
  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<logic::AbstractFactory> m_factory;
  std::unique_ptr<logic::World> m_world;
  std::vector<std::unique_ptr<EntityView>> m_views;
  std::shared_ptr<logic::ScoreModel> m_scoreModel;
  std::shared_ptr<ScoreView> m_scoreView;

public:
  GameState(StateManager &manager, sf::RenderWindow &window);
  ~GameState();

  void handleInput(sf::Event &event) override;
  void update(float dt) override;
  void render() override;
  void handleResize(sf::Event::SizeEvent size) override;
};
#endif // PACMAN_GAMESTATE_H

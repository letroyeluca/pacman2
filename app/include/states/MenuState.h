//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_MENUSTATE_H
#define PACMAN_MENUSTATE_H

#include "Camera.h"
#include "logic/models/ScoreModel.h"
#include "states/State.h"
#include "states/StateManager.h"

class MenuState : public State {
public:
  MenuState(StateManager &manager, sf::RenderWindow &window);

  void handleInput(sf::Event &event) override;
  void update(float dt) override;
  void render() override;
  void handleResize(sf::Event::SizeEvent size) override;

private:
  sf::Font m_font;
  sf::Text m_titleText;
  sf::Text m_playText;
  std::unique_ptr<Camera> m_camera;

  sf::Text m_leaderboardTitle;
  std::vector<sf::Text> m_scoreTexts;
  std::unique_ptr<logic::ScoreModel> m_tempScoreModel;
};

#endif // PACMAN_MENUSTATE_H

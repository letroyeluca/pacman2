//
// Created by Luca Letroye on 28/11/2025.
//
#ifndef PACMAN_SCOREMODEL_H
#define PACMAN_SCOREMODEL_H

#include "logic/models/EntityModel.h" // <--- DEZE ONTMANK
#include "logic/patterns/Observer.h"
#include <string>

namespace logic {

// Zorg dat EntityModel hierboven ge-include is!
class ScoreModel : public EntityModel, public Observer {
public:
  ScoreModel(double x, double y, double size); // Constructor zonder body hier

  void onNotify(const Subject &subject, Event event) override;

  int getScore() const { return m_currentScore; }
  int getHighScore() const { return m_highScore; }

  void update(float dt) override;
  void accept(Visitor &visitor) override;

private:
  void addPoints(int points);
  void loadHighScore();
  void saveHighScore();

  int m_currentScore;
  int m_highScore;
  float m_lastCoinEatTime;
};
} // namespace logic
#endif

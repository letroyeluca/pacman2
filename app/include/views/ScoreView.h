//
// Created by Luca Letroye on 6/12/2025.
//
#ifndef PACMAN_SCOREVIEW_H
#define PACMAN_SCOREVIEW_H

#include "logic/models/ScoreModel.h"
#include "views/EntityView.h" // Erf van EntityView
#include <memory>

// GEEN 'public logic::Observer' hier, want EntityView is al een Observer!
class ScoreView : public EntityView {
public:
  ScoreView(std::shared_ptr<logic::ScoreModel> model, Camera &camera);

  void onNotify(const logic::Subject &subject, logic::Event event) override;

  void onWindowResize() override;

  void draw(sf::RenderWindow &window) override;

private:
  std::shared_ptr<logic::ScoreModel>
      m_scoreModel; // Specifiek score model opslaan
  sf::Font m_font;
  sf::Text m_scoreText;
  sf::Text m_highScoreText;
};

#endif // PACMAN_SCOREVIEW_H
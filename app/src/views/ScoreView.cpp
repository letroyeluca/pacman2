//
// Created by Luca Letroye on 6/12/2025.
//
#include "views/ScoreView.h"
#include <iostream>

ScoreView::ScoreView(std::shared_ptr<logic::ScoreModel> model, Camera &camera)
    : EntityView(model, camera), // Base met nullptr initialiseren
      m_scoreModel(model) {
  if (!m_font.loadFromFile("assets/font.ttf")) {
    std::cerr << "ERROR: Font assets/font.ttf niet gevonden!" << std::endl;
  }

  m_scoreText.setFont(m_font);
  m_scoreText.setFillColor(sf::Color::White);
  m_highScoreText.setFont(m_font);
  m_highScoreText.setFillColor(sf::Color::Yellow);

  m_scoreText.setString("SCORE: 0");
  m_highScoreText.setString("HIGH: " +
                            std::to_string(m_scoreModel->getHighScore()));

  // Attachen aan het model
  m_scoreModel->attach(this);
}

void ScoreView::onNotify(const logic::Subject &subject, logic::Event event) {
  if (event == logic::Event::ScoreChanged) {
    m_scoreText.setString("SCORE: " + std::to_string(m_scoreModel->getScore()));
    m_highScoreText.setString("HIGH: " +
                              std::to_string(m_scoreModel->getHighScore()));
  }
}

void ScoreView::onWindowResize() {
  EntityView::onWindowResize();
  if (!m_model)
    return;

  // 1. Waar moet het staan? (Het exacte midden van de 2-tegel balk)
  sf::Vector2f anchorPos = m_camera.project(m_model->getX(), m_model->getY());

  // 2. Hoe groot is 1 tegel in pixels?
  // We gebruiken de breedte/hoogte die we in de loader hebben meegegeven
  // (calculatedTileSize)
  double logicTileSize = m_model->getHeight();
  sf::Vector2f pixelSize =
      m_camera.computeSpriteSize(logicTileSize, logicTileSize);

  // 3. Font instellen
  // We willen dat de tekst ongeveer even groot is als 1 tegel (zodat er ruimte
  // boven/onder is in de 2-tegel balk)
  unsigned int fontSize = static_cast<unsigned int>(pixelSize.y);
  if (fontSize < 1)
    fontSize = 1;

  m_scoreText.setCharacterSize(fontSize);
  m_highScoreText.setCharacterSize(fontSize);

  // 4. Positionering
  // De anchorPos is het midden van het scherm.
  // We zetten de Score links daarvan en Highscore rechts.
  // We gebruiken een offset van een half schermbreedte (0.5 logic width)
  float xOffset = m_camera.computeSpriteSize(0.6, 0).x;

  m_scoreText.setPosition(anchorPos.x - xOffset, anchorPos.y);
  m_highScoreText.setPosition(anchorPos.x + 0.2f * xOffset, anchorPos.y);

  // 5. VERTICAAL CENTREREN (Cruciaal!)
  // We zetten de origin van de tekst in het midden van de letters.
  // Hierdoor lijnt het midden van de tekst uit met onze scoreY.

  // Let op: getLocalBounds().top is belangrijk bij fonts voor exacte uitlijning
  sf::FloatRect boundsScore = m_scoreText.getLocalBounds();
  m_scoreText.setOrigin(boundsScore.left,
                        boundsScore.top + boundsScore.height / 2.0f);

  sf::FloatRect boundsHigh = m_highScoreText.getLocalBounds();
  m_highScoreText.setOrigin(boundsHigh.left,
                            boundsHigh.top + boundsHigh.height / 2.0f);
}

void ScoreView::draw(sf::RenderWindow &window) {
  window.draw(m_scoreText);
  window.draw(m_highScoreText);
}
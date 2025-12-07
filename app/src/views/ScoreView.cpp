//
// Created by Luca Letroye on 6/12/2025.
//
#include "views/ScoreView.h"
#include <iostream>

ScoreView::ScoreView(std::shared_ptr<logic::ScoreModel> model, Camera& camera)
        : EntityView(model, camera), // Base met nullptr initialiseren
          m_scoreModel(model)
{
    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERROR: Font assets/font.ttf niet gevonden!" << std::endl;
    }

    m_scoreText.setFont(m_font);
    m_scoreText.setFillColor(sf::Color::White);
    m_highScoreText.setFont(m_font);
    m_highScoreText.setFillColor(sf::Color::Yellow);

    m_scoreText.setString("SCORE: 0");
    m_highScoreText.setString("HIGH: " + std::to_string(m_scoreModel->getHighScore()));

    // Attachen aan het model
    m_scoreModel->attach(this);
}

void ScoreView::onNotify(const logic::Subject& subject, logic::Event event) {
    if (event == logic::Event::ScoreChanged) {
        m_scoreText.setString("SCORE:" + std::to_string(m_scoreModel->getScore()));
        m_highScoreText.setString("HIGH:" + std::to_string(m_scoreModel->getHighScore()));
    }
}

void ScoreView::onWindowResize() {
    EntityView::onWindowResize();
    if (!m_model) return;

    // 1. Projecteer het ankerpunt (dit is nu de linkerkant van de map)
    sf::Vector2f anchorPos = m_camera.project(m_model->getX(), m_model->getY());

    // 2. Font grootte (blijft hetzelfde)
    double logicSize = m_model->getHeight();
    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicSize, logicSize);
    unsigned int fontSize = static_cast<unsigned int>(pixelSize.y);
    if (fontSize < 1) fontSize = 1;

    m_scoreText.setCharacterSize(fontSize);
    m_highScoreText.setCharacterSize(fontSize);

    // 3. ORIGIN AANPASSEN (Links uitlijnen)
    // We zetten de X-origin op 0 (links) in plaats van bounds.width/2
    // De Y-origin blijft gecentreerd zodat hij netjes in de balk hangt

    sf::FloatRect boundsScore = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(0, boundsScore.top + boundsScore.height / 2.0f);

    sf::FloatRect boundsHigh = m_highScoreText.getLocalBounds();
    m_highScoreText.setOrigin(0, boundsHigh.top + boundsHigh.height / 2.0f);

    // 4. POSITIES INSTELLEN

    // SCORE: Staat direct op het ankerpunt (helemaal links)
    // We voegen een heel klein beetje pixel-offset toe zodat hij niet tegen de rand plakt
    float padding = pixelSize.x * 0.5f;
    m_scoreText.setPosition(anchorPos.x, anchorPos.y);

    // HIGHSCORE: Staat rechts van de score.
    // We kunnen een vaste logische afstand nemen.
    // De hele map is 'worldWidth' breed. Laten we hem op de helft zetten?
    // Of gewoon een stuk naar rechts.

    // Bereken afstand voor highscore (bijv. 0.8 logic units naar rechts)
    float highscoreOffset = m_camera.computeSpriteSize(1.0, 0).x;

    m_highScoreText.setPosition(anchorPos.x + highscoreOffset, anchorPos.y);
}

void ScoreView::draw(sf::RenderWindow& window) {
    window.draw(m_scoreText);
    window.draw(m_highScoreText);
}
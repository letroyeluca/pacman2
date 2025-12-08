//
// Created by Luca Letroye on 6/12/2025.
//
#include "views/ScoreView.h"
#include <iostream>

ScoreView::ScoreView(std::shared_ptr<logic::ScoreModel> model, Camera& camera)
    : EntityView(model, camera), // Base met nullptr initialiseren
      m_scoreModel(model) {
    if (!m_font.loadFromFile("assets/font.ttf")) {
        std::cerr << "ERROR: Font assets/font.ttf niet gevonden!" << std::endl;
    }

    m_scoreText.setFont(m_font);
    m_scoreText.setFillColor(sf::Color::White);
    m_highScoreText.setFont(m_font);
    m_highScoreText.setFillColor(sf::Color::Yellow);

    m_scoreText.setString("SCORE:0");
    m_highScoreText.setString("HIGH:" + std::to_string(m_scoreModel->getHighScore()));

    // Attachen aan het model
    m_scoreModel->attach(this);
}

void ScoreView::onNotify(const logic::Subject& subject, logic::Event event) {
    if (!m_scoreModel)
        return;

    if (event == logic::Event::ScoreChanged) {
        // 1. Update de tekst
        m_scoreText.setString("SCORE:" + std::to_string(m_scoreModel->getScore()));
        m_highScoreText.setString("HIGH:" + std::to_string(m_scoreModel->getHighScore()));

        // 2. CRUCIAAL: Update de Origin van Highscore
        // Omdat de tekst nu breder kan zijn (bv. 10 -> 100), moeten we het
        // ankerpunt opnieuw rechts leggen. Hierdoor groeit de tekst naar LINKS.
        sf::FloatRect highBounds = m_highScoreText.getLocalBounds();
        m_highScoreText.setOrigin(highBounds.width, highBounds.top + highBounds.height / 2.0f);

        // Voor Score (links uitgelijnd) hoeft dit niet, want Origin X blijft 0.
    }
}

void ScoreView::onWindowResize() {
    EntityView::onWindowResize();
    if (!m_model)
        return;

    // --- POSITIE BEPALEN ---

    // 1. Linkerkant van de map (Start X)
    double leftLogicX = m_model->getX();

    // 2. Rechterkant van de map (End X)
    // Omdat de map gecentreerd is rond 0, is de rechterkant het tegenovergestelde
    // van links. (Bijv: Links = -1.0, Rechts = 1.0)
    double rightLogicX = -leftLogicX;

    double logicY = m_model->getY();

    // 3. Projecteer naar pixels
    sf::Vector2f leftAnchor = m_camera.project(leftLogicX, logicY);
    sf::Vector2f rightAnchor = m_camera.project(rightLogicX, logicY);

    // --- FONT GROOTTE ---
    double logicSize = m_model->getHeight();
    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicSize, logicSize);
    unsigned int fontSize = static_cast<unsigned int>(pixelSize.y);
    if (fontSize < 1)
        fontSize = 1;

    m_scoreText.setCharacterSize(fontSize);
    m_highScoreText.setCharacterSize(fontSize);

    // SCORE (Links): Ankerpunt links + padding
    m_scoreText.setPosition(leftAnchor.x, leftAnchor.y);
    sf::FloatRect scoreBounds = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(0, scoreBounds.top + scoreBounds.height / 2.0f);

    // HIGHSCORE (Rechts): Ankerpunt rechts - padding
    m_highScoreText.setPosition(rightAnchor.x, rightAnchor.y);

    // Origin updaten (voor zekerheid bij resize)
    sf::FloatRect highBounds = m_highScoreText.getLocalBounds();
    m_highScoreText.setOrigin(highBounds.width, highBounds.top + highBounds.height / 2.0f);
}

void ScoreView::draw(sf::RenderWindow& window) {
    window.draw(m_scoreText);
    window.draw(m_highScoreText);
}
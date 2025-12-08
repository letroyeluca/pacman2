//
// Created by Luca Letroye on 28/11/2025.
//
#include "logic/models/ScoreModel.h"
#include "logic/utils/StopWatch.h"
#include <fstream>
#include <iostream>

namespace logic {

ScoreModel::ScoreModel(double x, double y, double size)
    : EntityModel(x, y, size, size), // Geef x, y door. Width/Height (1.0) maakt
                                     // niet uit voor tekst.
      m_currentScore(0), m_highScore(0), m_lastCoinEatTime(0.0f) {
    loadHighScores();
}

void ScoreModel::onNotify(const Subject& subject, Event event) {
    // Check welk event er gebeurt. De 'subject' parameter vertelt ons WIE het
    // stuurde (bijv. de specifieke Coin), maar dat hebben we voor de score niet
    // per se nodig.

    if (event == Event::CoinEaten) {
        // Score moet afhangen van tijd sinds laatste munt
        float currentTime = Stopwatch::getInstance().getTime(); // Zorg dat getTime() bestaat!
        float timeDiff = currentTime - m_lastCoinEatTime;
        m_lastCoinEatTime = currentTime;

        // Basis punten + bonus voor snelheid (Combo systeem)
        int points = 10;
        if (timeDiff < 0.5f)
            points += 20;
        else if (timeDiff < 1.0f)
            points += 10;

        addPoints(points);
    } else if (event == Event::GhostEaten) {
        addPoints(200); //
    }
}

void ScoreModel::addPoints(int points) {
    m_currentScore += points;
    if (m_currentScore > m_highScore) {
        m_highScore = m_currentScore;
        saveHighScores();
    }
    notify(Event::ScoreChanged);
}

void ScoreModel::loadHighScores() {
    m_highScores.clear();
    std::ifstream file("highscore.txt");
    int score;
    // Lees alle scores in het bestand (max 5 verwacht, maar we lezen alles)
    while (file >> score) {
        m_highScores.push_back(score);
    }
    // Voor de zekerheid sorteren (hoogste eerst)
    std::sort(m_highScores.rbegin(), m_highScores.rend());

    // Zorg dat we er max 5 hebben
    if (m_highScores.size() > 5) {
        m_highScores.resize(5);
    }
}
void ScoreModel::saveScoreIfPersonalBest() {
    // Voeg huidige score toe
    m_highScores.push_back(m_currentScore);

    // Sorteer van groot naar klein
    std::sort(m_highScores.rbegin(), m_highScores.rend());

    // Houd alleen de top 5
    if (m_highScores.size() > 5) {
        m_highScores.resize(5);
    }

    saveHighScores();
}

void ScoreModel::saveHighScores() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        for (int score : m_highScores) {
            file << score << "\n";
        }
    }
}

void ScoreModel::update(float dt) {}

void ScoreModel::accept(Visitor& visitor) {}

} // namespace logic

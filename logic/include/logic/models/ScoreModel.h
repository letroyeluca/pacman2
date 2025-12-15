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

    void onNotify(const Subject& subject, Event event) override;

    int getScore() const { return m_currentScore; }

    const std::vector<int>& getTopScores() const { return m_highScores; }

    int getHighScore() const {
        int savedHigh = m_highScores.empty() ? 0 : m_highScores.front();
        return std::max(savedHigh, m_currentScore);
    }

    // Roep dit aan bij Game Over
    void saveScoreIfPersonalBest();

    void update(float dt) override;
    void accept(Visitor& visitor) override;

private:
    void addPoints(int points);
    void loadHighScores();
    void saveHighScores();

    int m_currentScore;
    int m_highScore;
    std::vector<int> m_highScores;
    float m_lastCoinEatTime;
    float m_timer;
};
} // namespace logic
#endif

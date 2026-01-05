#ifndef PACMAN_SCOREVIEW_H
#define PACMAN_SCOREVIEW_H

#include "logic/models/ScoreModel.h"
#include "views/EntityView.h"
#include <memory>

// UI component: Toont huidige score en highscore
class ScoreView : public EntityView {
public:
    ScoreView(std::shared_ptr<logic::ScoreModel> model, Camera& camera);

    // Update de tekststring wanneer de score verandert in het model
    void onNotify(const logic::Subject& subject, logic::Event event) override;

    void onWindowResize() override;
    // anmaken van scoreview buiten de constructor.
    void init();

    void draw(sf::RenderWindow& window) override;

private:
    std::shared_ptr<logic::ScoreModel> m_scoreModel;
    sf::Font m_font;
    sf::Text m_scoreText;
    sf::Text m_highScoreText;
};

#endif // PACMAN_SCOREVIEW_H
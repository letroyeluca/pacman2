#ifndef PACMAN_MENUSTATE_H
#define PACMAN_MENUSTATE_H

#include "Camera.h"
#include "logic/models/ScoreModel.h"
#include "states/State.h"
#include "states/StateManager.h"

// Het hoofdmenu: startpunt van de applicatie
class MenuState : public State {
public:
    MenuState(StateManager& manager, sf::RenderWindow& window);

    void handleInput(sf::Event& event) override; // Start game bij Enter/Klik
    void update(float dt) override;
    void render() override;                      // Tekent titel en highscores
    void handleResize(sf::Event::SizeEvent size) override;
    void resume();                               // Herlaadt highscores als men terugkeert naar menu

private:
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_playText;
    std::unique_ptr<Camera> m_camera;

    // Leaderboard functionaliteit
    void initLeaderboard();                      // Leest txt-bestand uit voor scores
    sf::Text m_leaderboardTitle;
    std::vector<sf::Text> m_scoreTexts;          // Lijst van teksten voor top-scores
    std::unique_ptr<logic::ScoreModel> m_tempScoreModel; // Tijdelijk model om scores op te halen
};

#endif // PACMAN_MENUSTATE_H
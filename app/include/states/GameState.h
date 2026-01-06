//
// Created by Luca Letroye on 4/11/2025.
//

#ifndef PACMAN_GAMESTATE_H
#define PACMAN_GAMESTATE_H

#pragma once
#include "Camera.h"
#include "State.h"
#include "logic/World.h"
#include "views/EntityView.h"
#include "views/ScoreView.h"
#include <memory>
#include <vector>

// Beheert de actieve gameplay (het spelen zelf)
class GameState : public State {
private:
    std::unique_ptr<Camera> m_camera;                  // Regelt de camerapositie
    std::shared_ptr<logic::AbstractFactory> m_factory; // Factory om entities aan te maken
    std::unique_ptr<logic::World> m_world;             // De logische spelwereld en regels
    std::vector<std::shared_ptr<EntityView>> m_views;  // De visuele weergave van entities

    std::shared_ptr<logic::ScoreModel> m_scoreModel; // Data voor score en levens
    std::shared_ptr<ScoreView> m_scoreView;          // Weergave van de score (UI)
    int m_levelIndex;                                // Huidig levelnummer

public:
    // Constructor: start de game. Parameters maken level-overgangen mogelijk (score behouden)
    GameState(StateManager& manager, sf::RenderWindow& window, int currentScore = 0, int currentLives = 3,
              int currentLevel = 1);
    ~GameState();

    void handleInput(sf::Event& event) override;           // Verwerkt toetsenbordinvoer
    void update(float dt) override;                        // Update spellogica (beweging, botsingen)
    void render() const override;                          // Tekent alles op het scherm
    void handleResize(sf::Event::SizeEvent size) override; // Past beeld aan bij venstergrootte
};
#endif // PACMAN_GAMESTATE_H
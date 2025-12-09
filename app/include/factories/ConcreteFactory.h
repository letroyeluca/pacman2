//
// Created by Luca Letroye on 26/10/2025.
//

//
// Created by Luca Letroye on 26/10/2025.
//

#ifndef PACMAN_CONCRETEFACTORY_H
#define PACMAN_CONCRETEFACTORY_H

#include "logic/patterns/AbstractFactory.hpp"
#include "views/EntityView.h"
#include <memory>
#include <vector>

class Camera;

class ConcreteFactory : public logic::AbstractFactory {
public:
    ConcreteFactory(Camera& camera);

    // Overrides moeten shared_ptr teruggeven
    std::shared_ptr<logic::PacManModel> createPacMan(double x, double y, double w, double h) override;
    std::shared_ptr<logic::GhostModel> createGhost(double x, double y, double w, double h, char type) override;
    std::shared_ptr<logic::CoinModel> createCoin(double x, double y, double w, double h) override;
    std::shared_ptr<logic::WallModel> createWall(double x, double y, double w, double h) override;

    std::shared_ptr<logic::ScoreModel> createScore(double x, double y, double size) override;

    std::vector<std::unique_ptr<EntityView>> getCreatedViews() override { return std::move(m_views); }

private:
    std::vector<std::unique_ptr<EntityView>> m_views;
    sf::Texture m_sharedTexture;
    Camera& m_camera;
};
#endif // PACMAN_CONCRETEFACTORY_H
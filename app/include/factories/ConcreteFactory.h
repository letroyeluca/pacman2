//
// Created by Luca Letroye on 26/10/2025.
//
#ifndef PACMAN_CONCRETEFACTORY_H
#define PACMAN_CONCRETEFACTORY_H

#include "logic/patterns/AbstractFactory.hpp"
#include "views/EntityView.h"
#include <memory>
#include <vector>=

class Camera;

class ConcreteFactory : public logic::AbstractFactory {
public:
    ConcreteFactory(Camera& camera);

    // functies om entities aan te maken waaronder models en views
    std::shared_ptr<logic::PacManModel> createPacMan(const double x, const double y, const double w,
                                                     const double h) override;
    std::shared_ptr<logic::GhostModel> createGhost(const double x, const double y, const double w, const double h,
                                                   char type) override;
    std::shared_ptr<logic::CoinModel> createCoin(const double x, const double y, const double w,
                                                 const double h) override;
    std::shared_ptr<logic::AppleModel> createApple(const double x, const double y, const double w,
                                                   const double h) override;
    std::shared_ptr<logic::WallModel> createWall(const double x, const double y, const double w,
                                                 const double h) override;
    std::shared_ptr<logic::ScoreModel> createScore(const double x, const double y, const double size) override;

    std::vector<std::shared_ptr<EntityView>> getCreatedViews() override { return std::move(m_views); }

private:
    std::vector<std::shared_ptr<EntityView>> m_views;
    sf::Texture m_sharedTexture;
    Camera& m_camera;
};
#endif // PACMAN_CONCRETEFACTORY_H
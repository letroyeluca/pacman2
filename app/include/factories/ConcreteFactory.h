//
// Created by Luca Letroye on 26/10/2025.
//

#ifndef PACMAN_CONCRETEFACTORY_H
#define PACMAN_CONCRETEFACTORY_H

#include "logic/patterns/AbstractFactory.hpp"
#include "views/EntityView.h" // De basis 'View' klasse
#include <memory>
#include <vector>

class EntityView;
class Camera;

class ConcreteFactory : public logic::AbstractFactory {
public:
  // Pas de constructor aan:
  ConcreteFactory(Camera &camera);

  std::unique_ptr<logic::PacManModel> createPacMan(double x, double y, double w,
                                                   double h) override;
  std::unique_ptr<logic::CoinModel> createCoin(double x, double y, double w,
                                               double h) override;
  std::unique_ptr<logic::WallModel> createWall(double x, double y, double w,
                                               double h) override;
  // std::unique_ptr<logic::GhostModel> createGhost(double x, double y, double
  // w, double h) override;

  std::vector<std::unique_ptr<EntityView>> getCreatedViews() override {
    return std::move(m_views);
  }

private:
  std::vector<std::unique_ptr<EntityView>> m_views;
  Camera &m_camera;
};
#endif // PACMAN_CONCRETEFACTORY_H

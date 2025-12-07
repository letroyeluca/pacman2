//
// Created by Luca Letroye on 28/11/2025.
//
// logic/patterns/AbstractFactory.hpp

#ifndef PACMAN_ABSTRACTFACTORY_H
#define PACMAN_ABSTRACTFACTORY_H

#include <memory>
#include <vector>

class EntityView;

namespace logic {
class PacManModel;
class CoinModel;
class WallModel;
class GhostModel;
class ScoreModel;

class AbstractFactory {
public:
  virtual ~AbstractFactory() = default;

  // AANGEPAST: Return types zijn nu shared_ptr voor consistente ownership in
  // World én Views
  virtual std::shared_ptr<PacManModel> createPacMan(double x, double y,
                                                    double w, double h) = 0;
  virtual std::shared_ptr<CoinModel> createCoin(double x, double y, double w,
                                                double h) = 0;
  virtual std::shared_ptr<WallModel> createWall(double x, double y, double w,
                                                double h) = 0;

  // Voeg 'size' toe aan de parameters
  virtual std::shared_ptr<ScoreModel> createScore(double x, double y,
                                                  double size) = 0;

  virtual std::vector<std::unique_ptr<EntityView>> getCreatedViews() = 0;
};
} // namespace logic

#endif // PACMAN_ABSTRACTFACTORY_H
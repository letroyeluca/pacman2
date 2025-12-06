//
// Created by Luca Letroye on 26/11/2025.
//
// logic/World.h

#ifndef PACMAN_WORLD_H
#define PACMAN_WORLD_H

#include "logic/patterns/AbstractFactory.hpp"
#include <memory>
#include <vector>

// Forward declarations
namespace logic {
class PacManModel;
class CoinModel;
class WallModel;
class ScoreModel;
} // namespace logic

namespace logic {

class World {
public:
  World(AbstractFactory *factory);
  ~World();

  void update(float dt);

  // Setters
  void setGridDimensions(double startX, double startY, double tileSize);
  void setWorldDimensions(double width, double height);

  // Getters
  // Je kunt nog steeds raw pointers teruggeven voor simpel gebruik
  PacManModel *getPacMan() { return m_pacman.get(); }

  double getWidth() const { return m_width; }
  double getHeight() const { return m_height; }

  // Add functies
  void addWall(double x, double y, double w, double h);
  void addCoin(double x, double y, double w, double h);
  void addPacMan(double x, double y, double w, double h);
  void createScore(double x, double y, double size);

private:
  void readMap(std::string filename);
  bool isMapPositionFree(double x, double y);

  AbstractFactory *m_factory;
  std::shared_ptr<logic::ScoreModel> m_scoreModel;

  // --- HIER ZIT DE FIX ---
  // Verander unique_ptr naar shared_ptr
  std::shared_ptr<PacManModel> m_pacman;
  std::vector<std::shared_ptr<CoinModel>> m_coins;
  std::vector<std::shared_ptr<WallModel>> m_walls;

  double m_width = 2.0;
  double m_height = 2.0;
  double m_tileSize = 0.1;
  double m_startX = -1.0;
  double m_startY = -1.0;
};
} // namespace logic
#endif // PACMAN_WORLD_H
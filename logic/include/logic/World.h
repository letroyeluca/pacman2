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
class AppleModel;
} // namespace logic

namespace logic {

class World {
public:
    World(AbstractFactory* factory);
    ~World();

    void update(float dt);

    // Setters
    void setGridDimensions(double startX, double startY, double tileSize);
    void setWorldDimensions(double width, double height);

    // Getters
    // Je kunt nog steeds raw pointers teruggeven voor simpel gebruik
    PacManModel* getPacMan() { return m_pacman.get(); }

    double getWidth() const { return m_width; }
    double getHeight() const { return m_height; }

    double getTileSize() const { return m_tileSize; }
    double getStartX() const { return m_startX; }
    double getStartY() const { return m_startY; }
    std::vector<std::shared_ptr<GhostModel>>& getGhosts() { return m_ghosts; }

    // Add functies
    void addWall(double x, double y, double w, double h);
    void addCoin(double x, double y, double w, double h);
    void addApple(double x, double y, double w, double h);
    void addPacMan(double x, double y, double w, double h, int mapsize);
    void addGhost(double x, double y, double w, double h, char type);
    void createScore(double x, double y, double size);
    void addGate(double x, double y, double w, double h);
    bool isGateAt(double x, double y) const;
    void resetGhosts();
    void activateFrightenedMode();

    std::shared_ptr<ScoreModel> getScoreModel() { return m_scoreModel; }
    bool isMapPositionFree(double x, double y);

private:
    void readMap(std::string filename);

    AbstractFactory* m_factory;
    std::shared_ptr<logic::ScoreModel> m_scoreModel;

    std::shared_ptr<PacManModel> m_pacman;
    std::vector<std::shared_ptr<CoinModel>> m_coins;
    std::vector<std::shared_ptr<AppleModel>> m_apples;
    std::vector<std::shared_ptr<WallModel>> m_walls;
    std::vector<std::shared_ptr<GhostModel>> m_ghosts;

    struct Gate {
        double x;
        double y;
        double w;
        double h;
    };
    std::vector<Gate> m_gates;

    double m_width = 2.0;
    double m_height = 2.0;
    double m_tileSize = 0.1;
    double m_startX = -1.0;
    double m_startY = -1.0;
};
} // namespace logic
#endif // PACMAN_WORLD_H
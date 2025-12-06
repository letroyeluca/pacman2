//
// Created by Luca Letroye on 26/11/2025.
//

#ifndef PACMAN_WORLD_H
#define PACMAN_WORLD_H

#include <memory>
#include <vector>
#include <string>
#include "logic/patterns/AbstractFactory.hpp"

namespace logic {
    class PacManModel;
    class CoinModel;
    class WallModel;
}

namespace logic {

    class World {
    public:
        World(AbstractFactory* factory);
        ~World();
        void update(float dt);
        PacManModel* getPacMan() { return m_pacman.get(); }

        double getWidth() const { return m_width; }
        double getHeight() const { return m_height; }

        void addWall(double x, double y, double w, double h);
        void addCoin(double x, double y, double w, double h);
        void addPacMan(double x, double y, double w, double h);

        void setGridDimensions(double startX, double startY, double tileSize);

    private:
        void readMap(std::string filename);
        bool isMapPositionFree(double x, double y);

        AbstractFactory* m_factory;
        std::unique_ptr<PacManModel> m_pacman;
        std::vector<std::unique_ptr<CoinModel>> m_coins;
        std::vector<std::unique_ptr<WallModel>> m_walls;

        double m_width = 2.0;
        double m_height = 2.0;
        double m_tileSize = 0.1;
        double m_startX = -1.0;
        double m_startY = -1.0;
    };
}



#endif //PACMAN_WORLD_H

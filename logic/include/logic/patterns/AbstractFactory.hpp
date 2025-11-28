//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_ABSTRACTFACTORY_H
#define PACMAN_ABSTRACTFACTORY_H

#include <memory>

class EntityView;

namespace logic{
    class PacManModel;
    class CoinModel;
    class WallModel;
    class GhostModel;
    class ScoreModel;

    class AbstractFactory{
        virtual ~AbstractFactory() = default;

        virtual std::unique_ptr<PacManModel> createPacMan(double x, double y, double w, double h) = 0;
        virtual std::unique_ptr<CoinModel> createCoin(double x, double y, double w, double h) = 0;
        virtual std::unique_ptr<WallModel> createWall(double x, double y, double w, double h) = 0;
        virtual std::unique_ptr<GhostModel> createGhost(double x, double y, double w, double h) = 0;
        virtual std::unique_ptr<ScoreModel> createScore(double x, double y, double w, double h) = 0;
        virtual std::unique_ptr<std::unique_ptr<EntityView>> getCreatedViews() = 0;


    };
}

#endif //PACMAN_ABSTRACTFACTORY_H

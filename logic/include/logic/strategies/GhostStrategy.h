#ifndef PACMAN_GHOSTSTRATEGY_H
#define PACMAN_GHOSTSTRATEGY_H

#include "logic/World.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

namespace logic {

class GhostStrategy {
public:
    virtual ~GhostStrategy() = default;
    virtual Direction calculateNextMove(GhostModel& ghost, World& world) = 0;
};

class RandomStrategy : public GhostStrategy {
public:
    Direction calculateNextMove(GhostModel& ghost, World& world) override {
        // 1. Basis Info
        Direction currentDir = ghost.getDirection();
        Direction oppositeDir = Direction::STOP;

        if (currentDir == Direction::UP)
            oppositeDir = Direction::DOWN;
        if (currentDir == Direction::DOWN)
            oppositeDir = Direction::UP;
        if (currentDir == Direction::LEFT)
            oppositeDir = Direction::RIGHT;
        if (currentDir == Direction::RIGHT)
            oppositeDir = Direction::LEFT;

        // 2. Omgeving Scannen
        double gx = ghost.getX();
        double gy = ghost.getY();
        double ts = world.getTileSize();

        // Check fysieke muren
        bool freeUP = world.isMapPositionFree(gx, gy - ts);
        bool freeDOWN = world.isMapPositionFree(gx, gy + ts); // <-- Dit passen we zo aan
        bool freeLEFT = world.isMapPositionFree(gx - ts, gy);
        bool freeRIGHT = world.isMapPositionFree(gx + ts, gy);

        // --- GATE LOGICA (HET BELANGRIJKE STUK) ---

        // Check A: Staan we OP dit moment op een gate? (Situatie: we lopen naar buiten)
        // Zo ja, dan mogen we niet terug omlaag.
        if (world.isGateAt(gx, gy)) {
            freeDOWN = false;
            freeLEFT = false;
            freeRIGHT = false;
        }

        // Check B: Is de tegel ONDER ons een gate? (Situatie: we zijn buiten en willen naar beneden)
        // Zo ja, dan zien we dat als een muur. We stappen er NIET op.
        if (world.isGateAt(gx, gy + ts)) {
            freeDOWN = false;
        }

        // ------------------------------------------

        // 3. Opties verzamelen (Nu is freeDOWN 'false' als er een gate is)
        std::vector<Direction> options;
        if (freeUP)
            options.push_back(Direction::UP);
        if (freeDOWN)
            options.push_back(Direction::DOWN);
        if (freeLEFT)
            options.push_back(Direction::LEFT);
        if (freeRIGHT)
            options.push_back(Direction::RIGHT);

        // 4. Verwijder 'terug' (oppositeDir)
        if (options.size() > 1 && oppositeDir != Direction::STOP) {
            auto it = std::find(options.begin(), options.end(), oppositeDir);
            if (it != options.end()) {
                options.erase(it);
            }
        }

        // 5. Kies willekeurig
        if (!options.empty()) {
            return options[rand() % options.size()];
        }

        // Noodgeval: altijd omhoog proberen
        if (freeUP)
            return Direction::UP;

        return Direction::STOP;
    }
};

} // namespace logic

#endif // PACMAN_GHOSTSTRATEGY_H
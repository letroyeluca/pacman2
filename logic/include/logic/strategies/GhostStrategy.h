#ifndef PACMAN_GHOSTSTRATEGY_H
#define PACMAN_GHOSTSTRATEGY_H

#include "logic/World.h"
#include "logic/models/GhostModel.h"
#include "logic/models/PacManModel.h"
#include "logic/utils/Random.h"
#include <iostream>
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

// ------------------------------------------------------------------
// FRIGHTENED STRATEGY (Vlucht weg van Pac-Man)
// ------------------------------------------------------------------
    class FrightenedStrategy : public GhostStrategy {
    public:
        Direction calculateNextMove(GhostModel& ghost, World& world) override {
            std::cout << "GHOST [Type " << ghost.getType() << "] gebruikt FRIGHTENED strategie" << std::endl;
            // 1. Check of Pac-Man bestaat
            auto pacman = world.getPacMan();
            if (!pacman)
                return Direction::STOP;

            // ----------------------------------------------------------
            // STAP A: Bepaal het DOEL (Target is Pac-Man, om van weg te rennen)
            // ----------------------------------------------------------
            double targetX = pacman->getX();
            double targetY = pacman->getY();

            // ----------------------------------------------------------
            // STAP B: Verzamel geldige opties (Boilerplate)
            // ----------------------------------------------------------
            Direction currentDir = ghost.getDirection();
            Direction oppositeDir = Direction::STOP;
            if (currentDir == Direction::UP) oppositeDir = Direction::DOWN;
            if (currentDir == Direction::DOWN) oppositeDir = Direction::UP;
            if (currentDir == Direction::LEFT) oppositeDir = Direction::RIGHT;
            if (currentDir == Direction::RIGHT) oppositeDir = Direction::LEFT;

            double gx = ghost.getX();
            double gy = ghost.getY();
            double ts = world.getTileSize();

            // Check muren
            bool freeUP = world.isMapPositionFree(gx, gy - ts);
            bool freeDOWN = world.isMapPositionFree(gx, gy + ts);
            bool freeLEFT = world.isMapPositionFree(gx - ts, gy);
            bool freeRIGHT = world.isMapPositionFree(gx + ts, gy);

            // Check Gates (Spook mag meestal niet terug het huis in vluchten)
            if (world.isGateAt(gx, gy)) freeDOWN = false;
            if (world.isGateAt(gx, gy + ts)) freeDOWN = false;

            std::vector<Direction> options;
            if (freeUP) options.push_back(Direction::UP);
            if (freeDOWN) options.push_back(Direction::DOWN);
            if (freeLEFT) options.push_back(Direction::LEFT);
            if (freeRIGHT) options.push_back(Direction::RIGHT);

            // Verwijder 'terug' optie (tenzij doodlopend)
            if (options.size() > 1 && oppositeDir != Direction::STOP) {
                auto it = std::find(options.begin(), options.end(), oppositeDir);
                if (it != options.end())
                    options.erase(it);
            }

            if (options.empty())
                return Direction::STOP;

            // ----------------------------------------------------------
            // STAP C: Bereken Manhattan Distance & MAXIMALISEER
            // ----------------------------------------------------------

            // We beginnen met een zo klein mogelijke waarde (-1.0 is veilig omdat afstand altijd >= 0 is)
            double maxDistance = -1.0;
            std::vector<Direction> bestMoves;

            for (Direction dir : options) {
                double nextX = gx;
                double nextY = gy;

                if (dir == Direction::UP) nextY -= ts;
                else if (dir == Direction::DOWN) nextY += ts;
                else if (dir == Direction::LEFT) nextX -= ts;
                else if (dir == Direction::RIGHT) nextX += ts;

                // Formule: |x1 - x2| + |y1 - y2|
                double dist = std::abs(nextX - targetX) + std::abs(nextY - targetY);

                // LOGICA OMKERING: We willen de GROOTSTE afstand
                if (dist > maxDistance) {
                    maxDistance = dist;
                    bestMoves.clear();
                    bestMoves.push_back(dir);
                } else if (std::abs(dist - maxDistance) < 0.001) {
                    // Tie (gelijke afstand)
                    bestMoves.push_back(dir);
                }
            }

            // ----------------------------------------------------------
            // STAP D: Tie-breaking (Random)
            // ----------------------------------------------------------
            if (bestMoves.empty())
                return Direction::STOP;

            // Kies willekeurig uit de opties die het verst weg leiden
            double randVal = Random::getInstance().generate(0.0, static_cast<double>(bestMoves.size()));
            int index = static_cast<int>(randVal);
            if (index >= bestMoves.size())
                index = static_cast<int>(bestMoves.size()) - 1;

            return bestMoves[index];
        }
    };


// ------------------------------------------------------------------
// DIRECT CHASE STRATEGY (Jaagt direct op Pac-Man)
// ------------------------------------------------------------------
class DirectChaseStrategy : public GhostStrategy {
public:
    Direction calculateNextMove(GhostModel& ghost, World& world) override {
        std::cout << "GHOST [Type " << ghost.getType() << "] gebruikt CHASE strategie" << std::endl;
        // 1. Check of Pac-Man bestaat
        auto pacman = world.getPacMan();
        if (!pacman)
            return Direction::STOP;

        // ----------------------------------------------------------
        // STAP A: Bepaal het DOEL (Target)
        // "minimize the Manhattan distance to its location"
        // ----------------------------------------------------------
        double targetX = pacman->getX();
        double targetY = pacman->getY();

        // ----------------------------------------------------------
        // STAP B: Verzamel geldige opties (Standaard boilerplate)
        // ----------------------------------------------------------
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

        double gx = ghost.getX();
        double gy = ghost.getY();
        double ts = world.getTileSize();

        bool freeUP = world.isMapPositionFree(gx, gy - ts);
        bool freeDOWN = world.isMapPositionFree(gx, gy + ts);
        bool freeLEFT = world.isMapPositionFree(gx - ts, gy);
        bool freeRIGHT = world.isMapPositionFree(gx + ts, gy);

        // Check Gates
        if (world.isGateAt(gx, gy))
            freeDOWN = false;
        if (world.isGateAt(gx, gy + ts))
            freeDOWN = false;

        std::vector<Direction> options;
        if (freeUP)
            options.push_back(Direction::UP);
        if (freeDOWN)
            options.push_back(Direction::DOWN);
        if (freeLEFT)
            options.push_back(Direction::LEFT);
        if (freeRIGHT)
            options.push_back(Direction::RIGHT);

        if (options.size() > 1 && oppositeDir != Direction::STOP) {
            auto it = std::find(options.begin(), options.end(), oppositeDir);
            if (it != options.end())
                options.erase(it);
        }

        if (options.empty())
            return Direction::STOP;

        // ----------------------------------------------------------
        // STAP C: Bereken Manhattan Distance
        // ----------------------------------------------------------
        double minDistance = std::numeric_limits<double>::max();
        std::vector<Direction> bestMoves;

        for (Direction dir : options) {
            double nextX = gx;
            double nextY = gy;

            if (dir == Direction::UP)
                nextY -= ts;
            else if (dir == Direction::DOWN)
                nextY += ts;
            else if (dir == Direction::LEFT)
                nextX -= ts;
            else if (dir == Direction::RIGHT)
                nextX += ts;

            // Formule: |x1 - x2| + |y1 - y2|
            double dist = std::abs(nextX - targetX) + std::abs(nextY - targetY);

            if (dist < minDistance) {
                minDistance = dist;
                bestMoves.clear();
                bestMoves.push_back(dir);
            } else if (std::abs(dist - minDistance) < 0.001) {
                bestMoves.push_back(dir);
            }
        }

        // ----------------------------------------------------------
        // STAP D: Tie-breaking
        // ----------------------------------------------------------
        if (bestMoves.empty())
            return Direction::STOP;

        // Kies willekeurig uit de beste opties
        double randVal = Random::getInstance().generate(0.0, static_cast<double>(bestMoves.size()));
        int index = static_cast<int>(randVal);
        if (index >= bestMoves.size())
            index = static_cast<int>(bestMoves.size()) - 1;

        return bestMoves[index];
    }
};

// ------------------------------------------------------------------
// FRONT CHASE STRATEGY (Jaagt op de tegel VOOR Pac-Man)
// ------------------------------------------------------------------
class FrontChaseStrategy : public GhostStrategy {
public:
    Direction calculateNextMove(GhostModel& ghost, World& world) override {
        // 1. Check of Pac-Man er is (safety check)
        auto pacman = world.getPacMan();
        if (!pacman)
            return Direction::STOP;

        // ----------------------------------------------------------
        // STAP A: Bepaal het DOEL (Target)
        // "Locations 'in front of' Pac-Man"
        // ----------------------------------------------------------
        double targetX = pacman->getX();
        double targetY = pacman->getY();
        Direction pDir = pacman->getDirection();

        // We richten op 2 tot 4 tegels vóór Pac-Man.
        // Laten we 3 tegels nemen als "in front of".
        double offset = world.getTileSize() * 3.0;

        switch (pDir) {
        case Direction::UP:
            targetY -= offset;
            break;
        case Direction::DOWN:
            targetY += offset;
            break;
        case Direction::LEFT:
            targetX -= offset;
            break;
        case Direction::RIGHT:
            targetX += offset;
            break;
        default:
            break; // Bij STOP blijven we op Pac-Man zelf richten
        }

        // ----------------------------------------------------------
        // STAP B: Verzamel geldige opties (Boilerplate)
        // ----------------------------------------------------------
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

        double gx = ghost.getX();
        double gy = ghost.getY();
        double ts = world.getTileSize();

        // Check muren
        bool freeUP = world.isMapPositionFree(gx, gy - ts);
        bool freeDOWN = world.isMapPositionFree(gx, gy + ts);
        bool freeLEFT = world.isMapPositionFree(gx - ts, gy);
        bool freeRIGHT = world.isMapPositionFree(gx + ts, gy);

        // Check Gates (Niet terug het huis in)
        if (world.isGateAt(gx, gy))
            freeDOWN = false;
        if (world.isGateAt(gx, gy + ts))
            freeDOWN = false;

        std::vector<Direction> options;
        if (freeUP)
            options.push_back(Direction::UP);
        if (freeDOWN)
            options.push_back(Direction::DOWN);
        if (freeLEFT)
            options.push_back(Direction::LEFT);
        if (freeRIGHT)
            options.push_back(Direction::RIGHT);

        // Verwijder 'terug' (behalve bij doodlopend)
        if (options.size() > 1 && oppositeDir != Direction::STOP) {
            auto it = std::find(options.begin(), options.end(), oppositeDir);
            if (it != options.end())
                options.erase(it);
        }

        if (options.empty())
            return Direction::STOP;

        // ----------------------------------------------------------
        // STAP C: Bereken Manhattan Distance voor elke optie
        // ----------------------------------------------------------

        double minDistance = std::numeric_limits<double>::max();
        std::vector<Direction> bestMoves;

        for (Direction dir : options) {
            // "Compute for each viable action... if the ghost had taken one step"
            double nextX = gx;
            double nextY = gy;

            if (dir == Direction::UP)
                nextY -= ts;
            else if (dir == Direction::DOWN)
                nextY += ts;
            else if (dir == Direction::LEFT)
                nextX -= ts;
            else if (dir == Direction::RIGHT)
                nextX += ts;

            // Manhattan Distance Formule: |x1 - x2| + |y1 - y2|
            double dist = std::abs(nextX - targetX) + std::abs(nextY - targetY);

            // Minimaliseren
            if (dist < minDistance) {
                // Nieuw record gevonden! Vergeet de vorige opties.
                minDistance = dist;
                bestMoves.clear();
                bestMoves.push_back(dir);
            } else if (std::abs(dist - minDistance) < 0.001) {
                // Gelijkspel (Tie)? Voeg toe aan de lijst.
                bestMoves.push_back(dir);
            }
        }

        // ----------------------------------------------------------
        // STAP D: Kies uit beste opties (Tie-breaking)
        // ----------------------------------------------------------
        if (bestMoves.empty())
            return Direction::STOP; // Should not happen

        // "Ties between the best actions are broken at random"
        double randVal = Random::getInstance().generate(0.0, static_cast<double>(bestMoves.size()));
        int index = static_cast<int>(randVal);
        if (index >= bestMoves.size())
            index = static_cast<int>(bestMoves.size()) - 1;

        return bestMoves[index];
    }
};

// ------------------------------------------------------------------
// LOCKED STRATEGY (Voor 1 spook, volgens de PDF regels)
// ------------------------------------------------------------------
class LockedStrategy : public GhostStrategy {
public:
    Direction calculateNextMove(GhostModel& ghost, World& world) override {
        // 1. Huidige status ophalen
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

        // 2. Omgeving scannen
        double gx = ghost.getX();
        double gy = ghost.getY();
        double ts = world.getTileSize();

        // Check fysieke muren
        bool freeUP = world.isMapPositionFree(gx, gy - ts);
        bool freeDOWN = world.isMapPositionFree(gx, gy + ts);
        bool freeLEFT = world.isMapPositionFree(gx - ts, gy);
        bool freeRIGHT = world.isMapPositionFree(gx + ts, gy);

        // --- GATE LOGICA ---
        if (world.isGateAt(gx, gy)) {
            freeDOWN = false;
        } // We staan op een gate
        if (world.isGateAt(gx, gy + ts)) {
            freeDOWN = false;
        } // Tegel onder ons is een gate

        // 3. Opties verzamelen
        std::vector<Direction> options;
        if (freeUP)
            options.push_back(Direction::UP);
        if (freeDOWN)
            options.push_back(Direction::DOWN);
        if (freeLEFT)
            options.push_back(Direction::LEFT);
        if (freeRIGHT)
            options.push_back(Direction::RIGHT);

        // 4. Verwijder 'terug' (oppositeDir), tenzij het de enige optie is
        if (options.size() > 1 && oppositeDir != Direction::STOP) {
            auto it = std::find(options.begin(), options.end(), oppositeDir);
            if (it != options.end()) {
                options.erase(it);
            }
        }

        if (options.empty())
            return Direction::STOP;
        if (options.size() == 1)
            return options[0]; // Geen keuze (bocht of gang)

        // Check of we rechtdoor kunnen (Locked blijven)
        bool canKeepGoing = false;
        for (const auto& dir : options) {
            if (dir == currentDir) {
                canKeepGoing = true;
                break;
            }
        }

        bool shouldSwitch = false;

        if (!canKeepGoing) {
            // We hebben een hoek bereikt (muur voor ons), dus we MOETEN switchen.
            shouldSwitch = true;
        } else {
            // We zijn op een kruispunt en KUNNEN rechtdoor.
            // Regel: "With probability p=0.5, the ghost will lock to a random direction"

            // Gebruik jouw Random class: genereer double tussen 0.0 en 1.0
            double p = Random::getInstance().generate(0.0, 1.0);

            if (p <= 0.5) {
                shouldSwitch = true;
            } else {
                shouldSwitch = false; // Blijf Locked
            }
        }

        // 5. Resultaat
        if (!shouldSwitch && canKeepGoing) {
            return currentDir;
        } else {
            // Kies een willekeurige NIEUWE richting
            // Genereer double tussen 0 en size (exclusief size), en cast naar int
            double randVal = Random::getInstance().generate(0.0, static_cast<double>(options.size()));
            int index = static_cast<int>(randVal);

            // Veiligheidscheck (zou niet nodig moeten zijn met correcte logica, maar veiligheid voorop)
            if (index >= options.size())
                index = static_cast<int>(options.size()) - 1;

            return options[index];
        }
    }
};

// ------------------------------------------------------------------
// RANDOM STRATEGY (Fallback, ook aangepast aan jouw Random class)
// ------------------------------------------------------------------
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
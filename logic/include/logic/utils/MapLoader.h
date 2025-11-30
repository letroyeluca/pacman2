//
// Created by Luca Letroye on 29/11/2025.
//

#ifndef PACMAN_MAPLOADER_H
#define PACMAN_MAPLOADER_H

#include <string>
#include "logic/World.h"

namespace logic{
    class MapLoader{
    public:
        MapLoader(World& world) : m_world(world) {};
        virtual ~MapLoader() = default;
        virtual void loadMap(std::string filename) = 0;
    protected:
        World& m_world;
    };
}

#endif //PACMAN_MAPLOADER_H

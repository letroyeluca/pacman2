//
// Created by Luca Letroye on 29/11/2025.
//

#ifndef PACMAN_TXTMAPLOADER_H
#define PACMAN_TXTMAPLOADER_H

#include "logic/utils/MapLoader.h"

namespace logic {
    class txtMapLoader : public MapLoader {
    public:

        txtMapLoader(World& world) : MapLoader(world) {}
        void loadMap(std::string filename) override;
    };
}

#endif //PACMAN_TXTMAPLOADER_H

//
// Created by Luca Letroye on 29/11/2025.
//

#ifndef PACMAN_TXTMAPLOADER_H
#define PACMAN_TXTMAPLOADER_H

#include "logic/utils/MapLoader.h"

namespace logic {
class TxtMapLoader : public MapLoader {
public:
    TxtMapLoader(World& world) : MapLoader(world) {}
    void loadMap(std::string filename) override;
};
} // namespace logic

#endif // PACMAN_TXTMAPLOADER_H

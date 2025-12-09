//
// Created by Luca Letroye on 9/12/2025.
//

#ifndef PACMAN_ASSETMANAGER_H
#define PACMAN_ASSETMANAGER_H

#include <SFML/Graphics.hpp>

namespace Asset {
class AssetManager {
private:
    AssetManager();
    sf::Texture m_texture;

public:
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    static AssetManager& getInstance();
    sf::Sprite getSpriteAtIndex(int x, int y, int size);
    const sf::Texture& getTexture() const;
    static std::vector<sf::IntRect> getAnimationFrames(int x, int y, int count = 3, int size = 50);
};
} // namespace Asset

#endif // PACMAN_ASSETMANAGER_H

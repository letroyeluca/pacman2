//
// Created by Luca Letroye on 9/12/2025.
//
#include "AssetManager.h"
namespace Asset {

// singleton enforce
AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager() {
    if (!m_texture.loadFromFile("assets/sprite.png")) {
        throw std::runtime_error("Failed to load assets/sprite.png");
    }
    m_texture.setSmooth(true);
}

// single sprite return
sf::Sprite AssetManager::getSpriteAtIndex(int x, int y, int size) {
    sf::Sprite sprite(m_texture);
    sprite.setTextureRect(sf::IntRect({(x * 50) + (-6), (y * 50) + (-4)}, {size, size}));
    return sprite;
}

// de volledige texture returne
const sf::Texture& AssetManager::getTexture() const { return m_texture; }

// meerdere intrects returne voor makkelijke integratie met animation sprites
std::vector<sf::IntRect> AssetManager::getAnimationFrames(int x, int y, int count, int size) {
    std::vector<sf::IntRect> frames;
    frames.reserve(count);

    for (int i = 0; i < count; i++) {
        int left = (x * 50) + (-6);
        int top = ((y + i) * 50) + (-4);

        frames.emplace_back(left, top, size, size);
    }
    return frames;
}

} // namespace Asset
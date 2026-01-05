#ifndef PACMAN_ASSETMANAGER_H
#define PACMAN_ASSETMANAGER_H

#include <SFML/Graphics.hpp>

namespace Asset {

// Singleton: Beheert het laden en uitdelen van textures (spritesheet)
    class AssetManager {
    private:
        AssetManager(); // Private constructor
        sf::Texture m_texture;

    public:
        // Kopieer-beveiliging (Singleton regels)
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        static AssetManager& getInstance(); // Geeft de enige instantie terug

        // Haalt een specifieke sprite uit de grote texture sheet
        sf::Sprite getSpriteAtIndex(int x, int y, int size);

        const sf::Texture& getTexture() const;

        // Haalt een reeks frames op voor animaties
        static std::vector<sf::IntRect> getAnimationFrames(int x, int y, int count = 3, int size = 50);
    };
} // namespace Asset

#endif // PACMAN_ASSETMANAGER_H
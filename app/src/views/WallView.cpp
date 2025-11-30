//
// Created by Luca Letroye on 17/11/2025.
//
#include "views/WallView.h"
#include "logic/models/WallModel.h"
#include "Camera.h"
#include <stdexcept>

WallView::WallView(logic::WallModel& model, Camera& camera)
        : EntityView(model, camera)
{
    // Maak een 1x1 witte pixel-textuur in het geheugen
    // Dit is de correcte manier om een 'sf::Shape' na te bootsen
    // terwijl we de EntityView (sf::Sprite) interface behouden.
    sf::Image image;
    image.create(1, 1, sf::Color::White);
    if (!m_texture.loadFromImage(image)) {
        throw std::runtime_error("Failed to create 1x1 texture for wall");
    }
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 1, 1));

    // Tint deze witte pixel blauw (zoals de muren in de PDF [cite: 21-22])
    m_sprite.setColor(sf::Color::Blue);

    // --- Schaal en positioneer de sprite ---
    // spriteBounds is nu {0, 0, 1, 1}
    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();

    // Vraag de logische grootte (bv. 0.05)
    double logicW = m_model.getWidth();
    double logicH = m_model.getHeight();

    // De Camera berekent de pixelgrootte (bv. 0.05 * 300 = 15 pixels)
    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);

    // Schaal de 1x1 sprite op naar de juiste pixelgrootte (bv. 15x15)
    float scaleX = pixelSize.x / spriteBounds.width;
    float scaleY = pixelSize.y / spriteBounds.height;
    m_sprite.setScale(scaleX, scaleY);

    // Zet de origin naar het midden van de 1x1 sprite
    m_sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);

    // Synchroniseer de startpositie
    onNotify(model, logic::Event::DEFAULT);
}
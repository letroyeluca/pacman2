//
// Created by Luca Letroye on 17/11/2025.
//
#include "views/CoinView.h"
#include "logic/models/CoinModel.h"
#include "Camera.h"
#include <stdexcept>
#include <iostream>
CoinView::CoinView(logic::CoinModel& model, Camera& camera)
        : EntityView(model, camera), m_isHidden(false)
{
    // We gebruiken dezelfde spritesheet
    if (!m_texture.loadFromFile("/Users/lucaletroye/Documents/PROJECTEN/pacman/assets/sprite.png")) {
        throw std::runtime_error("Failed to load assets/sprite.png");
    }
    m_sprite.setTexture(m_texture);

    // TODO: Stel de juiste IntRect in voor de MUNT
    // (Vervang 0,0,16,16 door de juiste coördinaten)
    sf::IntRect coinRect(411, 313, 16, 16);
    m_sprite.setTextureRect(coinRect);
    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();

    // Schaal de sprite op basis van de Model-grootte
    double logicW = m_model.getWidth();
    double logicH = m_model.getHeight();
    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);
    float scaleX = pixelSize.x / spriteBounds.width;
    float scaleY = pixelSize.y / spriteBounds.height;
    m_sprite.setScale(scaleX, scaleY);

    // Zet de origin naar het midden
    m_sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
    m_sprite.scale(0.3f, 0.3f);

    // Synchroniseer de startpositie
    onNotify(model, logic::Event::DEFAULT);
}

void CoinView::onNotify(const logic::Subject& subject, logic::Event event) {
    // Controleer of de munt is opgegeten
    if (!m_model.isActive()) {
        m_isHidden = true;
    }

    // Roep de basisklasse aan om de positie bij te werken
    // (Dit is nodig voor de allereerste positie-instelling)
    EntityView::onNotify(subject, event);
}

// We overschrijven 'draw' zodat we stoppen met tekenen
// als de munt is opgegeten.
void CoinView::draw(sf::RenderWindow& window) {
    if (!m_isHidden) {
        window.draw(m_sprite);
    }
}

void CoinView::onWindowResize() {

    EntityView::onWindowResize();
    m_sprite.scale(0.3f, 0.3f);
}
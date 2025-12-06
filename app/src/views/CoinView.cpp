//
// Created by Luca Letroye on 17/11/2025.
//
#include "views/CoinView.h"
#include "Camera.h"
#include "logic/models/CoinModel.h"
#include <stdexcept>

CoinView::CoinView(std::shared_ptr<logic::CoinModel> model, Camera &camera)
        : EntityView(model, camera), m_isHidden(false) {

    if (!m_texture.loadFromFile("assets/sprite.png")) {
        throw std::runtime_error("Failed to load assets/sprite.png");
    }
    m_sprite.setTexture(m_texture);
    sf::IntRect coinRect(411, 313, 16, 16); // Voorbeeld coords
    m_sprite.setTextureRect(coinRect);

    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    double logicW = m_model->getWidth();   // ->
    double logicH = m_model->getHeight();  // ->

    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);
    float scaleX = pixelSize.x / spriteBounds.width;
    float scaleY = pixelSize.y / spriteBounds.height;
    m_sprite.setScale(scaleX, scaleY);
    m_sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
    m_sprite.scale(0.3f, 0.3f);

    EntityView::onNotify(*model, logic::Event::DEFAULT);
}

void CoinView::onNotify(const logic::Subject &subject, logic::Event event) {
    // Casten om isActive te checken, of gebruik base logic als EntityModel isActive heeft
    auto coinModel = std::dynamic_pointer_cast<logic::CoinModel>(m_model);

    if (coinModel && !coinModel->isActive()) {
        m_isHidden = true;
    }
    EntityView::onNotify(subject, event);
}

void CoinView::draw(sf::RenderWindow &window) {
    if (!m_isHidden) {
        window.draw(m_sprite);
    }
}

void CoinView::onWindowResize() {
    EntityView::onWindowResize();
    m_sprite.scale(0.3f, 0.3f);
}
//
// Created by Luca Letroye on 17/11/2025.
//
#include "views/WallView.h"
#include "Camera.h"
#include "logic/models/WallModel.h"
#include <stdexcept>

WallView::WallView(std::shared_ptr<logic::WallModel> model, Camera& camera, sf::Texture& texture)
    : EntityView(model, camera) {

    sf::Image image;
    image.create(1, 1, sf::Color::White);
    m_sprite.setTexture(texture);
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 1, 1));
    m_sprite.setColor(sf::Color::Blue);

    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    double logicW = m_model->getWidth();  // ->
    double logicH = m_model->getHeight(); // ->

    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);
    float scaleX = pixelSize.x / spriteBounds.width;
    float scaleY = pixelSize.y / spriteBounds.height;
    m_sprite.setScale(scaleX, scaleY);
    m_sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);

    EntityView::onNotify(*model, logic::Event::DEFAULT);
}
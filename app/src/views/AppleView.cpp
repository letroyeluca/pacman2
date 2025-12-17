//
// Created by Luca Letroye on 15/12/2025.
//
#include "views/AppleView.h"
#include "AssetManager.h"
#include "Camera.h"
#include "logic/models/AppleModel.h"
#include <stdexcept>

AppleView::AppleView(std::shared_ptr<logic::AppleModel> model, Camera& camera, sf::Texture& texture)
    : EntityView(model, camera), m_isHidden(false) {
    auto& assets = Asset::AssetManager::getInstance();
    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(assets.getAnimationFrames(12, 3, 1)[0]); // Row 0
    m_sprite.setOrigin(25.0f, 25.0f);
    m_sprite.scale(0.8f, 0.8f);
    EntityView::onNotify(*model, logic::Event::DEFAULT);
}

void AppleView::onNotify(const logic::Subject& subject, logic::Event event) {
    // Casten om isActive te checken, of gebruik base logic als EntityModel
    // isActive heeft
    auto coinModel = std::dynamic_pointer_cast<logic::AppleModel>(m_model);

    if (coinModel && !coinModel->isActive()) {
        m_isHidden = true;
    }
    EntityView::onNotify(subject, event);
}

void AppleView::draw(sf::RenderWindow& window) {
    if (!m_isHidden) {
        window.draw(m_sprite);
    }
}

void AppleView::onWindowResize() {
    EntityView::onWindowResize();
    m_sprite.scale(0.8f, 0.8f);
}
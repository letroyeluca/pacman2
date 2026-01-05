//
// Created by Luca Letroye on 9/11/2025.
//
#include "views/EntityView.h"
#include "Camera.h"
#include "logic/patterns/Subject.h"

EntityView::EntityView(std::shared_ptr<logic::EntityModel> model, Camera& camera) : m_model(model), m_camera(camera) {}

void EntityView::onNotify(const logic::Subject& subject, logic::Event event) {
    // Check pointer validiteit
    if (m_model) {
        // Gebruik pijl-operator -> voor pointers
        m_sprite.setPosition(m_camera.project(m_model->getX(), m_model->getY()));
    }
}

void EntityView::init() { m_model->attach(shared_from_this()); }

void EntityView::draw(sf::RenderWindow& window) { window.draw(m_sprite); }

void EntityView::updateAnimation(float dt) {
    // Standaard doet dit niets, maar moet bestaan
}

void EntityView::onWindowResize() {
    // Veiligheidscheck: ScoreView mag dit niet uitvoeren op een null model
    if (!m_model)
        return;

    m_sprite.setPosition(m_camera.project(m_model->getX(), m_model->getY()));

    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    double logicW = m_model->getWidth();  // -> operator
    double logicH = m_model->getHeight(); // -> operator

    sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);

    if (spriteBounds.width > 0 && spriteBounds.height > 0) {
        float scaleX = pixelSize.x / spriteBounds.width;
        float scaleY = pixelSize.y / spriteBounds.height;
        m_sprite.setScale(scaleX, scaleY);
    }
}
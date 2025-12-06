//
// Created by Luca Letroye on 9/11/2025.
//
#include "views/EntityView.h"
#include "Camera.h"
#include "logic/patterns/Subject.h"
#include <memory>

EntityView::EntityView(logic::EntityModel &model, Camera &camera)
    : m_model(model), m_camera(camera) {
  m_model.attach(this);
}

EntityView::~EntityView() {}

void EntityView::onNotify(const logic::Subject &subject, logic::Event event) {
  m_sprite.setPosition(m_camera.project(m_model.getX(), m_model.getY()));
}

void EntityView::draw(sf::RenderWindow &window) { window.draw(m_sprite); }

void EntityView::onWindowResize() {
  // 1. Herbereken de positie (want de camera offset/schaal is veranderd)
  // Dit zorgt dat alles netjes op zijn plek blijft t.o.v. de zwarte balken
  m_sprite.setPosition(m_camera.project(m_model.getX(), m_model.getY()));

  // 2. Herbereken de schaal (grootte)
  // We halen de originele (ongeschaalde) afmetingen van de sprite op
  sf::FloatRect spriteBounds = m_sprite.getLocalBounds();

  // Haal de logische afmetingen uit het model (die we in de Factory hebben
  // ingesteld)
  double logicW = m_model.getWidth();
  double logicH = m_model.getHeight();

  // Vraag de camera: "Hoeveel pixels is deze logische grootte nu?"
  sf::Vector2f pixelSize = m_camera.computeSpriteSize(logicW, logicH);

  // Bereken de nieuwe schaalfactor
  // (Nieuwe Pixel Grootte / Originele Sprite Grootte)
  if (spriteBounds.width > 0 && spriteBounds.height > 0) {
    float scaleX = pixelSize.x / spriteBounds.width;
    float scaleY = pixelSize.y / spriteBounds.height;

    // Pas de schaal toe. Omdat PacMan, Munten en Muren nu allemaal
    // dezelfde logische 'tileSize' gebruiken, worden ze allemaal
    // perfect even groot geschaald.
    m_sprite.setScale(scaleX, scaleY);
  }
}
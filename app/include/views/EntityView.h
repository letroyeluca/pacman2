//
// Created by Luca Letroye on 26/10/2025.
//

#ifndef PACMAN_ENTITYVIEW_H
#define PACMAN_ENTITYVIEW_H
#include "logic/Event.h"
#include "logic/models/EntityModel.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>

class Camera; // Voorwaartse declaratie

class EntityView : public logic::Observer {
public:
  EntityView(logic::EntityModel &model, Camera &camera);

  // 1. DECLAREER de destructor hier (niet definiëren)
  virtual ~EntityView();
  void init();
  // 2. GEEF een lege inline implementatie
  //    (Aangezien de basisklasse-versie niets doet)
  virtual void updateAnimation(float dt) {}

  virtual void onNotify(const logic::Subject &subject,
                        logic::Event event) override;
  virtual void draw(sf::RenderWindow &window);
  virtual void onWindowResize();

protected:
  sf::Sprite m_sprite;
  logic::EntityModel &m_model;
  Camera &m_camera;

  // Deze twee zijn nu overbodig, de Camera-klasse doet dit werk
  // float m_windowWidth;
  // float m_windowHeight;
};
#endif // PACMAN_ENTITYVIEW_H

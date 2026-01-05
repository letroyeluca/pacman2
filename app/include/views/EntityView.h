#ifndef PACMAN_ENTITYVIEW_H
#define PACMAN_ENTITYVIEW_H

#include "Camera.h"
#include "logic/models/EntityModel.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Basisklasse voor alle visuele objecten. Koppelt een Logisch Model aan een SFML Sprite.
class EntityView : public logic::Observer, public std::enable_shared_from_this<EntityView> {
public:
    // Koppelt model en camera. Shared_ptr mag nullptr zijn (bv. voor UI elementen)
    EntityView(std::shared_ptr<logic::EntityModel> model, Camera& camera);

    virtual ~EntityView() = default;

    virtual void updateAnimation(float dt);      // Wordt elke frame aangeroepen voor animaties
    virtual void onWindowResize();               // Past grootte/positie aan bij schermwijziging
    virtual void draw(sf::RenderWindow& window); // Tekent de sprite op het scherm
    virtual void onNotify(const logic::Subject& subject, logic::Event event) override; // Luistert naar updates van het Model

    int getRenderLayer() const { return m_renderLayer; }
    void setRenderLayer(int layer) { m_renderLayer = layer; } // Bepaalt tekenvolgorde (z-index)
    void init();

protected:
    std::shared_ptr<logic::EntityModel> m_model;
    Camera& m_camera;
    sf::Sprite m_sprite;     // De visuele representatie (gedeeld door alle child-classes)
    int m_renderLayer = 0;   // 0 = achtergrond, hoger = voorgrond
};

#endif // PACMAN_ENTITYVIEW_H
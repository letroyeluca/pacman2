//
// Created by Luca Letroye on 26/10/2025.
//
#ifndef PACMAN_ENTITYVIEW_H
#define PACMAN_ENTITYVIEW_H

#include "Camera.h"
#include "logic/models/EntityModel.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>
#include <memory>

class EntityView : public logic::Observer {
public:
    // Accepteer shared_ptr (mag nullptr zijn)
    EntityView(std::shared_ptr<logic::EntityModel> model, Camera& camera);

    virtual ~EntityView() = default;

    virtual void updateAnimation(float dt);
    virtual void onWindowResize();
    virtual void draw(sf::RenderWindow& window);
    virtual void onNotify(const logic::Subject& subject, logic::Event event) override;
    int getRenderLayer() const { return m_renderLayer; }
    void setRenderLayer(int layer) { m_renderLayer = layer; }

protected:
    std::shared_ptr<logic::EntityModel> m_model;
    Camera& m_camera;
    sf::Sprite m_sprite; // Deze variabele is nodig voor alle subklassen!
    int m_renderLayer = 0;
};

#endif // PACMAN_ENTITYVIEW_H

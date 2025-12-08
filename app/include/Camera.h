//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_CAMERA_H
#define PACMAN_CAMERA_H

#include <SFML/System/Vector2.hpp>

class Camera {
public:
    Camera(float windowWidth, float windowHeight);
    sf::Vector2f project(double logicX, double logicY);
    sf::Vector2f computeSpriteSize(double logicWidth, double logicHeight);
    void onWindowResize(float newWidth, float newHeight);
    void setWorldDimensions(double width, double height);

private:
    float m_scale;
    float m_offsetX;
    float m_offsetY;
    float m_worldWidth;
    float m_worldHeight;
    float m_windowWidth;
    float m_windowHeight;
};

#endif // PACMAN_CAMERA_H

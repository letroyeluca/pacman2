#ifndef PACMAN_CAMERA_H
#define PACMAN_CAMERA_H

#include <SFML/System/Vector2.hpp>

// Vertaalt logische grid-coördinaten naar schermpixels (en andersom)
class Camera {
public:
    Camera(float windowWidth, float windowHeight);

    sf::Vector2f project(double logicX, double logicY);                    // Zet Logic (x,y) om naar Pixel (x,y)
    sf::Vector2f computeSpriteSize(double logicWidth, double logicHeight); // Berekent schaalgrootte

    void onWindowResize(float newWidth, float newHeight); // Past schaal aan bij venstergrootte
    void setWorldDimensions(double width, double height); // Stelt de logische grenzen in

private:
    float m_scale;   // Pixels per logische unit
    float m_offsetX; // Centrering X
    float m_offsetY; // Centrering Y
    float m_worldWidth;
    float m_worldHeight;
    float m_windowWidth;
    float m_windowHeight;
};

#endif // PACMAN_CAMERA_H
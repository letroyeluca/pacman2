//
// Created by Luca Letroye on 26/10/2025.
//
#include "Camera.h"
#include <algorithm>
#include <cmath>

Camera::Camera(float windowWidth, float windowHeight)
        : m_windowWidth(windowWidth), m_windowHeight(windowHeight),
          m_worldWidth(2.0f), m_worldHeight(2.0f) {
    onWindowResize(windowWidth, windowHeight);
}

void Camera::setWorldDimensions(double width, double height) {
    m_worldWidth = static_cast<float>(width);
    m_worldHeight = static_cast<float>(height);
    onWindowResize(m_windowWidth, m_windowHeight);
}

void Camera::onWindowResize(float newWidth, float newHeight) {
    m_windowWidth = newWidth;
    m_windowHeight = newHeight;

    float scaleX = m_windowWidth / m_worldWidth;
    float scaleY = m_windowHeight / m_worldHeight;

    m_scale = std::min(scaleX, scaleY);
    //m_scale *= 0.98f;
    float worldPixelWidth = m_worldWidth * m_scale;
    float worldPixelHeight = m_worldHeight * m_scale;

    m_offsetX = (m_windowWidth - worldPixelWidth) / 2.0f;
    m_offsetY = (m_windowHeight - worldPixelHeight) / 2.0f;
}

sf::Vector2f Camera::project(double logicX, double logicY) {
    float normX = logicX + (m_worldWidth / 2.0f);
    float normY = logicY + (m_worldHeight / 2.0f);

    return sf::Vector2f((normX * m_scale) + m_offsetX,(normY * m_scale) + m_offsetY);
}

sf::Vector2f Camera::computeSpriteSize(double logicWidth, double logicHeight) {
    return sf::Vector2f(logicWidth * m_scale,logicHeight * m_scale);
}
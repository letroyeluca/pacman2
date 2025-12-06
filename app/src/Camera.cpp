//
// Created by Luca Letroye on 26/10/2025.
//
#include "Camera.h"
#include <algorithm>

Camera::Camera(float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight) {
  onWindowResize(windowWidth, windowHeight);
}

void Camera::setWorldDimensions(double width, double height) {
  m_worldWidth = width;
  m_worldHeight = height;
  onWindowResize(m_windowWidth, m_windowHeight);
}

void Camera::onWindowResize(float newWidth, float newHeight) {
  m_windowWidth = newWidth;
  m_windowHeight = newHeight;

  // --- SLIMME SCHAAL MET MARGE ---
  // 1. Bereken de schaal om te fitten in breedte en hoogte
  float scaleX = newWidth / m_worldWidth;
  float scaleY = newHeight / m_worldHeight;

  // 2. Neem de kleinste (zodat alles past)
  // EN doe maal 0.98 voor een kleine veiligheidsmarge (padding).
  // Dit lost je "half zichtbare muren" probleem op.
  m_scale = std::min(scaleX, scaleY) * 0.98f;

  // 3. Centreer de wereld
  float worldPixelWidth = m_worldWidth * m_scale;
  float worldPixelHeight = m_worldHeight * m_scale;

  m_offsetX = (newWidth - worldPixelWidth) / 2.0f;
  m_offsetY = (newHeight - worldPixelHeight) / 2.0f;
}

sf::Vector2f Camera::project(double logicX, double logicY) {
  sf::Vector2f screenPos;

  // Normaliseer logische coördinaten [-1...1] naar [0...worldSize]
  // We gebruiken de halve wereldgrootte als offset omdat (0,0) het midden is.
  float normX = logicX + (m_worldWidth / 2.0);
  float normY = logicY + (m_worldHeight / 2.0);

  screenPos.x = (normX * m_scale) + m_offsetX;
  screenPos.y = (normY * m_scale) + m_offsetY;

  return screenPos;
}

sf::Vector2f Camera::computeSpriteSize(double logicWidth, double logicHeight) {
  sf::Vector2f spriteSize;
  spriteSize.x = logicWidth * m_scale;
  spriteSize.y = logicHeight * m_scale;
  return spriteSize;
}
//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_ENTITYMODEL_H
#define PACMAN_ENTITYMODEL_H

#include "logic/patterns/Subject.h"
#include "logic/utils/nouble.h"

namespace logic {

class Visitor;

class EntityModel : public Subject {
public:
  virtual ~EntityModel();
  virtual void update(float dt) = 0;
  virtual void accept(Visitor &visitor) = 0;

  double getX() const { return m_x.asFloat(); }
  double getY() const { return m_y.asFloat(); }
  double getWidth() const { return m_width.asFloat(); }
  double getHeight() const { return m_height.asFloat(); }

  bool isActive() const { return m_isActive; }
  void setActive(bool active) { m_isActive = active; }

  void setHitboxScale(double scale) { m_hitboxScale = scale; }
  double getHitboxScale() const { return m_hitboxScale; }

  bool collidesWith(const EntityModel &other) const;
  bool collidesWithBox(double otherX, double otherY, double otherW,
                       double otherH) const;

protected:
  EntityModel(double x, double y, double width, double height)
      : m_x(x), m_y(y), m_width(width), m_height(height), m_isActive(true),
        m_hitboxScale(1.0) {}
  Nouble m_x;
  Nouble m_y;
  Nouble m_width;
  Nouble m_height;
  bool m_isActive;
  double m_hitboxScale;
};
} // namespace logic

#endif // PACMAN_ENTITYMODEL_H

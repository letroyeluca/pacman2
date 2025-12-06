//
// Created by Luca Letroye on 9/11/2025.
//

#ifndef PACMAN_NOUBLE_H
#define PACMAN_NOUBLE_H
#pragma once
#include <algorithm>
#include <cmath>

struct Nouble {
  double m_value;

  // Constructor: automatisch clampen tussen -1 en 1
  Nouble(double v = 0.0) { m_value = std::clamp(v, -1.0, 1.0); }

  // Automatische conversie naar double
  operator double() const { return m_value; }

  // Toewijzing met clamp
  Nouble &operator=(double v) {
    m_value = std::clamp(v, -1.0, 1.0);
    return *this;
  }

  float asFloat() const {
    // static_cast zet de 'double' veilig om naar een 'float'
    return static_cast<float>(m_value);
  }

  // Basisoperatoren (je kan er meer toevoegen indien gewenst)
  Nouble operator+(double v) const { return Nouble(m_value + v); }
  Nouble operator-(double v) const { return Nouble(m_value - v); }
  Nouble operator*(double v) const { return Nouble(m_value * v); }
  Nouble operator/(double v) const {
    return Nouble(v == 0.0 ? m_value : m_value / v);
  }
  Nouble operator/(int v) const {
    return Nouble(v == 0.0 ? m_value : m_value / v);
  }

  Nouble &operator+=(double v) { return *this = m_value + v; }
  Nouble &operator-=(double v) { return *this = m_value - v; }
  Nouble &operator*=(double v) { return *this = m_value * v; }
  Nouble &operator/=(double v) {
    return *this = (v == 0.0 ? m_value : m_value / v);
  }
};

#endif // PACMAN_NOUBLE_H

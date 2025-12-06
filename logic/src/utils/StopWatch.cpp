//
// Created by Luca Letroye on 27/11/2025.
//

#include "logic/utils/StopWatch.h"
namespace logic {
Stopwatch &Stopwatch::getInstance() {
  static Stopwatch instance;
  return instance;
}

Stopwatch::Stopwatch() { m_lastTime = std::chrono::steady_clock::now(); }

float Stopwatch::tick() {
  const auto now = std::chrono::steady_clock::now();
  std::chrono::duration<float> deltaTime = now - m_lastTime;
  m_lastTime = now;
  return deltaTime.count();
}
} // namespace logic
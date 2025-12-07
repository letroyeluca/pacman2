//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H

#include <chrono>

namespace logic {
class Stopwatch {
public:
  float tick();
  static Stopwatch &getInstance();
  float getTime() const;

private:
  Stopwatch();
  Stopwatch(const Stopwatch &) = delete;
  Stopwatch &operator=(const Stopwatch &) = delete;
  std::chrono::steady_clock::time_point m_lastTime;
  std::chrono::steady_clock::time_point m_startTime;
};
} // namespace logic

#endif // PACMAN_STOPWATCH_H

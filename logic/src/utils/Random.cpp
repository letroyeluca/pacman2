//
// Created by Luca Letroye on 27/11/2025.
//
#include "logic/utils/Random.h"
namespace logic {

Random::Random() {
  std::random_device rd;
  m_engine.seed(rd());
}

Random &Random::getInstance() {
  static Random instance;
  return instance;
}

double Random::generate(double min, double max) {
  std::uniform_real_distribution<double> dis(min, max);
  return dis(m_engine);
}
} // namespace logic
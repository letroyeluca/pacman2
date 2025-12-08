//
// Created by Luca Letroye on 27/11/2025.
//

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H

#include <random>

namespace logic {

class Random {
public:
    static Random& getInstance();
    double generate(double min, double max);
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

private:
    Random();
    std::mt19937 m_engine;
};
} // namespace logic
#endif // PACMAN_RANDOM_H

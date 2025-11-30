//
// Created by Luca Letroye on 28/10/2025.
//

#ifndef PACMAN_EVENT_H
#define PACMAN_EVENT_H

namespace logic {
    enum class Event {
        Tick,
        PacManMoved,
        PacManDied,
        CoinEaten,
        FruitEaten,
        GhostEaten,
        GhostVulnerable,
        PacManUP,
        PacManDOWN,
        PacManLEFT,
        PacManRIGHT,
        DEFAULT,
        SCORE_CHANGED

    };
}

#endif //PACMAN_EVENT_H

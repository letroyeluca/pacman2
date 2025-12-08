//
// Created by Luca Letroye on 26/11/2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

class Game {
public:
    Game() = default;
    ~Game() = default;

    // main game start functie.
    void run(int width, int height);
};

#endif // PACMAN_GAME_H

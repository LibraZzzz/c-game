#ifndef GAME_H
#define GAME_H

#include "board.h"

typedef enum {
    GAME_RUNNING = 0,
    GAME_WON,
    GAME_LOST,
    GAME_QUIT
} GameStatus;

typedef struct {
    Board board;
    GameStatus status;
    int firstMove;
} Game;

int game_init(Game *game, int rows, int cols, int mines);
void game_destroy(Game *game);
void game_run(Game *game);

#endif

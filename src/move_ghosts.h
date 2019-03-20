#ifndef MOVE_GHOSTS_H
#define MOVE_GHOSTS_H

#include "defines.h"

void move_ghosts(char board[FIELD_HEIGHT][FIELD_WIDTH], int nr_ghosts, ghost_info ghost_array[4], int* pacman_caught, int moves);

#endif

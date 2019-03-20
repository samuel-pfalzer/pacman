/*
resets ghosts for a new level
*/

#include "init_ghosts.h"

void init_ghosts (ghost_info ghost_array[4]) {
	for (int i = 0; i < 4; i++) {
		ghost_array[i].pos.y = 9;
		ghost_array[i].pos.x = 8;
		ghost_array[i].last_direction = MOVE_RIGHT;
		ghost_array[i].covered_field = EMPTY_SYMBOL;
		ghost_array[i].active = 0;
		ghost_array[i].speed = 3 - i;
	}
}

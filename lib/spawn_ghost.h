#ifndef SPAWN_GHOST_H
#define SPAWN_GHOST_H

void spawn_ghost(
	char board[FIELD_HEIGHT][FIELD_WIDTH],
	ghost_info ghost_array[4],
	int* ghost_nr,
	int* release_timer
);

#endif

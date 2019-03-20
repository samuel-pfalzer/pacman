/*
controls all steps of spawning a new ghost
until the ghost is set to active and moved via move_ghost()
*/

void spawn_ghost(char board[FIELD_HEIGHT][FIELD_WIDTH], ghost_info ghost_array[4], int* ghost_nr, int* release_timer) {
	if (ghost_array[*ghost_nr].pos.x == 8 || ghost_array[*ghost_nr].pos.x == 10) {
		move_ghost_pre_spawn(board, ghost_array, *ghost_nr);
		*release_timer = 6;
	} else if (ghost_array[*ghost_nr].pos.x == 9 && ghost_array[*ghost_nr].pos.y == 9) {
		ghost_array[*ghost_nr].pos.y = 8;
		board[9][9] = EMPTY_SYMBOL;
		board[8][9] = GHOST_SYMBOL;
		*release_timer = 4;
	}

	if (*release_timer == 2) {
		board[8][9] = WALL_SYMBOL;
		ghost_array[*ghost_nr].covered_field = board[7][9];
		board[7][9] = GHOST_SYMBOL;
		ghost_array[*ghost_nr].pos.y = 7;
		ghost_array[*ghost_nr].active = 1;
	}

	if (*release_timer > 0) {
		*release_timer -= 1;
	} else {
		*release_timer = 42;
		*ghost_nr += 1;
	}
}

/*
moves ghosts whilst they are waiting to be spawned, but still inactive
*/

#include "../lib/defines.h"
#include "../lib/move_ghost_pre_spawn.h"

void move_ghost_pre_spawn(char board[FIELD_HEIGHT][FIELD_WIDTH], ghost_info ghost_array[4], int ghost_nr) {
	//move ghost to new direction
	switch (ghost_array[ghost_nr].pos.x) {
		case 8:
			board[9][8] = EMPTY_SYMBOL;
			ghost_array[ghost_nr].pos.x = 9;
			ghost_array[ghost_nr].last_direction = MOVE_RIGHT;
			break;
		case 9:
			board[9][9] = EMPTY_SYMBOL;
			switch (ghost_array[ghost_nr].last_direction) {
				case MOVE_LEFT:
					ghost_array[ghost_nr].pos.x = 8;
					break;
				case MOVE_RIGHT:
					ghost_array[ghost_nr].pos.x = 10;
					break;
			}
			break;
		case 10:
			board[9][10] = EMPTY_SYMBOL;
			ghost_array[ghost_nr].pos.x = 9;
			ghost_array[ghost_nr].last_direction = MOVE_LEFT;
			break;
	}

	//draw ghost to board
	board[ghost_array[ghost_nr].pos.y][ghost_array[ghost_nr].pos.x] = GHOST_SYMBOL;
}

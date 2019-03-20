/*
move alle active ghosts
preferably, ghosts should continue their movement in the current direction
if impossible, they randomly choose one of the possible directions
*/

#include "move_ghosts.h"
#include <stdlib.h>

void move_ghosts(char board[FIELD_HEIGHT][FIELD_WIDTH], int nr_ghosts, ghost_info ghost_array[4], int* pacman_caught, int moves) {
	
	//check if pacman was caught
	for (int i = 0; i < nr_ghosts; i++) {
		if (board[ghost_array[i].pos.y][ghost_array[i].pos.x] == PLAYER_SYMBOL) {
			*pacman_caught = 1;
		}
	}
	
	//remove ghosts from board
	for (int i = 0; i < nr_ghosts; i++) {
		if (ghost_array[i].covered_field != UNDEFINED_SYMBOL) {
			board[ghost_array[i].pos.y][ghost_array[i].pos.x] = ghost_array[i].covered_field;
		}
	}

	//move ghosts and add them to board
	for (int i = 0; i < nr_ghosts; i++) {
		if (ghost_array[i].active == 1) {
			position next_position = ghost_array[i].pos;
			int direction_valid = 0;
			char next_direction = ghost_array[i].last_direction;
			char direction_map[4] = {MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT};
		
			//determine direction if ghost needs to move
			if (moves % ghost_array[i].speed == 0) {
				//find a valid direction to move
				while (direction_valid == 0) {
					next_position = ghost_array[i].pos;

					//first check if continuing in same direction is possible
					//from second iteration, next_direction holds randomly generated direction
					switch (next_direction) {
						case MOVE_UP:
							next_position.y--;
							break;
						case MOVE_RIGHT:
							next_position.x++;
							break;
						case MOVE_DOWN:
							next_position.y++;
							break;
						case MOVE_LEFT:
							next_position.x--;
							break;
						default:
							break;
					}

					if (
						board[next_position.y][next_position.x] != WALL_SYMBOL &&
						(next_position.y != ghost_array[i].pos.y || next_position.x != ghost_array[i].pos.x) &&
						next_position.x >= 0 && next_position.x < FIELD_WIDTH
					) {
						direction_valid = 1;
					}
					
					//if not, try a different destination
					if (direction_valid == 0) {
						char old_direction = next_direction;
						do {
							int rand_direction = rand() % 4;
							next_direction = direction_map[rand_direction];
						} while (next_direction == old_direction);
					}
				}
				
				//store content of destination
				ghost_array[i].pos = next_position;
				if (board[next_position.y][next_position.x] == FOOD_SYMBOL || board[next_position.y][next_position.x] == EMPTY_SYMBOL) {
					ghost_array[i].covered_field = board[next_position.y][next_position.x];
				} else {
					ghost_array[i].covered_field = UNDEFINED_SYMBOL;
				}
			}

			//draw ghost to board at new position
			board[next_position.y][next_position.x] = GHOST_SYMBOL;

			//store direction for next move
			ghost_array[i].last_direction = next_direction;
		}
	}
}

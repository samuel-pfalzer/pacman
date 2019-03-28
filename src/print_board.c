#include "../lib/defines.h"
#include "../lib/print_board.h"

#include <stdio.h>
#include <stdlib.h>

void print_board(
	char board[FIELD_HEIGHT][FIELD_WIDTH],
	int score,
	int lives,
	int level,
	int food_to_win,
	char player_last_action
) {
	system("clear");

	//update player symbol (v, <, ^, >) based on movement direction
	char current_player_symbol;
	switch (player_last_action) {
		case MOVE_UP:
			current_player_symbol = 'v';
			break;
		case MOVE_RIGHT:
			current_player_symbol = '<';
			break;
		case MOVE_DOWN:
			current_player_symbol = '^';
			break;
		case MOVE_LEFT:
			current_player_symbol = '>';
			break;
		default:
			current_player_symbol = '<';
			break;
	}

	printf("PACMAN ---- LEVEL %d\n\n", level);
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			
			if (board[y][x] == PLAYER_SYMBOL) {
				
				//print player symbol in bold yellow
				printf("\033[1;33m%c\033[0m", current_player_symbol);
			
			} else if (board[y][x] == WALL_SYMBOL) {
				
				//print walls in dark blue
				printf("\033[1;44m \033[0m");

			} else if (board[y][x] == GHOST_SYMBOL) {
				
				//print ghosts in bold red
				printf("\033[1;91mG\033[0m");

			} else {
				printf("%c", board[y][x]);
			}
		}
		printf("\n");
	}

	//additional information
	printf("\n");
	printf("Score: %d/%d\n", score, food_to_win);
	printf("Lives: %d\n", lives);
}

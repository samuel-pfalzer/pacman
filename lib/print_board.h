#ifndef PRINT_BOARD_H
#define PRINT_BOARD_H

void print_board(
	char board[FIELD_HEIGHT][FIELD_WIDTH],
	int score,
	int lives,
	int level,
	int food_to_win,
	char player_last_action
);

#endif

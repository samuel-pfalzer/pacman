#ifndef MOVE_PLAYER_H
#define MOVE_PLAYER_H

position move_player(
	char board[FIELD_HEIGHT][FIELD_WIDTH],
	position player_position,
	char* player_next_action,
	int* score, int* lives,
	int* game_over,
	int* pacman_caught,
	char* player_last_action
);

#endif

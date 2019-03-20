/*
move the player based on input provided by the input handler thread
*/

position move_player(char board[FIELD_HEIGHT][FIELD_WIDTH], position player_position, char* player_next_action, int* score, int* lives, int* game_over, int* pacman_caught, char* player_last_action) {
	position next_position = player_position;
	
	//prevent removing two lives at once
	int life_removed = 0;

	if (*pacman_caught == 1) {
		if (*lives > 0) {
			*lives -= 1;
		} else {
			*game_over = 1;
		}
		*pacman_caught = 0;
		life_removed = 1;
	}

	if (*player_next_action == MOVE_LEFT && player_position.x == 0) {
		next_position.x = FIELD_WIDTH - 1;
	} else if (*player_next_action == MOVE_RIGHT && player_position.x == FIELD_WIDTH - 1) {
		next_position.x = 0;
	} else {
		switch(*player_next_action) {
			case MOVE_STOP:
				break;
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
	}

	char destination = board[next_position.y][next_position.x];
	if (destination == EMPTY_SYMBOL || destination == FOOD_SYMBOL) {
		if (destination == FOOD_SYMBOL) {
			*score += 1;
		}
		board[player_position.y][player_position.x] = EMPTY_SYMBOL;
		player_position = next_position;
	} else if (destination == GHOST_SYMBOL) {
		if (life_removed == 0) {
			if (*lives > 0) {
				*lives -= 1;
			} else {
				*game_over = 1;
			}
		}
		board[player_position.y][player_position.x] = EMPTY_SYMBOL;
		player_position = next_position;
	}

	board[player_position.y][player_position.x] = PLAYER_SYMBOL;
	*player_last_action = *player_next_action;
	return player_position;
}

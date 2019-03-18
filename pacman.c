#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "getch.h"

#define WALL_SYMBOL '#'
#define EMPTY_SYMBOL ' '
#define PLAYER_SYMBOL 'P'
#define GHOST_SYMBOL 'G'
#define FOOD_SYMBOL '.'
#define UNDEFINED_SYMBOL 'u'

#define MOVE_UP 'w'
#define MOVE_RIGHT 'd'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_STOP 'h'
#define QUIT 'q'

#define FIELD_HEIGHT 21
#define FIELD_WIDTH 19


//STRUCT: stores a position (x and y coordinates)
typedef struct position {
	int y;
	int x;
} position;


//STRUCT: stores information about a level of the game
typedef struct level_info {
	int speed;
	int nr_ghosts;
	int food_to_win;
	int release_after[4];
} level_info;


//STRUCT: store information about ghosts
typedef struct ghost_info {
	position pos;
	char last_direction;
	char covered_field;
	int active;
	int speed;
} ghost_info;


//FUNCTION: fill board with borders
void initialize_board(char board[FIELD_HEIGHT][FIELD_WIDTH]) {
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (
				y == 0 || y == FIELD_HEIGHT - 1 ||
				x == 0 || x == FIELD_WIDTH - 1
			) {
				board[y][x] = WALL_SYMBOL;
			} else {
				board[y][x] = EMPTY_SYMBOL;
			}
		}
	}
	board[7][0] = EMPTY_SYMBOL;
	board[7][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
	board[9][0] = EMPTY_SYMBOL;
	board[9][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
	board[11][0] = EMPTY_SYMBOL;
	board[11][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
}

void add_internal_borders(char board[FIELD_HEIGHT][FIELD_WIDTH]) {
	//store border positions
	position borders[] = {
		{1, 9},
		{2, 2}, {2, 3}, {2, 5}, {2, 6}, {2, 7}, {2, 9}, {2, 11}, {2, 12}, {2, 13}, {2, 15}, {2, 16},
		{4, 2}, {4, 3}, {4, 5}, {4, 7}, {4, 8}, {4, 9}, {4, 10}, {4, 11}, {4, 13}, {4, 15}, {4, 16},
		{5, 5}, {5, 9}, {5, 13},
		{6, 1}, {6, 2}, {6, 3}, {6, 5}, {6, 6}, {6, 7}, {6, 9}, {6, 11}, {6, 12}, {6, 13}, {6, 15}, {6, 16}, {6, 17},
		{7, 3}, {7, 5}, {7, 13}, {7, 15},
		{8, 1}, {8, 2}, {8, 3}, {8, 5}, {8, 7}, {8, 8}, {8, 9}, {8, 10}, {8, 11}, {8, 13}, {8, 15}, {8, 16}, {8, 17},
		{9, 7}, {9, 11},
		{10, 1}, {10, 2}, {10, 3}, {10, 5}, {10, 7}, {10, 8}, {10, 9}, {10, 10}, {10, 11}, {10, 13}, {10, 15}, {10, 16}, {10, 17},
		{11, 3}, {11, 5}, {11, 13}, {11, 15},
		{12, 1}, {12, 2}, {12, 3}, {12, 5}, {12, 7}, {12, 8}, {12, 9}, {12, 10}, {12, 11}, {12, 13}, {12, 15}, {12, 16}, {12, 17},
		{13, 9},
		{14, 2}, {14, 3}, {14, 5}, {14, 6}, {14, 7}, {14, 9}, {14, 11}, {14, 12}, {14, 13}, {14, 15}, {14, 16},
		{15, 3}, {15, 15},
		{16, 1}, {16, 3}, {16, 5}, {16, 7}, {16, 8}, {16, 9}, {16, 10}, {16, 11}, {16, 13}, {16, 15}, {16, 17},
		{17, 5}, {17, 9}, {17, 13},
		{18, 2}, {18, 3}, {18, 4}, {18, 5}, {18, 6}, {18, 7}, {18, 9}, {18, 11}, {18, 12}, {18, 13}, {18, 14}, {18, 15}, {18, 16}
	};
	int nr_borders = sizeof(borders) / sizeof(borders[0]);
	for (int i = 0; i < nr_borders; i++) {
		board[borders[i].y][borders[i].x] = WALL_SYMBOL;
	}
}

void add_food(char board[FIELD_HEIGHT][FIELD_WIDTH]) {
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (board[y][x] == EMPTY_SYMBOL &&
				!(y == 9 && (x == 8 || x == 9 || x == 10)) &&
				!(y == 7 && (x == 0 || x == 1 || x == 2)) &&
				!(y == 7 && (x == 16 || x == 17 || x == 18)) &&
				!(y == 11 && (x == 0 || x == 1 || x == 2)) &&
				!(y == 11 && (x == 16 || x == 17 || x == 18))
			) {
				board[y][x] = FOOD_SYMBOL;
			}
		}
	}
}

//FUNCTION: print game board
void print_board(char board[FIELD_HEIGHT][FIELD_WIDTH], int score, int lives, int level, int food_to_win, char current_player_symbol) {
	system("clear");
	printf("PACMAN ---- LEVEL %d\n\n", level);
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (board[y][x] == PLAYER_SYMBOL) {
				printf("\033[1;33m%c\033[0m", current_player_symbol);
			} else if (board[y][x] == WALL_SYMBOL) {
				printf("\033[1;44m \033[0m");
			} else if (board[y][x] == GHOST_SYMBOL) {
				printf("\033[1;91mG\033[0m");			
			} else {
				printf("%c", board[y][x]);
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("Score: %d/%d\n", score, food_to_win);
	printf("Lives: %d\n", lives);
}


//FUNCTION: update player position + board, score, lives, game_over
position update_player_position(char board[FIELD_HEIGHT][FIELD_WIDTH], position player_position, char* player_next_action, int* score, int* lives, int* game_over, int* pacman_caught, char* player_last_action) {
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


//FUNCTION: move ghosts and update board accordingly
void update_ghost_positions(char board[FIELD_HEIGHT][FIELD_WIDTH], int nr_ghosts, ghost_info ghost_array[4], int* pacman_caught, int moves) {
	
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


//FUNCTION: update player symbol before printing
char update_player_symbol (char player_last_action, char current_player_symbol) {
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
			break;
	}
	return current_player_symbol;
}


//FUNCTION: initialize ghosts for new level
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


//FUNCTION: move ghost before spawn
void pre_release_ghost_move(char board[FIELD_HEIGHT][FIELD_WIDTH], ghost_info ghost_array[4], int ghost_nr) {
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


//FUNCTION: coordinate spawn of a ghost
void release_ghost(char board[FIELD_HEIGHT][FIELD_WIDTH], ghost_info ghost_array[4], int* ghost_nr, int* release_timer) {
	if (ghost_array[*ghost_nr].pos.x == 8 || ghost_array[*ghost_nr].pos.x == 10) {
		pre_release_ghost_move(board, ghost_array, *ghost_nr);
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


//FUNCTION/THREAD: handle user input
void* getch_loop(void* ptr) {
	char* player_next_action = (char*) ptr;

	while(*player_next_action != QUIT) {
		*player_next_action = getch();
	}
}


//main
int main() {
	//seed random numbers
	srand(time(NULL));

	//some variables
	int game_over = 0;
	int score;
	int lives = 3;
	int level = 0;
	int moves;
	int pacman_caught = 0;
	char current_player_symbol = '<';

	//store next movement direction
	char player_next_action;
	player_next_action = MOVE_STOP;

	//store last movement direction for printing
	char player_last_action = MOVE_RIGHT;

	//store info about available levels
	level_info arr_level[] = {
//		{150000, 0, 60, {0, 0, 0, 0}},
		{140000, 1, 70, {20, 20, 20, 20}},
		{130000, 1, 80, {20, 20, 20, 20}},
		{120000, 2, 90, {20, 100, 100, 100}},
		{110000, 2, 100, {20, 80, 80, 80}},
		{100000, 3, 110, {20, 60, 120, 120}}
	};
	int nr_levels = sizeof(arr_level) / sizeof(arr_level[0]);

	//create and fill game board
	char board[FIELD_HEIGHT][FIELD_WIDTH];

	//store player position
	position player_position;

	//THREAD to handle user input
	pthread_t input_thread;
	pthread_create(&input_thread, NULL, getch_loop, (void*) &player_next_action);

	//start levels
	for (; level < nr_levels; level++) {
		score = 0;
		moves = 0;
		player_position.y = 1;
		player_position.x = 1;
		player_next_action = MOVE_STOP;
		initialize_board(board);
		add_internal_borders(board);
		add_food(board);

		ghost_info ghost_array[4];
		int nr_active_ghosts = 0;
		init_ghosts(ghost_array);
		int ghost_release_timer = 42;

		//level intro
		for (int t = 3; t > 0; t--) {
			system("clear");
			printf("Level %d starting in %d ...\n", level, t);
			sleep(1);
			if (player_next_action == QUIT) {
				break;
			}
		}

		while (game_over == 0 && player_next_action != QUIT && score < arr_level[level].food_to_win) {
			
			//move alle active ghosts
			update_ghost_positions(board, nr_active_ghosts, ghost_array, &pacman_caught, moves);
		
			//check if ghost needs to be released
			if ((arr_level[level].nr_ghosts > 0 && arr_level[level].release_after[nr_active_ghosts] == moves) || ghost_release_timer < 42) {
				release_ghost(board, ghost_array, &nr_active_ghosts, &ghost_release_timer);
			} else if (nr_active_ghosts < arr_level[level].nr_ghosts && ghost_release_timer == 42 && moves % (ghost_array[nr_active_ghosts].speed + 1) == 1) {
				pre_release_ghost_move(board, ghost_array, nr_active_ghosts);
			}

			//move pacman
			player_position = update_player_position(board, player_position, &player_next_action, &score, &lives, &game_over, &pacman_caught, &player_last_action);
			
			//print game to screen, then wait for next iteration
			current_player_symbol = update_player_symbol(player_last_action, current_player_symbol);
			print_board(board, score, lives, level, arr_level[level].food_to_win, current_player_symbol);
			moves++;
			usleep(arr_level[level].speed);
		}

		if (game_over == 1 || player_next_action == QUIT) {
			printf("\nGAME OVER!\n");
			break;
		} else {
			printf("\nYou finished level %d!\n", level);
			sleep(2);
		}
	}

	//wait for input thread to finish
	if (player_next_action != QUIT) {
		printf("\nHit <q> to quit!\n");
	}
	pthread_join(input_thread, NULL);

	return 0;
}

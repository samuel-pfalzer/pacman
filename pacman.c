#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "getch.h"


#define WALL_SYMBOL '#'
#define EMPTY_SYMBOL ' '
#define PLAYER_SYMBOL 'P'
#define GHOST_SYMBOL 'G'
#define FOOD_SYMBOL '.'

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
} level_info;


//STRUCT: store information about ghosts
typedef struct ghost_info {
	position pos;
	char last_direction;
}


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
void print_board(char board[FIELD_HEIGHT][FIELD_WIDTH], int score, int lives, int level) {
	system("clear");
	printf("PACMAN --- LEVEL %d\n\n", level);
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (board[y][x] == PLAYER_SYMBOL) {
				printf("\033[1;33m%c\033[0m", board[y][x]);
			} else if (board[y][x] == WALL_SYMBOL) {
				printf("\033[01;44m \033[0m");
			} else {
				printf("%c", board[y][x]);
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("Score: %d\n", score);
	printf("Lives: %d\n", lives);
}


//FUNCTION: update player position + board, score, lives, game_over
position update_player_position(char board[FIELD_HEIGHT][FIELD_WIDTH], position player_position, char* player_next_action, int* score, int* lives, int* game_over) {
	position next_position = player_position;

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
		board[player_position.y][player_position.x] = PLAYER_SYMBOL;
	} else if (destination == GHOST_SYMBOL) {
		if (*lives > 0) {
			*lives -= 1;
		} else {
			*game_over = 1;
		}
		board[player_position.y][player_position.x] = EMPTY_SYMBOL;
		player_position = next_position;
		board[player_position.y][player_position.x] = PLAYER_SYMBOL;
	}

	*player_next_action = MOVE_STOP;
	return player_position;
}


//THREAD: handle user input

void* getch_loop(void* ptr) {
	char* player_next_action = (char*) ptr;

	while(*player_next_action != QUIT) {
		*player_next_action = getch();
	}
}


//main
int main() {
	//some variables
	int game_over = 0;
	int score = 0;
	int lives = 3;
	int level = 0;

	//store next movement direction
	char player_next_action;
	player_next_action = MOVE_STOP;

	//store info about all available levels
	level_info arr_level[] = {
		{100000, 0, 20},
		{75000, 0, 20},
		{50000, 0, 20}
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
		player_position.y = 1;
		player_position.x = 1;
		initialize_board(board);
		add_internal_borders(board);
		add_food(board);
		
		//level intro
		for (int t = 3; t > 0; t--) {
			system("clear");
			printf("Level %d starting in %d ...\n", level, t);
			sleep(1);
		}

		while (game_over == 0 && player_next_action != QUIT && score < arr_level[level].food_to_win) {
			player_position = update_player_position(board, player_position, &player_next_action, &score, &lives, &game_over);
			print_board(board, score, lives, level);
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

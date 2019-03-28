#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <time.h>

#include "../lib/getch.h"
#include "../lib/defines.h"
#include "../lib/add_food.h"
#include "../lib/getch_loop.h"
#include "../lib/init_board.h"
#include "../lib/init_ghosts.h"
#include "../lib/move_ghost_pre_spawn.h"
#include "../lib/move_ghosts.h"
#include "../lib/move_player.h"
#include "../lib/print_board.h"
#include "../lib/spawn_ghost.h"


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

	//store direction for a) move_player() and b) print_board()
	char player_next_action = MOVE_STOP;
	char player_last_action = MOVE_RIGHT;

	//store info about available levels
	level_info arr_level[] = {
		{150000, 0, 60, {0, 0, 0, 0}},
		{140000, 1, 70, {20, 20, 20, 20}},
		{130000, 1, 80, {20, 20, 20, 20}},
		{120000, 2, 90, {20, 100, 100, 100}},
		{110000, 2, 100, {20, 80, 80, 80}},
		{100000, 3, 110, {20, 60, 120, 120}}
	};
	int nr_levels = sizeof(arr_level) / sizeof(arr_level[0]);

	//create game board
	char board[FIELD_HEIGHT][FIELD_WIDTH];

	//store player position
	position player_position;

	//THREAD to handle user input
	pthread_t input_thread;
	pthread_create(&input_thread, NULL, getch_loop, (void*) &player_next_action);

	//start levels
	for (; level < nr_levels; level++) {
		
		//reset board for each level
		score = 0;
		moves = 0;
		player_position.y = 1;
		player_position.x = 1;
		player_next_action = MOVE_STOP;
		init_board(board);
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
			move_ghosts(board, nr_active_ghosts, ghost_array, &pacman_caught, moves);
		
			//check if ghost needs to be released
			if ((arr_level[level].nr_ghosts > 0 && arr_level[level].release_after[nr_active_ghosts] == moves) || ghost_release_timer < 42) {
				spawn_ghost(board, ghost_array, &nr_active_ghosts, &ghost_release_timer);
			} else if (nr_active_ghosts < arr_level[level].nr_ghosts && ghost_release_timer == 42 && moves % (ghost_array[nr_active_ghosts].speed + 1) == 1) {
				move_ghost_pre_spawn(board, ghost_array, nr_active_ghosts);
			}

			//move pacman
			player_position = move_player(board, player_position, &player_next_action, &score, &lives, &game_over, &pacman_caught, &player_last_action);
			
			//print game to screen
			print_board(board, score, lives, level, arr_level[level].food_to_win, player_last_action);
			
			//increment counter for ghost timing, wait till next cycle
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

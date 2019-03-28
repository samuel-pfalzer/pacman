/*
FUNCTION: initialize board with borders(/walls) and spaces
*/

#include "../lib/defines.h"
#include "../lib/init_board.h"

void init_board(char board[FIELD_HEIGHT][FIELD_WIDTH]) {
	
	//add borders around the edge and empty spaces inside
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
	
	//clean up left and right edge
	board[7][0] = EMPTY_SYMBOL;
	board[7][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
	board[9][0] = EMPTY_SYMBOL;
	board[9][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
	board[11][0] = EMPTY_SYMBOL;
	board[11][FIELD_WIDTH - 1] = EMPTY_SYMBOL;
	
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

	//add internal borders
	int nr_borders = sizeof(borders) / sizeof(borders[0]);
	for (int i = 0; i < nr_borders; i++) {
		board[borders[i].y][borders[i].x] = WALL_SYMBOL;
	}
}

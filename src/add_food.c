/*
add food to all empty spaces except
	a) the ghost spawning area and
	b) the unaccessable fields at both sides
*/

#include "../lib/defines.h"
#include "../lib/add_food.h"

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

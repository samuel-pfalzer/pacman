/*
function to get user input constantly
designed to be executed by a dedicated thread
*/

#include "getch_loop.h"
#include "../lib/getch.h"

void* getch_loop(void* ptr) {
	char* player_next_action = (char*) ptr;

	while(*player_next_action != QUIT) {
		*player_next_action = getch();
	}
}

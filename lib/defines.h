#ifndef DEFINE_H
#define DEFINE_H

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

#endif

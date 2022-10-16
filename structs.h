#ifndef _STRUCTS_H
#define _STRUCTS_H

typedef struct board_tile {
		char x;
		char y;
		
		char type;
		
		struct board_tile *next[3];
		
		char has_neighbor_ne;
		char has_neighbor_nw;
		char has_neighbor_se;
		char has_neighbor_sw;
		
		char z1;
		char z2;
		char z3;
};

typedef struct player {
	struct board_tile *standing;
	struct board_tile *home_tile;
	
	char player_state;
	char player_num;
	char character_index;
	
	char attack;
	char evade;
	char defend;
	
	char maxhp;
	char hp;
	
	char attack_modifier;
	char evade_modifier;
	char defend_modifier;
	
	short stars;
	short wins;
	char norma_level;
	char norma_type;
	
	char base_revive;
	char revive;
};

typedef struct game_info {
	char whose_turn;
	char chapter;
};

#define STATE_IDLE 0
#define STATE_ATTACK 1
#define STATE_DAMAGED 2
#define STATE_NORMA 3
#define STATE_DEAD 4
#define STATE_ROLL 5

#define TYPE_DRAW 0
#define TYPE_DROP 1
#define TYPE_WARP 2
#define TYPE_BONUS 3
#define TYPE_ENEMY 4
#define TYPE_NEUTRAL 5

#define TYPE_HOME 6

#define NORMA_STAR 0
#define NORMA_WIN 1

#define ACTION_DEFEND 0
#define ACTION_EVADE 1


#endif
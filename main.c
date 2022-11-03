#include <peekpoke.h>
#include <cbm.h>
#include <cx16.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "structs.h"
#include "routines.h"

#define DEVICE_NUM 8

#define RAM_BANK *((volatile char *)0)

#define NUM_TO_ASCII(x) (x >= 10 ? (x - 10 + 0x41) : (x + 0x30))

extern void waitforjiffy();

char setup_characters[][16] = {
	"kai", // 0
	"qp", // 1
	"suguri", // 2
	"marc", // 3
	"yuki", // 4
	"aru", // 5
	"hime", // 6
	"sora", // 7
	"fernet", // 8
	"peat", // 9
	"mariepoppo", // a
	"tomomo", // b
	"mio", // c
	"chicken", // $0d
	"roboball", // $0e
	"seagull", // $0f
};
#define SETUP_CHARACTERS_LEN 16

#define PLAYER_CHICKEN 0xd
#define PLAYER_ROBOBALL 0xe
#define PLAYER_SEAGULL 0xf


char map_data[] = {
	/* type x y   indexes link to */
	4, 0, 0, 0x01, 0xFF, 0xFF, // 0
	2, 0, 1, 0x02, 0xFF, 0xFF, // 1
	3, 0, 2, 0x03, 0xFF, 0xFF, // 2
	0, 0, 3, 0x04, 0xFF, 0xFF, // 3
	3, 0, 4, 0x05, 0xFF, 0xFF, // 4
	5, 0, 5, 0x06, 0xFF, 0xFF, // 5
	4, 0, 6, 0x07, 0xFF, 0xFF, // 6
	3, 1, 6, 0x08, 0xFF, 0xFF, // 7
	2, 1, 7, 0x09, 0xFF, 0xFF, // 8
	0, 2, 7, 0x0a, 0xFF, 0xFF, // 9
	3, 2, 8, 0x0b, 0xFF, 0xFF, // a
	0, 3, 8, 0x0c, 0xFF, 0xFF, // b
	3, 4, 8, 0x0d, 0x33, 0xFF, // c
	5, 5, 8, 0x0e, 0xFF, 0xFF, // d
	0, 6, 8, 0x0f, 0xFF, 0xFF, // e
	2, 7, 8, 0x10, 0xFF, 0xFF, // f
	4, 8, 8, 0x11, 0xFF, 0xFF, // 10
	2, 8, 7, 0x12, 0xFF, 0xFF, // 11
	3, 8, 6, 0x13, 0xFF, 0xFF, // 12
	0, 8, 5, 0x14, 0xFF, 0xFF, // 13
	3, 8, 4, 0x15, 0xFF, 0xFF, // 14
	5, 8, 3, 0x16, 0xFF, 0xFF, // 15
	4, 8, 2, 0x17, 0xFF, 0xFF, // 16
	3, 7, 2, 0x18, 0xFF, 0xFF, // 17
	2, 7, 1, 0x19, 0xFF, 0xFF, // 18
	0, 6, 1, 0x1a, 0xFF, 0xFF, // 19
	3, 6, 0, 0x1b, 0xFF, 0xFF, // 1a
	0, 5, 0, 0x1c, 0xFF, 0xFF, // 1b
	3, 4, 0, 0x1d, 0x31, 0xFF, // 1c
	5, 3, 0, 0x1e, 0xFF, 0xFF, // 1d
	0, 2, 0, 0x1f, 0xFF, 0xFF, // 1e
	2, 1, 0, 0x00, 0xFF, 0xFF, // 1f
	
	6, 2, 2, 0x21, 0xFF, 0xFF, // 20
	4, 2, 3, 0x22, 0xFF, 0xFF, // 21
	0, 2, 4, 0x23, 0x30, 0xFF, // 22
	4, 2, 5, 0x24, 0xFF, 0xFF, // 23
	6, 3, 5, 0x25, 0xFF, 0xFF, // 24
	4, 3, 6, 0x26, 0xFF, 0xFF, // 25
	0, 4, 6, 0x27, 0xFF, 0xFF, // 26
	1, 5, 6, 0x28, 0xFF, 0xFF, // 27
	6, 6, 6, 0x29, 0xFF, 0xFF, // 28
	4, 6, 5, 0x2a, 0xFF, 0xFF, // 29
	0, 6, 4, 0x2b, 0x32, 0xFF, // 2a
	4, 6, 3, 0x2c, 0xFF, 0xFF, // 2b
	6, 5, 3, 0x2d, 0xFF, 0xFF, // 2c
	4, 5, 2, 0x2e, 0xFF, 0xFF, // 2d
	0, 4, 2, 0x2f, 0xFF, 0xFF, // 2e
	1, 3, 2, 0x20, 0xFF, 0xFF, // 2f
	
	1, 1, 4, 0x04, 0xFF, 0xFF, // 30
	1, 4, 1, 0x2e, 0xFF, 0xFF, // 31
	1, 7, 4, 0x14, 0xFF, 0xFF, // 32
	1, 4, 7, 0x26, 0xFF, 0xFF, // 33
};

char player_data_table[][5] = {
	{4, 3, 3, 5, 5},
	{3, 3, 3, 5, 5},
	{4, 2, 5, 4, 5},
	{4, 4, 2, 4, 5},
	{5, 2, 2, 5, 5},
	{2, 2, 5, 5, 5},
	{4, 2, 4, 5, 5},
	{4, 3, 4, 5, 4},
	{2, 5, 1, 5, 6},
	{4, 4, 4, 3, 4},
	{2, 2, 2, 7, 5},
	{5, 3, 3, 4, 6},
	{3, 2, 4, 6, 5},
	{2, 2, 4, 3, 3},
	{2, 4, 2, 3, 4},
	{4, 2, 2, 3, 4},
};


char norma_stars[] = {10, 30, 70, 120, 200};
char norma_wins[] = {0, 2, 5, 9, 14};

struct board_tile *game_tiles = (struct board_tile *)0x8000; //[255];

struct board_tile **norma_tiles = (struct board_tile **)0x7000;

struct player players[4];

struct game_info game;

char enemy_is_alive = 0;
struct player enemy;

char key;
char steps;
char player_has_reached_level_4;

char k;

extern void setup_rand();

void main() {
	static unsigned short i;
	
	setup_rand();
	
	enemy_is_alive = 0;
	player_has_reached_level_4 = 0;
	
	character_menu();
	setup_video();
	
	POKE(0x9F20, 0);
	POKE(0x9F21, 0xFA);
	POKE(0x9F22, 0x11);
	
	for (i = 0; i < (32 * 7); ++i) {
		POKE(0x9F23, base_palettes[0][i]);	
	}
	
	//VERA.layer1.hscroll = 31 * 16;
	parse_map_data(map_data, 0x34);
	for (i = 0; i < 0x34; ++i) {
		plot_tile(&(game_tiles[i]));
	}
	
	game.chapter = 1;
	game.whose_turn = 0;
	for (i = 0; i < 4; ++i) {
		players[i].standing = norma_tiles[i];
		players[i].home_tile = norma_tiles[i];
		players[i].player_num = i;
		players[i].stars = 1;
	}
	
	draw_gui();
	draw_players();
	while (1) {
		if (players[game.whose_turn].hp == 0) {
			center_around_player(&players[game.whose_turn]);
			draw_players();
			players[game.whose_turn].player_state = STATE_ROLL;
			display_text_sprite(INDEX_REVIVE, SIZE_REVIVE, 160, 96);
			display_player_no(game.whose_turn + 1, 160, 96);
			while (keyboard_get() != 0x20) {
				draw_player_sprite(&players[game.whose_turn], 0);
				players[game.whose_turn].player_state = players[game.whose_turn].player_state == STATE_IDLE ? STATE_ROLL : STATE_IDLE;
				wait_jiffies(30);
			}
			i = roll_die();
			display_die_sprite(0x170, i, 200, 128 - 32 - 8);
			if (i >= players[game.whose_turn].revive) {
				players[game.whose_turn].hp = players[game.whose_turn].maxhp;
				players[game.whose_turn].player_state = STATE_IDLE;
			} else {
				--players[game.whose_turn].revive;
				players[game.whose_turn].player_state = STATE_DEAD;
			}
			draw_player_sprite(&players[game.whose_turn], 0);
			wait_jiffies(60);
			clear_sprites(0xE0, 4);
			clear_sprites(0x170, 1);
		} else {
			move_player();
			switch (players[game.whose_turn].standing->type) {
				case TYPE_BONUS:
					draw_player_sprite(&players[game.whose_turn], 0);
					display_text_sprite(INDEX_BONUS, SIZE_BONUS, 160, 96);
					while (keyboard_get() != 0x20);
					i = roll_die();
					display_die_sprite(0x170, i, 200, 128 - 32 - 8);
					i *= (players[game.whose_turn].norma_level > 3) ? 3 : players[game.whose_turn].norma_level;
					players[game.whose_turn].stars += i;
					wait_jiffies(60);
					clear_sprites(0x170, 1);
					break;
				case TYPE_DROP:
					draw_player_sprite(&players[game.whose_turn], 0);
					display_text_sprite(INDEX_DROP, SIZE_DROP, 160, 96);
					while (keyboard_get() != 0x20);
					i = roll_die();
					display_die_sprite(0x170, i, 200, 128 - 32 - 8);
					i *= players[game.whose_turn].norma_level;
					if (i >= players[game.whose_turn].stars) {
						players[game.whose_turn].stars = 0;
					} else {
						players[game.whose_turn].stars -= i;
					}				
					wait_jiffies(60);
					clear_sprites(0x170, 1);
					break;
				case TYPE_WARP:
					clear_sprites(0xE0, 4);
					wait_jiffies(20);
					for (i = (rand_byte() & 0x1) + 1; i != 0; --i) {
						do {
								players[game.whose_turn].standing = players[game.whose_turn].standing->next[0];
						} while (players[game.whose_turn].standing->type != TYPE_WARP);
					}
					draw_players();
					wait_jiffies(50);
					break;
				case TYPE_HOME:
					if (players[game.whose_turn].norma_type == NORMA_STAR) {
						if (players[game.whose_turn].stars >= norma_stars[players[game.whose_turn].norma_level]) {
							++players[game.whose_turn].norma_level;
							players[game.whose_turn].player_state = STATE_NORMA;
							draw_players();
							wait_jiffies(45);
						}
					} else {
						if (players[game.whose_turn].wins >= norma_wins[players[game.whose_turn].norma_level]) {
							++players[game.whose_turn].norma_level;
							players[game.whose_turn].player_state = STATE_NORMA;
							draw_players();
							wait_jiffies(45);
						}
					}
					if (players[game.whose_turn].hp != 0 && players[game.whose_turn].hp < players[game.whose_turn].maxhp) {
						++players[game.whose_turn].hp;
						players[game.whose_turn].player_state = STATE_NORMA;
						draw_players();
						wait_jiffies(30);
					}
					players[game.whose_turn].player_state = STATE_IDLE;
					draw_players();
					
					if (!player_has_reached_level_4 && players[game.whose_turn].norma_level == 4) {
						/* spawn a boss */
						new_enemy(1);
					}
					break;
				case TYPE_ENEMY:
					if (players[game.whose_turn].hp == 0) {
						break;
					}
					if (!enemy_is_alive) {
						new_enemy(0);
						enemy_is_alive = 1;
					}
					fight(&players[game.whose_turn], &enemy);
					break;
				case TYPE_DRAW:
					break;
			}
		}
		draw_gui();
		clear_sprites(0xE0, 4);
		game.whose_turn = (game.whose_turn + 1) % 4;
		if (game.whose_turn == 0) {
			for (i = 0; i < 4; i++) {
				++game.chapter;
				players[i].stars += game.chapter / 5 + 1;
			}
		}
	}
}

void move_player() {
	char i;
	
	draw_players();
	display_text_sprite(INDEX_CLICK, SIZE_CLICK, 160, 96);
	display_player_no(game.whose_turn + 1, 160, 96);
	
	i = 0;
	while (keyboard_get() != 0x20) {
			wait_jiffies(30);
			if (i == STATE_IDLE) {
				i = STATE_ROLL;
			} else {
				i = STATE_IDLE;
			}
			players[game.whose_turn].player_state = i;
			draw_player_sprite(&players[game.whose_turn], 0);
	}
	players[game.whose_turn].player_state = STATE_IDLE;
	draw_player_sprite(&players[game.whose_turn], 0);
	
	steps = roll_die();
	while (steps) {
		display_number_sprite(steps);
		if (players[game.whose_turn].standing->next[1] != NULL) {
			draw_next_arrows(players[game.whose_turn].standing);
			while (1) {
				key = keyboard_get();
				if (key == 0x31) {
					players[game.whose_turn].standing = players[game.whose_turn].standing->next[0];
					break;
				} else if (key == 0x32) {
					players[game.whose_turn].standing = players[game.whose_turn].standing->next[1];
					break;
				}
			}
			clear_sprites(0x130, 4);
		} else {
			wait_jiffies(10);
			players[game.whose_turn].standing = players[game.whose_turn].standing->next[0];
		}
		draw_players();
		--steps;
		for (i = 0; i < 4; ++i) {
			if (i == game.whose_turn) { continue; }
			if (players[i].standing == players[game.whose_turn].standing && players[i].hp > 0) {
				waitforjiffy();
				display_text_sprite(INDEX_ATTACK, SIZE_ATTACK, 160, 96);
				while (1) {
					key = keyboard_get();
					if (key == 0x31) {
						fight(&players[game.whose_turn], &players[i]);
						return;
					} else if (key == 0x32) {
						break;
					}
				}
				break;
			}
		}
		if (steps != 0) {
			if (players[game.whose_turn].standing == players[game.whose_turn].home_tile) {
				waitforjiffy();
				display_text_sprite(INDEX_STOP, SIZE_STOP, 160, 96);
				while (1) {
					key = keyboard_get();
					if (key == 0x31) {
						return;
					} else if (key == 0x32) {
						break;
					}
				}
			}
		}
	}
	display_number_sprite(0);
	wait_jiffies(30);
}

void center_around_player(struct player *p) {
	static struct board_tile *p_standing;
	unsigned char act_x;
	p_standing = p->standing;	
	act_x	= (p_standing->y > p_standing->x) ? 6 - (p_standing->y - p_standing->x) : 6 + (p_standing->x - p_standing->y);
	
	VERA.layer1.hscroll = 64 * act_x + 32 - 160;
	VERA.layer1.vscroll = 32 * (p_standing->x + p_standing->y) + 16 - 120;
	
	VERA.display.hscale = 64;
	VERA.display.vscale = 64;
}

void draw_players() {
	char i;
	
	center_around_player(&(players[game.whose_turn]));
	draw_norma_signs();
	draw_player_sprite(&players[game.whose_turn], 0);
	for (i = 0; i < 4; ++i) {
			if (i == game.whose_turn) {
				POKEW(0x9F20, 0xFD06 + ((i + 1) << 3));
				POKE(0x9F22, 0x11);
				__asm__ ("stz $9F23");
				continue;
			}
			draw_player_sprite(&(players[i]), i + 1);
	}
}

char bubble_sprite_hflip = 0;

void display_bubble_sprite(short x, char y) {
	POKE(0x9F23, 128 + 10);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, x);
	POKE(0x9F23, x >> 8);
	POKE(0x9F23, y);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC | bubble_sprite_hflip);
	POKE(0x9F23, 0xF1);
}

void display_number_sprite(char num) {
	POKEW(0x9F20, 0xFCF0);
	POKE(0x9F22, 0x11);
	
	POKE(0x9F23, 128 + num);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, 160 - 4);
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 36);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 0x01);
	
	display_bubble_sprite(160 - 32, 128 - 64);
}

char half_sprite_lengths[] = {4, 8, 16, 32};

void display_die_sprite(short offset, char roll, char x, char y) {
	POKEW(0x9F20, 0xFC00 + offset);
	POKE(0x9F22, 0x11);
	
	POKE(0x9F23, INDEX_DICE_1 + (roll - 1) * 4);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, x);
	POKE(0x9F23, 0);
	POKE(0x9F23, y);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, SIZE_DICE | 1);
}

void draw_next_arrows(struct board_tile *tile) {
	static char hflip, vflip;
	static short x, y;
	static char i;
	
	POKEW(0x9F20, 0xFD30);
	POKE(0x9F22, 0x11);
	for (i = 0; i < 2; ++i) {		
		hflip = get_act_x(tile->next[i]->x, tile->next[i]->y) > get_act_x(tile->x, tile->y) ? 1 : 0;
		vflip = (tile->next[i]->x + tile->next[i]->y) > (tile->x + tile->y) ? 0 : 2;
		
		x = 128;
		y = 168;
		
		if (hflip) { x += 64; }
		if (!vflip) { y += 32; }
		
		x = x - 4;
		y = y - 4;
		POKE(0x9F23, 129 + i);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, x);
		POKE(0x9F23, x >> 8);
		POKE(0x9F23, y);
		POKE(0x9F23, y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, 0x01);
		
		x = x - 4;
		y = y - 4;
		POKE(0x9F23, INDEX_ARROW);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, x);
		POKE(0x9F23, x >> 8);
		POKE(0x9F23, y);
		POKE(0x9F23, y >> 8);
		POKE(0x9F23, 0xC | hflip | vflip);
		POKE(0x9F23, SIZE_ARROW | 2);
	}
}

void clear_sprites(short offset, char num) {
	POKEW(0x9F20, 0xFC06 + offset);
	POKE(0x9F22, 0x41);
	
	while (num > 0) {
		__asm__ ("stz $9F23");
		--num;
	}
}

short display_text_custom_offset;

void display_text_sprite(short index, char size, short x, char y) {
	short temp;
	
	if (x & 0xF000) { 
		POKEW(0x9F20, 0xFC00 + display_text_custom_offset);
	} else {
		POKEW(0x9F20, 0xFCF0);
	}
	POKE(0x9F22, 0x11);
	
	x = x & 0x0FFF;
	temp = x - half_sprite_lengths[(size >> 4) & 0x3];
	
	POKE(0x9F23, index);
	POKE(0x9F23, 0x6 + (index >> 8));
	POKE(0x9F23, temp);
	POKE(0x9F23, temp >> 8);
	POKE(0x9F23, y - half_sprite_lengths[size >> 6]);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, size | 1);
	
	display_bubble_sprite(x - 32, y - 32);
}

void display_player_no(char pnum, short x, char y) {

	POKEW(0x9F20, 0xFCE0);
	POKE(0x9F22, 0x11);
		
	POKE(0x9F23, INDEX_P_PLAYER);
	POKE(0x9F23, 0x7);
	POKE(0x9F23, x - 8);
	POKE(0x9F23, 0);
	POKE(0x9F23, y - 16);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 0x01);
	
	POKE(0x9F23, 128 + pnum);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, x);
	POKE(0x9F23, 0);
	POKE(0x9F23, y - 16);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 0x01);
}

void draw_player_sprite(struct player *p, char sprite_table_index) {
	short val, val2;
	struct board_tile *p_standing;
	
	p_standing = p->standing;
	
	POKEW(0x9F20, 0xFD00 + (sprite_table_index << 3));
	POKE(0x9F22, 0x11);
	
	val = (p->player_num * 6 + p->player_state) << 6 /* times 64 */;
	POKE(0x9F23, val);
	POKE(0x9F23, 8 | (val >> 8));
	
	if (VERA.display.hscale == 64) {
		val = get_act_x(players[game.whose_turn].standing->x,players[game.whose_turn].standing->y); // player centered around 
		val2 = get_act_x(p_standing->x, p_standing->y); // current player drawing 
		if (val > val2) {
			val = 128 - 64 * (val - val2);
		} else {
			val = 128 + 64 * (val2 - val);
		}
		POKE(0x9F23, val);
		POKE(0x9F23, val >> 8);
		
		val = players[game.whose_turn].standing->x + players[game.whose_turn].standing->y; // player centered around 
		val2 = p_standing->x + p_standing->y; // current player drawing 
		if (val > val2) {
			val = 128 - 32 * (val - val2);
		} else {
			val = 136 + 32 * (val2 - val);
		}
		POKE(0x9F23, val);
		POKE(0x9F23, val >> 8);	
	} else if (VERA.display.hscale == 128) {
		val = (p_standing->y > p_standing->x) ? (6 - (p_standing->y - p_standing->x)) : (6 + (p_standing->x - p_standing->y));
		val = val * 64;
		POKE(0x9F23, val);
		POKE(0x9F23, val >> 8);
		val = 32 * (p_standing->x + p_standing->y) + 28;
		POKE(0x9F23, val);
		POKE(0x9F23, val >> 8);	
		
	}
	if (VERA.display.hscale == 64 || VERA.display.hscale == 128) {
		POKE(0x9F23, 0xC | (
		sabs(p_standing->next[0]->x, p_standing->next[0]->y) < sabs(p_standing->x, p_standing->y) != (p_standing->x >= p_standing->y)
		));
	} else {
		POKE(0x9F23, 0xC);
	}
	POKE(0x9F23, 0xFB + p->player_num);
}

void draw_norma_signs() {
	char i;
	short player_x = get_act_x(players[game.whose_turn].standing->x, players[game.whose_turn].standing->y);
	short player_y = players[game.whose_turn].standing->x + players[game.whose_turn].standing->y;
	
	POKEW(0x9F20, 0xFD50);
	POKE(0x9F22, 0x11);
	for (i = 0; i < 4; ++i) {
		short display_x = get_act_x(players[i].home_tile->x, players[i].home_tile->y);
		short display_y = (players[i].home_tile->x + players[i].home_tile->y);
		
		if (player_x >= display_x) {
			display_x = 128 - (player_x - display_x) * 64;
		} else {
			display_x = 128 + (display_x - player_x) * 64;
		}
		if (player_y >= display_y) {
			display_y = 128 - (player_y - display_y) * 32;
		} else {
			display_y = 128 + (display_y - player_y) * 32;
		}
		
		POKEW(0x02 + 4 * i, display_x);
		POKEW(0x04 + 4 * i, display_y);
		
		POKE(0x9F23, INDEX_SIGN);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, display_x);
		POKE(0x9F23, display_x >> 8);
		POKE(0x9F23, display_y);
		POKE(0x9F23, display_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, SIZE_SIGN | (i + 3));
	}
}

short gui_x_offsets[] = {0, 256, 0, 256};
char gui_y_offsets[] = {0, 0, 176, 176};

short gui_base_x;
short gui_base_y;

void set_base_gui_x_y(char i) {
	gui_base_x = i % 2 ? 280 : 8;
	gui_base_y = i >= 2 ? 200 : 8;
}

void draw_gui() {
	char i;
	
	POKEW(0x9F20, 0xFC00);
	POKE(0x9F22, 0x11);
	
	for (i = 0; i < 4; ++i) {
		char stars_hundred = players[i].stars;
		char stars_ten = (stars_hundred % 100) / 10;
		char stars_ones = stars_hundred % 10;
		stars_hundred = stars_hundred / 100;
		
		set_base_gui_x_y(i);
		
		POKE(0x9F23, INDEX_STAR);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, gui_base_x);
		POKE(0x9F23, gui_base_x >> 8);
		POKE(0x9F23, gui_base_y);
		POKE(0x9F23, gui_base_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, SIZE_STAR | 3);
		gui_base_x += 12;
		if (stars_hundred > 0) {
			POKE(0x9F23, 128 + stars_hundred);
			POKE(0x9F23, 0x6);
			POKE(0x9F23, gui_base_x);
			POKE(0x9F23, gui_base_x >> 8);
			POKE(0x9F23, gui_base_y);
			POKE(0x9F23, gui_base_y >> 8);
			POKE(0x9F23, 0xC);
			POKE(0x9F23, 0x03);
			gui_base_x += 8;
		}
		if (stars_hundred > 0 || stars_ten > 0) {
			POKE(0x9F23, 128 + stars_ten);
			POKE(0x9F23, 0x6);
			POKE(0x9F23, gui_base_x);
			POKE(0x9F23, gui_base_x >> 8);
			POKE(0x9F23, gui_base_y);
			POKE(0x9F23, gui_base_y >> 8);
			POKE(0x9F23, 0xC);
			POKE(0x9F23, 0x03);
			gui_base_x += 8;
		}
		POKE(0x9F23, 128 + stars_ones);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, gui_base_x);
		POKE(0x9F23, gui_base_x >> 8);
		POKE(0x9F23, gui_base_y);
		POKE(0x9F23, gui_base_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, 0x03);
		
		set_base_gui_x_y(i);
		gui_base_y += 9;
		gui_base_x += 20;
		stars_ten = players[i].wins;
		if (stars_ten >= 10) {
			POKE(0x9F23, 128 + stars_ten / 10);
			POKE(0x9F23, 0x6);
			POKE(0x9F23, gui_base_x);
			POKE(0x9F23, gui_base_x >> 8);
			POKE(0x9F23, gui_base_y);
			POKE(0x9F23, gui_base_y >> 8);
			POKE(0x9F23, 0xC);
			POKE(0x9F23, 0x03);
			gui_base_x += 8;
		}
		POKE(0x9F23, 128 + stars_ten % 10);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, gui_base_x);
		POKE(0x9F23, gui_base_x >> 8);
		POKE(0x9F23, gui_base_y);
		POKE(0x9F23, gui_base_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, 0x03);		
		
		set_base_gui_x_y(i);
		gui_base_y += 17;		
		gui_base_x += 20;		
		POKE(0x9F23, 128 + players[i].norma_level);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, gui_base_x);
		POKE(0x9F23, gui_base_x >> 8);
		POKE(0x9F23, gui_base_y);
		POKE(0x9F23, gui_base_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, 0x03);
		
		set_base_gui_x_y(i);
		gui_base_y += 25;		
		gui_base_x += 14;
		POKE(0x9F23, 128 + players[i].hp);
		POKE(0x9F23, 0x6);
		POKE(0x9F23, gui_base_x);
		POKE(0x9F23, gui_base_x >> 8);
		POKE(0x9F23, gui_base_y);
		POKE(0x9F23, gui_base_y >> 8);
		POKE(0x9F23, 0xC);
		POKE(0x9F23, 0x03);
		
		
		POKE(0x9F23, INDEX_CIRCLE);
		POKE(0x9F23, 0x7);
		POKE(0x9F23, gui_x_offsets[i]);
		POKE(0x9F23, gui_x_offsets[i] >> 8);
		POKE(0x9F23, gui_y_offsets[i]);
		POKE(0x9F23, 0);
		POKE(0x9F23, 0xC | (i >= 2 ? 0 : 2) | (i % 2));
		POKE(0x9F23, 0xF3 + i);
	}
}

void wait_jiffies(char num) {
	while (num) {
		waitforjiffy();
		--num;
	}
}

void zoom_out() {
	VERA.display.hscale = 128;
	VERA.display.vscale = 128;
	
	VERA.layer1.hscroll = 0;
	VERA.layer1.vscroll = 0;
}

extern void draw_horiz_band();

void fight(struct player *attacker, struct player *defender) {
	char i;
	
	clear_sprites(0xE0, 2);
	clear_sprites(0xF0, 2);
	wait_jiffies(30);
	i = 0;
	POKEW(0x9F20, 0xFD0E);
	POKE(0x9F22, 0x41);
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	
	POKEW(0x9F20, 0xFD56);
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	__asm__ ("stz $9F23");
	
	draw_horiz_band();
	attack(attacker, defender, 1);
	if (defender->hp > 0) { attack(defender, attacker, 0); }
	
	wait_jiffies(45);
	clear_sprites(0x128, 1);
	clear_sprites(0x100, 1);
	clear_layer1();
	for (i = 0; i < 0x34; ++i) {
		plot_tile(&(game_tiles[i]));
	}
}

char draw_attack_pos_x_offset;
char draw_attack_neg_x_offset;

void draw_player_attack(struct player *p, char state, char right_side) {
	short val = (p->player_num * 6 + state) * 64;
	POKE(0x9F23, val);
	POKE(0x9F23, 8 | (val >> 8));
	POKE(0x9F23, (right_side ? (320 - 80 - 64) : 80) + draw_attack_pos_x_offset - draw_attack_neg_x_offset);
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 32);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC | (1 ^ right_side));
	POKE(0x9F23, 0xFB + p->player_num);
	
	if (right_side) {
		POKEW(0x9F20, 0xFD80);
		draw_hp_sprites(p, right_side);
	} else {
		POKEW(0x9F20, 0xFDC0);
		draw_hp_sprites(p, right_side);
	}
}

void draw_hp_sprites(struct player *p, char right_side) {
	short temp;
	
	POKE(0x9F23, INDEX_HP);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, (right_side ? (320 - 64 - 4) : 74));
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 31);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, SIZE_HP | 1);
	
	POKE(0x9F23, 128 + p->hp);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, (right_side ? (320 - 74 - 4) : 64));
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 30);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, (p->attack + p->attack_modifier >= 6) ? INDEX_PLUS : INDEX_MINUS);
	POKE(0x9F23, 0x7);
	temp = (right_side ? (320 - 40 - 4) : 30);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 - 20);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, 128 + sabs(p->attack + p->attack_modifier, 6));
	POKE(0x9F23, 0x6);
	temp = (right_side ? (320 - 30 - 4) : 40);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 - 20);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, (p->defend + p->defend_modifier >= 6) ? INDEX_PLUS : INDEX_MINUS);
	POKE(0x9F23, 0x7);
	temp = (right_side ? (320 - 40 - 4) : 30);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 - 8);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, 128 + sabs(p->defend + p->defend_modifier, 6));
	POKE(0x9F23, 0x6);
	temp = (right_side ? (320 - 30 - 4) : 40);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 - 8);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, (p->evade + p->evade_modifier >= 6) ? INDEX_PLUS : INDEX_MINUS);
	POKE(0x9F23, 0x7);
	temp = (right_side ? (320 - 40 - 4) : 30);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 + 4);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
	POKE(0x9F23, 128 + sabs(p->evade + p->evade_modifier, 6));
	POKE(0x9F23, 0x6);
	temp = (right_side ? (320 - 30 - 4) : 40);
	__asm__ ("lda %v", temp);
	__asm__ ("sta $9F23");
	__asm__ ("lda %v + 1", temp);
	__asm__ ("sta $9F23");
	POKE(0x9F23, 128 + 4);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 1);
	
}


void attack(struct player *attacker, struct player *defender, char attacker_on_left) {
	short damage; 
	char attack_roll;
	char defend_evade_roll;
	char choice;
	
	draw_attack_neg_x_offset = 0;
	draw_attack_pos_x_offset = 0;
	/* draw defender */
	POKE(0x9F22, 0x11);
	POKEW(0x9F20, 0xFD28);
	draw_player_attack(defender, STATE_IDLE, attacker_on_left);
	
	if (attacker->player_num == 4) {
		/* draw attacker */
		POKEW(0x9F20, 0xFD00);
		POKE(0x9F22, 0x11);
		draw_player_attack(attacker, STATE_ROLL, 1 ^ attacker_on_left);
		wait_jiffies(60);
	} else {
		attacker->player_state = STATE_ROLL;
		bubble_sprite_hflip = attacker_on_left ^ 1;
		display_text_sprite(INDEX_ATTACK, SIZE_ATTACK, attacker_on_left ? (160 - 24) : (160 + 24), 96);
		bubble_sprite_hflip = 0;
		while (1) {
			/* draw attacker */
			POKEW(0x9F20, 0xFD00);
			POKE(0x9F22, 0x11);
			draw_player_attack(attacker, attacker->player_state, 1 ^ attacker_on_left);
			wait_jiffies(30);
			if (keyboard_get() == 0x20) { break; }
			attacker->player_state = attacker->player_state == STATE_IDLE ? STATE_ROLL : STATE_IDLE;
		}
		clear_sprites(0xF0, 2);
	}
	attacker->player_state = STATE_ATTACK;
	POKEW(0x9F20, 0xFD00);
	POKE(0x9F22, 0x11);
	draw_player_attack(attacker, attacker->player_state, 1 ^ attacker_on_left);
	attack_roll = roll_die();
	display_die_sprite(0x170, attack_roll, attacker_on_left ? 128 : (320 - 128 - 8), 128 - 32 - 8);
	attack_roll = attack_roll + attacker->attack + attacker->attack_modifier - 6;
	if (attack_roll >= 128) { attack_roll = 0; }
	wait_jiffies(20);
	POKEW(0x9F20, 0xFD70);
	POKE(0x9F23, 128 + attack_roll);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, attacker_on_left ? 132 : (320 - 132 - 4));
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 32 + 4);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 0x1);
	wait_jiffies(20);
	
	display_text_custom_offset = 0x130;
	display_text_sprite(INDEX_DEFEND, SIZE_DEFEND, 0xF000 | (160 - 96), 128 + 32 + 8);	
	display_text_custom_offset = 0x140;
	display_text_sprite(INDEX_EVADE, SIZE_EVADE, 0xF000 | (160 + 96), 128 + 32 + 8);
	if (defender->player_num == 4) {
		if (defender->hp == 1 || attack_roll == 1) {
			choice = ACTION_EVADE;
		} else {
			choice = rand_byte() & 1;
		}
		clear_sprites(0x130 + (choice == ACTION_DEFEND ? 0x10 : 0), 2);
		wait_jiffies(60);
	} else {
		keyboard_get();
		while (1) {
			choice = keyboard_get();
			if (choice == 0x31) {
				choice = ACTION_DEFEND;
				break;
			} else if (choice == 0x32) {
				choice = ACTION_EVADE;
				break;
			}
			waitforjiffy();
		}
		clear_sprites(0x130 + (choice == ACTION_DEFEND ? 0x10 : 0), 2);
		wait_jiffies(30);
		defender->player_state = STATE_ROLL;
		while (keyboard_get() != 0x20) {
			POKE(0x9F22, 0x11);
			POKEW(0x9F20, 0xFD28);
			draw_player_attack(defender, defender->player_state, attacker_on_left);
			defender->player_state = defender->player_state == STATE_IDLE ? STATE_ROLL : STATE_IDLE;
			wait_jiffies(30);
		}
		defender->player_state = STATE_IDLE;
	}
	defend_evade_roll = roll_die();
	display_die_sprite(0x178, defend_evade_roll, !attacker_on_left ? 128 : (320 - 128 - 8), 128 - 32 - 8);
	if (choice == ACTION_DEFEND) {
		defend_evade_roll = defend_evade_roll + defender->defend + defender->defend_modifier - 6;
	} else {
		defend_evade_roll = defend_evade_roll + defender->evade + defender->evade_modifier - 6;
	}
	if (defend_evade_roll >= 128) { defend_evade_roll = 0; } 
	wait_jiffies(20);
	POKEW(0x9F20, 0xFD78);
	POKE(0x9F23, 128 + defend_evade_roll);
	POKE(0x9F23, 0x6);
	POKE(0x9F23, attacker_on_left ? (320 - 132 - 4) : 132);
	POKE(0x9F23, 0);
	POKE(0x9F23, 128 - 32 + 4);
	POKE(0x9F23, 0);
	POKE(0x9F23, 0xC);
	POKE(0x9F23, 0x1);
	if (choice == ACTION_DEFEND) {
		if (defend_evade_roll >= attack_roll) {
			damage = 1;
		} else {
			damage = attack_roll - defend_evade_roll;
		}
	} else {
		damage = defend_evade_roll > attack_roll ? 0 : attack_roll;
	}
	if (damage == 0) {
		defender->player_state = STATE_IDLE;
		if (attacker_on_left) {
			draw_attack_pos_x_offset = 12;
		} else {
			draw_attack_neg_x_offset = 12;
		}
	} else {
		defender->player_state = STATE_DAMAGED;
		if (damage >= defender->hp) {
			defender->hp = 0;
			if (attacker->player_num != 4) {
				if ((defender->player_num != 4 && defender->character_index != PLAYER_CHICKEN && defender->character_index != PLAYER_ROBOBALL && 		defender->character_index != PLAYER_SEAGULL) | defender->character_index >= 0xE3) {
					attacker->wins += 2;
				} else {
					++attacker->wins;
				}
				if (defender->player_num == 4 && defender->character_index <= 0xE3 /* not a boss */) {
					attacker->stars += defender->stars;
					enemy_is_alive = 0;
				} else if ( defender->character_index != PLAYER_CHICKEN && defender->character_index != PLAYER_ROBOBALL && defender->character_index != PLAYER_SEAGULL) {
					damage = defender->stars >> 1;
					attacker->stars += damage;
					defender->stars -= damage;
				} else {
					damage = defender->stars >> 2;
					attacker->stars += damage;
					defender->stars -= damage;
				}
			}
		} else {
			defender->hp -= damage;
			
		}
	}
	
	POKEW(0x9F20, 0xFD28);
	POKE(0x9F22, 0x11);
	draw_player_attack(defender, defender->player_state, attacker_on_left);
	wait_jiffies(12);
	draw_attack_neg_x_offset = 0;
	draw_attack_pos_x_offset = 0;
	wait_jiffies(18);
	if (defender->hp == 0) {
		defender->player_state = STATE_DEAD;
		defender->revive = defender->base_revive;
	}
	POKEW(0x9F20, 0xFD28);
	POKE(0x9F22, 0x11);
	draw_player_attack(defender, defender->player_state, attacker_on_left);
	wait_jiffies(30);
	
	attacker->player_state = STATE_IDLE;
	if (defender->hp != 0) {
			defender->player_state = STATE_IDLE;
	}
	clear_sprites(0x130, 4);
	clear_sprites(0x170, 2);
	clear_sprites(0x180, 8);
	clear_sprites(0x1C0, 8);
}


void parse_map_data(char *data, char num_tiles) {
	static char i, temp;
	
	memset(game_tiles, 0, 255 * sizeof(struct board_tile));
	memset(norma_tiles, 0, 4 * sizeof(void *)); 
	
	for (i = 0; i < num_tiles; ++i) {
		static unsigned short index; 
		
		index = i * 6;
		game_tiles[i].type = data[index + 0];
		if (game_tiles[i].type == TYPE_HOME) {
			char j;
			for (j = rand_byte() & 0x3; norma_tiles[j] != NULL; j = (j + 1) % 4);
			norma_tiles[j] = &(game_tiles[i]);
		}
		game_tiles[i].x = data[index + 1];
		game_tiles[i].y = data[index + 2];
				
		temp = data[index + 3];
		game_tiles[i].next[0] = &(game_tiles[temp]);
		temp = data[index + 4];
		if (temp != 0xFF) {
			game_tiles[i].next[1] = &(game_tiles[temp]);
			
			temp = data[index + 5];
			if (temp != 0xFF) {
				game_tiles[i].next[2] = &(game_tiles[temp]);
			} else {
				game_tiles[i].next[2] = NULL;
			}
		} else {
			game_tiles[i].next[1] = NULL;
			game_tiles[i].next[2] = NULL;
		}
	}
	for (i = 0; i < num_tiles; ++i) {		
		for (temp = 0; game_tiles[i].next[temp] != NULL; ++temp) {
			if (game_tiles[i].x == game_tiles[i].next[temp]->x) {
				if (game_tiles[i].y < game_tiles[i].next[temp]->y) {
					game_tiles[i].has_neighbor_sw = 1;
					game_tiles[i].next[temp]->has_neighbor_ne = 1;
				} else {
					game_tiles[i].has_neighbor_ne = 1;
					game_tiles[i].next[temp]->has_neighbor_sw = 1;
				}
			} else {
				if (game_tiles[i].x < game_tiles[i].next[temp]->x) {
					game_tiles[i].has_neighbor_se = 1;
					game_tiles[i].next[temp]->has_neighbor_nw = 1;
				} else {
					game_tiles[i].has_neighbor_nw = 1;
					game_tiles[i].next[temp]->has_neighbor_se = 1;
				}
			}
		}
	}
}

#define LOAD_ADDRESS 0xA000
#define TILES_CHR_FILELEN 2048
#define CHARACTER_FILELEN 12288

void character_menu() {
	char i;
	char selected;
	
	VERA.layer0.hscroll = 0; // Reset scroll
	VERA.layer0.vscroll = 0;
	VERA.layer1.hscroll = 0;
	VERA.layer1.vscroll = 0;	
	VERA.layer1.mapbase = 0;
	
	clear_layer1_blue();
	POKEW(0x9F20, 0x0102);
	POKE(0x9F22, 0x10);
	print_str("player 1, choose a character:");
	__asm__ ("inc $9F21");
	for (i = 0; i < SETUP_CHARACTERS_LEN; ++i) {		
		__asm__ ("inc $9F21");
		POKE(0x9F20, 6);
		print_str(setup_characters[i]);
		
	}
	for (i = 0; i < 4; ++i) {		
		POKEW(0x9F20, 0x0110);
		POKE(0x9F22, 0x00);
		POKE(0x9F23, '1' + i);
		
		POKEW(0x9F20, 0x0302);
		POKE(0x9F23, '>');
		selected = 0;
		while (1) {
			key = keyboard_get();
			if (key == 0x91 /* UP */ && selected != 0) {
				--selected;
				POKE(0x9F23, 0x20);
				__asm__ ("dec $9F21");
				POKE(0x9F23, '>');
			} else if (key == 0x11 /* DOWN */ && selected < SETUP_CHARACTERS_LEN - 1) {
				++selected;
				POKE(0x9F23, 0x20);
				__asm__ ("inc $9F21");
				POKE(0x9F23, '>');
			} else if (key == 0x0d) {
				POKE(0x9F23, 0x20);
				break;
			}
		}
		players[i].character_index = selected;
		players[i].player_num = i;
	}
	
	load_character_data();
	load_enemies_into_hiram();
}

void setup_video() {	
	VERA.control = 0;
	VERA.display.video = 0x71 | (VERA.display.video & 3); // don't want layer 0 right now 
	
	VERA.display.hscale = 128;
	VERA.display.vscale = 128;
	
	VERA.layer0.hscroll = 0; // Reset scroll
	VERA.layer0.vscroll = 0;
	VERA.layer1.hscroll = 0;
	VERA.layer1.vscroll = 0;	
		
	VERA.layer1.config = 0xA2;
	VERA.layer1.mapbase = 0; // map_base @ $00000 in VRAM 
	VERA.layer1.tilebase = 0x7D; // tilemap @ $08800 in VRAM 
	
	clear_layer1();
	
	cbm_k_setnam("tiles.chr");
	cbm_k_setlfs(0xFF, DEVICE_NUM, 2);
	RAM_BANK = 1;
	cbm_k_load(0, LOAD_ADDRESS);
	POKEW(0x2, 0xA000);
	POKEW(0x4, 0x9F23);
	POKEW(0x6, TILES_CHR_FILELEN);
	
	POKEW(0x9F20, 0xF800);
	POKE(0x9F22, 0x10);
	__asm__("jsr $FEE7");
	
	cbm_k_setnam("sprites.chr");
	cbm_k_setlfs(0xFF, DEVICE_NUM, 2);
	RAM_BANK = 1;
	cbm_k_load(0, LOAD_ADDRESS);
	
	POKEW(0x9F20, 0xD000);
	POKE(0x9F22, 0x10);
	
	RAM_BANK = 1;
	POKEW(0x6, 0x2000);
	__asm__ ("jsr $FEE7");
	
	
	RAM_BANK = 2;
	POKEW(0x6, 0x0800);
	__asm__ ("jsr $FEE7");	
}

char load_filename_string[32];

void load_character_data() {
	char i, j;
	
	POKEW(0x4, 0x9F23);
	POKEW(0x9F20, 0x0000);
	for (i = 0; i < 4; ++i) {
		strcpy(load_filename_string, setup_characters[players[i].character_index]);
		strcat(load_filename_string, ".chr");
		
		cbm_k_setnam(load_filename_string);
		cbm_k_setlfs(0xFF, DEVICE_NUM, 2);
		POKE(0x00, 1);
		cbm_k_load(0, LOAD_ADDRESS);
		
		POKE(0x00, 1);
		
		POKEW(0x9F20, 0xFB60 + (i << 5));
		POKE(0x9F22, 0x11);
		for (j = 0; j < 32; ++j) {
			POKE(0x9F23, ((char *)0xA000)[j]);
		}
		
		POKE(0x9F22, 0x11);
		POKEW(0x9F20, CHARACTER_FILELEN * i);
		
		POKEW(0x2, 0xA020);
		POKEW(0x06, 0x1FE0);
		POKE(0x00, 1);
		__asm__("jsr $FEE7");
		POKE(0x00, 2);
		POKEW(0x2, 0xA000);
		POKEW(0x06, CHARACTER_FILELEN - 0x1FE0);
		__asm__("jsr $FEE7");
		
		init_player_properties(&players[i] , player_data_table[players[i].character_index]);
	}
}

void init_player_properties(struct player *player, char *data) {
	player->attack = data[0];
	player->defend = data[1];
	player->evade = data[2];
	
	player->maxhp = data[3];
	player->hp = data[3];
	
	player->base_revive = data[4];
	
	player->attack_modifier = 3;
	player->defend_modifier = 3;
	player->evade_modifier = 3;
	
	player->stars = 0;
	player->wins = 0;
	player->norma_level = 1;
	player->norma_type = NORMA_STAR;
}

char enemy_properties_table[] = {
	2, 2, 4, 3,
	4, 2, 2, 3,
	2, 4, 2, 3,
	5, 4, 0, 10,
	5, 6, 1, 7,
	6, 5, 2, 8,
};

void new_enemy(char is_boss) {
	unsigned char index = rand() % 3;
	
	POKEW(0x02, index);
	if (is_boss) { index += 3; }
	load_enemy_vram(index);
	
	init_player_properties(&enemy, enemy_properties_table + (index * 4));
	enemy.player_num = 4;
	enemy.character_index = 0xE0 + index;
	if (is_boss) {
		enemy.stars = 50;
	} else {
		enemy.stars = 3;
	}
}

char enemy_names[][16] = {
	"chicken.chr",
	"seagull.chr",
	"roboball.chr",
	"castle.chr",
	"shifurobot.chr",
	"storemanager.chr"
};

void load_enemies_into_hiram() {
	char i;
	
	for (i = 0; i < 6; ++i) {
		cbm_k_setnam(enemy_names[i]);
		cbm_k_setlfs(0xFF, DEVICE_NUM, 2);
		POKE(0x00, 4 + i * 2);
		cbm_k_load(0, LOAD_ADDRESS);
	}
}

void load_enemy_vram(char enemy_index) {
	char i;
	
	POKEW(0x9F20, 0xFBE0);
	POKE(0x9F22, 0x11);
	POKE(0x00, 4 + enemy_index * 2);
	for (i = 0; i < 32; ++i) {
		POKE(0x9F23, ((char *)0xA000)[i]);
	}
	POKE(0x9F22, 0x11);
	POKEW(0x9F20, 0xC000);
	
	POKEW(0x04, 0x9F23);
	POKEW(0x02, 0xA020);
	POKEW(0x06, 0x1FE0);
	__asm__("jsr $FEE7");
	POKE(0x00, 5 + enemy_index * 2);
	POKEW(0x02, 0xA000);
	POKEW(0x06, CHARACTER_FILELEN - 0x1FE0);
	__asm__("jsr $FEE7");
}

char get_act_x(char tile_x, char tile_y) {
	return 6 + tile_x - tile_y;
}

char base_palettes[][32] = {
	{
	0x00, 0x00, // transparent color
	0xff, 0x0f, //#ffffff white
	0xf0, 0x00, //#00ff00 green
	0x6c, 0x00, //#0066cc blue
	0x06, 0x06, //#660066 purple
	0xc0, 0x0f, //#ffcc00 yellow
	0x00, 0x0f, //#ff0000 red
	0xaa, 0x0a, //#aaaaaa light gray
	0x50, 0x0c, //#ffaa00 orange
	},
	{0x00, 0x00, 0xFF, 0x0F, 0xDD, 0x0E, 0xBB, 0x0C, 0xBA, 0x0D, 0xB9, 0x0C, 0xCA, 0x0D, 0x97, 0x0B, 0xED, 0x0F, 0xDB, 0x0E, 0x99, 0x0A, 0xFE, 0x0F, 0xBA, 0x0B, 0x75, 0x08, 0x77, 0x08, 0x00, 0x00},
	{0x00, 0x00, 0x9B, 0x08, 0xAB, 0x09, 0xBD, 0x09, 0xDE, 0x0C, 0x8A, 0x06, 0xEF, 0x0E, 0xFF, 0x0F, 0xFF, 0x0E, 0xDD, 0x0C, 0xDE, 0x0D, 0xAE, 0x08, 0xEF, 0x0D, 0xCE, 0x0A, 0xDF, 0x0B},
	
	{0x00, 0x00, 0xCC, 0x0C, 0xDD, 0x0D, 0xFE, 0x0F, 0xED, 0x0F, 0xCB, 0x0F, 0x99, 0x09, 0xB8, 0x0F, 0xCA, 0x0F, 0xA7, 0x0F, 0xB9, 0x0F, 0x84, 0x0F, 0x96, 0x0F, 0x77, 0x07, 0x54, 0x05, 0xFF, 0x0F}, /* player 1 palette */
  {0x00, 0x00, 0xCC, 0x0C, 0xDD, 0x0D, 0xFE, 0x0F, 0xFF, 0x0E, 0xEF, 0x0D, 0x99, 0x09, 0xEF, 0x0C, 0xED, 0x0E, 0xCF, 0x0A, 0xDF, 0x0B, 0xBF, 0x07, 0xCF, 0x09, 0x77, 0x07, 0x55, 0x05}, /* player 2 palette */
  {0x00, 0x00, 0xCC, 0x0C, 0xDD, 0x0D, 0xFE, 0x0F, 0xFE, 0x0E, 0xFD, 0x0D, 0x99, 0x09, 0xFB, 0x0B, 0xFC, 0x0C, 0xF9, 0x09, 0xFA, 0x0A, 0xF6, 0x06, 0xF7, 0x08, 0x77, 0x07, 0x55, 0x05}, /* player 3 palette */
  {0x00, 0x00, 0xCC, 0x0C, 0xDD, 0x0D, 0xFE, 0x0F, 0xFE, 0x0F, 0xFC, 0x0F, 0x99, 0x09, 0xEA, 0x0F, 0xFD, 0x0F, 0xE9, 0x0F, 0xE8, 0x0F, 0xD4, 0x0F, 0xE7, 0x0F, 0x77, 0x07, 0x55, 0x05}, /* player 4 palette */
};


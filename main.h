#ifndef _MAIN_H
#define _MAIN_H 

#include "structs.h"

void main();

void parse_map_data(char *data, char num_tiles);

void center_around_player(struct player *p);
void draw_players();
void draw_player_sprite(struct player *p, char sprite_table_index);

#define INDEX_CLICK 202
#define SIZE_CLICK 0x10

#define INDEX_STOP 204
#define SIZE_STOP 0x20

#define INDEX_BONUS 208
#define SIZE_BONUS 0x20

#define INDEX_DROP 212
#define SIZE_DROP 0x10

#define INDEX_DICE_1 214
#define INDEX_DICE_2 218
#define INDEX_DICE_3 222
#define INDEX_DICE_4 226
#define INDEX_DICE_5 230
#define INDEX_DICE_6 234
#define SIZE_DICE 0x50

#define INDEX_ARROW 238
#define SIZE_ARROW 0x50

#define INDEX_STAR 242
#define SIZE_STAR 0x90

#define INDEX_HP 248
#define SIZE_HP 0x10

#define INDEX_SIGN 250
#define SIZE_SIGN 0xF0

#define INDEX_CIRCLE 58
#define SIZE_CIRCLE 0xF0

#define INDEX_ATTACK (256 + 122)
#define SIZE_ATTACK 0x20

#define INDEX_DEFEND (256 + 126)
#define SIZE_DEFEND 0x20

#define INDEX_EVADE (256 + 130)
#define SIZE_EVADE 0x20

#define INDEX_REVIVE (256 + 134)
#define SIZE_REVIVE 0x20

extern short display_text_custom_offset;
void display_text_sprite(short, char size, short x, char y);

void display_die_sprite(short offset, char roll, char x, char y);

void display_bubble_sprite(short x, char y);
void display_number_sprite(char num);
void clear_number_sprite();
void clear_sprites(short offset, char num);

void draw_next_arrows(struct board_tile *tile); 

void draw_norma_signs();

void move_player();

void draw_gui();

void attack(struct player *attacker, struct player *defender, char attacker_on_left);
void fight(struct player *attacker, struct player *defender);

extern char draw_attack_pos_x_offset;
extern char draw_attack_neg_x_offset; 

void draw_player_attack(struct player *p, char state, char right_side);
void draw_hp_sprites(char hp, char right_side);

void wait_jiffies(char num);

void character_menu();
void load_character_data();

void init_player_properties(struct player *player, char *data);

extern char enemy_properties_table[];
extern char enemy_names[][16];

void new_enemy(char is_boss);
void load_enemies_into_hiram();
void load_enemy_vram(char enemy_index);

void setup_video();

char get_act_x(char tile_x, char tile_y);

extern char base_palettes[][32];

#endif
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
#define INDEX_DICE_2 222
#define INDEX_DICE_3 230
#define INDEX_DICE_4 238
#define INDEX_DICE_5 246
#define INDEX_DICE_6 252
#define SIZE_DICE 0x50

#define INDEX_ARROW 2



void display_text_sprite(short, char size);

void display_die_sprite(char offset, char roll);

void display_bubble_sprite();
void display_number_sprite(char num);
void clear_number_sprite();
void clear_sprite(short offset);

void move_player();

void draw_gui();

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
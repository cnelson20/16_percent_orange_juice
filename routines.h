#include "structs.h"

char __fastcall__ roll_die();
char __fastcall__ rand_byte();
char __fastcall__ sabs(char a, char b);
char __fastcall__ dig_to_ascii(char c);
char __fastcall__ keyboard_get();
void __fastcall__ print_str(char *str);

void __fastcall__ clear_layer1_blue();
void __fastcall__ clear_layer1();

void __fastcall__ plot_tile(struct board_tile *tile);

void __fastcall__ draw_horiz_band();
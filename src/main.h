#ifndef MAIN_H
#define MAIN_H

#include "types.h"

// Prototypes **********************************************
void reload_play_field(void);
void draw_shifted_char(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
void draw_sprites(byte y, byte *pBuf, GFX_OBJECT *pList, byte bCount);
void draw_play_field(background_game background);
void move_background_to(_Bool increment);
void move_background(void);
void adjust_play_field_rows(void);
void adjust_play_field_cols(void);
void plot_point(byte x, byte y);
void draw_to(int x, int y, int x1, int y1);
void plot_text(int x, int y, PGM_P s);
int stretch (int x);
void plot_big_text(int x, int y, PGM_P s);
void setup(void);
void loop(void);

#endif // MAIN_H

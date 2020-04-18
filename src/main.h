#ifndef MAIN_H
#define MAIN_H

#include "types.h"

// Prototypes **********************************************
void reload_play_field(void);
void draw_shifted_char(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
void draw_sprites(byte y, byte *pBuf, gfx_object *pList, byte bCount);
void draw_play_field(void);
void adjust_play_field_rows(void);
void adjust_play_field_cols(void);

void setup(void);
void loop(void);

// Framerate control
void init_frame_control(void);
void set_frame_duration(byte duration);
_Bool next_frame(void);
_Bool every_x_frame(byte frames);
void idle(void);

// Functions for game (move background and player)
void move_background_to(_Bool increment);
void move_background(void);
_Bool check_collision(void);
_Bool check_ground(void);

#endif // MAIN_H

#ifndef ENGINE_H
#define ENGINE_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"


// GLOBALS
// ////////////////////////////////////////////////////////////////////
byte each_frame_millis;
byte this_frame_start;
byte last_frame_duration_ms;
uint16_t frame_count;
_Bool just_rendered;

// PROTOTYPES
// ////////////////////////////////////////////////////////////////////
// FRAME CONTROL
void engine_init_frame_control(void);
void engine_set_frame_duration(byte duration);
_Bool engine_next_frame(void);
void engine_idle(void);
_Bool engine_every_x_frame(byte frames);

// BACKGROUND
void engine_background_init(background_game_ptr background);
const background_game *engine_background_get(void);
int engine_background_set(const background_game *new_background_config);
void engine_background_draw(background_game_ptr background);
void engine_background_reload(background_game_ptr background, map_game_ptr map);
void engine_background_adjust(background_game_ptr background, map_game_ptr map);
void engine_background_set_pos(uint16_t x, uint16_t y);

//void engine_shifter_char(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
//void engine_draw_sprites(byte y, byte *pBuf, gfx_object *pList, byte bCount);

#endif // ENGINE_H

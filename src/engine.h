#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"

// BACKGROUND
// ////////////////////////////////////////////////////////////////////
void engine_background_init(background_game_ptr background);
const background_game *engine_background_get(void);
int engine_background_set(const background_game *new_background_config);
void engine_background_draw(background_game_ptr background);
void engine_background_reload(background_game_ptr background);
void engine_background_adjust(background_game_ptr background);


void engine_shifter_char(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
void engine_draw_sprites(byte y, byte *pBuf, GFX_OBJECT *pList, byte bCount);

#endif // ENGINE_H

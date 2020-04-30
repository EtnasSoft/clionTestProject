#ifndef PLAYER_H
#define PLAYER_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"


// PROTOTYPES
// ////////////////////////////////////////////////////////////////////
void player_init(gfx_object_ptr player);
//const background_game *engine_background_get(void);
//int engine_background_set(const background_game *new_background_config);
void player_apply_gravity(gfx_object_ptr player);
void player_start_jump(gfx_object_ptr player);
void player_end_jump(gfx_object_ptr player);
void player_fix_ground_position(gfx_object_ptr player);
void player_reset_y(gfx_object_ptr player);
void player_move_to_up(gfx_object_ptr player);
void player_move_to_down(gfx_object_ptr player);
void player_move_to_left(gfx_object_ptr player);
void player_move_to_right(gfx_object_ptr player);
void player_update(gfx_object_ptr player);

#endif // PLAYER_H

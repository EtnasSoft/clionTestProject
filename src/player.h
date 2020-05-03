#ifndef PLAYER_H
#define PLAYER_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"


// PROTOTYPES
// ////////////////////////////////////////////////////////////////////
void player_init(sprite_object_ptr player);
//const background_game *engine_background_get(void);
//int engine_background_set(const background_game *new_background_config);
void player_apply_gravity(sprite_object_ptr player);
void player_start_jump(sprite_object_ptr player);
void player_end_jump(sprite_object_ptr player);
void player_fix_ground_position(sprite_object_ptr player);
void player_reset_y(sprite_object_ptr player);
void player_move_to_up(sprite_object_ptr player);
void player_move_to_down(sprite_object_ptr player);
void player_move_to_left(sprite_object_ptr player);
void player_move_to_right(sprite_object_ptr player);
void player_update(sprite_object_ptr player);

#endif // PLAYER_H

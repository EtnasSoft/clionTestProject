#include "player.h"

void player_init(sprite_object_ptr player) {
  player->bType = 0x80;
  player->width = 16;
  player->height = 16;
  player->x = 56;
  player->x_old = 56;
  player->x_offset = 56 & 7;
  player->x_page = 56 >> 3;
  player->y = 40;
  player->y_old = 40;
  player->y_offset = 40 & 7;
  player->y_page = 40 >> 3;

  // TODO: No se usan, pero sería interesante calcular aquí su posición dentro del background_data.
  //  player->x_main_grid_pos = 56 + 48;
  //  player->y_main_grid_pos = 40;

  player->x_speed = 8;
  player->y_speed = 2;
  player->gravity = 1;
  player->on_ground = 1;
  player->need_render = 1;
}

void player_start_jump(sprite_object_ptr player) {
  if (player->on_ground == 1 && player->is_jumping == 0) {
    player->is_jumping = 1;
    player->y_speed = -8; // Jump acceleration
    player->on_ground = 0;
  }
}

void player_end_jump(sprite_object_ptr player) {
  // Minimum jump height allowed
  if (player->y_speed >= -5) {
    player->is_jumping = 0;

    if (player->y_speed < 0) {
      player->y_speed = 0;
    }
  }
}

void player_fix_ground_position(sprite_object_ptr player) {
  player->y = player->y_page << 3;
  player->y_offset = player->y & 7;
  player->y_page = player->y >> 3;
  player->y_speed = 0;
  player->on_ground = 1;
}

void player_apply_gravity(sprite_object_ptr player) {
  // Euler simple integration
  player->y_speed += player->gravity;
  if (player->y_speed >= 8) player->y_speed = 8;

  player_move_to_up(player);
  if (player->y <= 0) player_reset_y(player);
}

void player_reset_y(sprite_object_ptr player) {
  player->y = 0;
  player->y_offset = 0;
  player->y_page = 0;
}

void player_move_to_up(sprite_object_ptr player) {
  player->y += player->y_speed;
  player->y_offset = player->y & 7;
  player->y_page = player->y >> 3;
}

void player_move_to_down(sprite_object_ptr player) {
  player->y -= player->y_speed;
  player->y_offset = player->y & 7;
  player->y_page = player->y >> 3;
}

void player_move_to_left(sprite_object_ptr player) {
  player->x -= player->x_speed;
  player->x_offset = player->x & 7;
  player->x_page = player->x >> 3;
}

void player_move_to_right(sprite_object_ptr player) {
  player->x += player->x_speed;
  player->x_offset = player->x & 7;
  player->x_page = player->x >> 3;
}

void player_update(sprite_object_ptr player) {
  if (player->x_old != player->x) {
    player->x_old = player->x;
    player->need_render = 1;
  }

  if (player->y_old != player->y) {
    player->y_old = player->y;
    player->need_render = 1;
  }
}

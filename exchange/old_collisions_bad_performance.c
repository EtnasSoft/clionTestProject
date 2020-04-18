_Bool check_collision_2() {
  int current_player_pos_y_in_grid = ((background.y + player->y) >> 3) * TILEMAP_WIDTH,
      current_player_pos_x_in_grid = ((background.x + player->x) >> 3) - 1,
      current_player_byte_pos_in_grid = current_player_pos_y_in_grid + current_player_pos_x_in_grid,
      tile_top_before = current_player_byte_pos_in_grid + 1,
      tile_top_after = current_player_byte_pos_in_grid + 2;

  int data_tile_top_before = pgm_read_byte(&map.data[tile_top_before]),
      data_tile_bottom_before = pgm_read_byte(&map.data[tile_top_before + TILEMAP_WIDTH]),
      data_tile_top_after = pgm_read_byte(&map.data[tile_top_after]),
      data_tile_bottom_after = pgm_read_byte(&map.data[tile_top_after + TILEMAP_WIDTH]);

  if (
      (data_tile_top_before == 1) ||
      (data_tile_bottom_before == 1) ||
      (data_tile_top_after == 1) ||
      (data_tile_bottom_after == 1) ||
      (player->on_ground == 0 && check_ground())
      ) {
    return 1;
  }

  return 0;
}

_Bool check_ground_old() {
  int current_player_pos_y_in_grid = ((background.y + player->y) >> 3) * TILEMAP_WIDTH,
      current_player_pos_x_in_grid = ((background.x + player->x) >> 3) - 1,
      current_player_byte_pos_in_grid = current_player_pos_y_in_grid + current_player_pos_x_in_grid,
      tile_floor_left = current_player_byte_pos_in_grid + 1 + TILEMAP_WIDTH + TILEMAP_WIDTH;

  int data_tile_floor_left = pgm_read_byte(&map.data[tile_floor_left]),
      data_tile_floor_right = pgm_read_byte(&map.data[tile_floor_left + 1]);

  if (data_tile_floor_left == 1 || data_tile_floor_right == 1) {
    return 1;
  }

  return 0;
}

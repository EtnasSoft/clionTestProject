#include "assets_manager.h"

#ifndef TEST
#include <avr/pgmspace.h>
#else
#endif

void assets_manager_map_init(map_game_ptr map, byte *level) {
  map->width = pgm_read_byte(level);
  map->height = pgm_read_byte(++level);
  map->data = ++level;
}

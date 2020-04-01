#include "assets_manager.h"

#ifndef TEST
#include <avr/pgmspace.h>
#else
#include <../test/support/avr/stub_pgmspace.h>
#endif

void assets_manager_map_init(map_game_ptr map, const uint8_t* level) {
  map->width = pgm_read_byte(level);
  map->height = pgm_read_byte(++level);
  map->size = map->width * map->height;
  map->data = ++level;
}

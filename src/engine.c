#include "types.h"
#include "engine.h"

void engine_background_init(background_game_ptr background) {
  background->x = 0;
  background->y = 0;
  background->speed = 0;
  background->direction = 0;
}

int engine_background_set(const background_game *new_background_config) {
  background = *new_background_config;

  return 0;
}

const background_game *engine_background_get(void) {
  return &background;
}

void engine_background_reload(background_game_ptr background) {
  // TODO: tengo que arreglar los assets primero:
  // - Adaptar el PROGMEM
  // - Adaptar el memcpy_P
  // - Indicar el tamaño mediante los dos valores iniciales
  // - ...

  /*byte x, y, *d, bitStart,
      iStart = engine_background_get()->y >> 3;

  for (y = 0; y < PLAYFIELD_ROWS; y++) {
    bitStart = ((iStart + y) * PLAYFIELD_COLS) % (PLAYFIELD_COLS * PLAYFIELD_ROWS);
    d = &engine_background_get()->data[bitStart];

    for (x = 0; x < PLAYFIELD_COLS; x++) {
      memcpy_P(d++, &tileMap[((iStart + y + 1) % TILEMAP_HEIGHT)  * PLAYFIELD_COLS + x], 1);
    }
  }*/
}

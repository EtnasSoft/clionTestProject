#include "types.h"
#include "engine.h"

void engine_background_init(background_game_ptr background) {
  background->x = 0;
  background->y = 0;
  background->speed = 0;
  background->direction = 0;

  memset(&background->data[0], 0, sizeof(background->data));
}

int engine_background_set(const background_game *new_background_config) {
  background = *new_background_config;

  return 0;
}

const background_game *engine_background_get(void) {
  return &background;
}

// Pinta en la pantalla la porcion media del background:
// desde la fila 1 (la 0 es buffer) a la 8 (la 9 es buffer también).
void engine_background_reload(background_game_ptr background, map_game_ptr map) {
  byte x, y, *d, bit_start,
      start = background->y >> 3;

  for (y = 0; y < SCREEN_BUFFER_HEIGHT; y++) {
    bit_start = ((start + y) * SCREEN_BUFFER_WIDTH) % SCREEN_BUFFER_SIZE;
    d = &background->data[bit_start];

    uint16_t first_byte = ((start + y + 0) % map->height) * SCREEN_BUFFER_WIDTH;
    for (x = 0; x < SCREEN_BUFFER_WIDTH; x++) {
      _memcpy(d++, &map->data[first_byte + x], 1);
    }
  }
}

// Ajustamos el background
void engine_background_adjust(background_game_ptr background, map_game_ptr map) {
  byte *d1, *d2;
  byte current_row = (background->y >> 3) + (EDGES / 2);

  int next_playfield_bit = (current_row + VIEWPORT_HEIGHT) * SCREEN_BUFFER_WIDTH,
      c_next_playfield_bit = next_playfield_bit % SCREEN_BUFFER_SIZE,
      c_next_row = (current_row + VIEWPORT_HEIGHT + 1) % TILEMAP_HEIGHT;

  int prev_playfield_bit = current_row * SCREEN_BUFFER_WIDTH,
      c_prev_playfield_bit = prev_playfield_bit % SCREEN_BUFFER_SIZE,
      c_prev_row = (current_row + 1) % TILEMAP_HEIGHT;

  // Pintamos las filas que quedan fuera de la pantall tanto por arriba, como por abajo.
  // Igual es momento de cambiar esta estrategia...
  d1 = &background->data[c_next_playfield_bit];
  d2 = &background->data[c_prev_playfield_bit];

  printf("current_row: %i\nnext_playfield_bit: %i\nprev_playfield_bit: %i\n",
  current_row, next_playfield_bit, prev_playfield_bit);


  for (byte x1 = 0; x1 < SCREEN_BUFFER_WIDTH; x1++) {
    _memcpy(d1 + x1, &map->data[c_next_row * SCREEN_BUFFER_WIDTH + x1], 1);
    _memcpy(d2 + x1, &map->data[c_prev_row * SCREEN_BUFFER_WIDTH + x1], 1);
  }
}

void engine_background_draw(background_game_ptr background) {
  /*byte bTemp[SCREEN_WIDTH]; // holds data for the current scan line
  byte x, y, tx;
  int ty, bXOff, bYOff;
  byte c, *s, *sNext, *d;
  int iOffset, iOffset2, cIndex, cIndex2;

  // Solo es cero cuando el scroll completa un MODULO su eje X (8 unidades)
  bXOff = background->x & (MODULE - 1);
  bYOff = background->y & (MODULE - 1);

  // Incrementa una unidad cada vez que el scroll completa un MODULO en Y
  ty = (background->y >> 3) + (EDGES / 2);

  engine_background_adjust(&background, &map);*/
}

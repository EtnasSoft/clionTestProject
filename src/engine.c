// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"
#include "engine.h"
#include <stdbool.h>

#ifdef TEST
#include "../test/support/Arduino/stub_arduino.h"
unsigned long millis(void);
#else
#include <Arduino.h>
#endif


// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
void engine_init_frame_control() {
  engine_set_frame_duration(30);
  frame_count = 0;
  just_rendered = false;
}

void engine_set_frame_duration(byte duration) {
  each_frame_millis = duration;
}

_Bool engine_next_frame() {
  byte now = (byte) millis();
  byte frame_duration_ms = now - this_frame_start;

  if (just_rendered) {
    last_frame_duration_ms = frame_duration_ms;
    just_rendered = false;

    return false;
  } else if (frame_duration_ms < each_frame_millis) {
    if (++frame_duration_ms < each_frame_millis) {
    engine_idle();
    }

    return false;
  }

  // pre render
  just_rendered = true;
  this_frame_start = now;
  frame_count++;

  return true;
}

void engine_idle() {
  // power safe mode...
  //  SMCR = _BV(SE); // select idle mode and enable sleeping
  //  sleep_cpu();
  //  SMCR = 0; // disable sleeping
}

_Bool engine_every_x_frame(byte frames) {
  return frame_count % frames == 0;
}

void engine_background_init(background_game_ptr background) {
  background->x = 0;
  background->y = 0;
  background->x_old = 0;
  background->y_old = 0;
  background->x_offset = 0;
  background->y_offset = 0;
  background->x_page = 0;
  background->y_page = 0;
  background->speed = 1;
  background->need_render = 0;
}

int engine_background_set(const background_game *new_background_config) {
  background = *new_background_config;

  return 0;
}

const background_game *engine_background_get(void) {
  return &background;
}

void engine_background_set_pos(uint16_t x, uint16_t y) {
  background.x = x;
  background.x_offset = x & 7;
  background.x_page = x >> 3;

  background.y = y;
  background.y_offset = y & 7;
  background.y_page = y >> 3;
}

void engine_background_adjust_rows(map_game_ptr map) {
  byte offsetCol = background.x_page % PLAYFIELD_WIDTH,
      offsetRow = background.y_page % PLAYFIELD_HEIGHT,
      prevPlayFieldRow = (offsetRow == 0 ? PLAYFIELD_HEIGHT : offsetRow) - 1,
      nextPlayFieldRow = (offsetRow + VIEWPORT_HEIGHT) % PLAYFIELD_HEIGHT,
      carry;

  // Tiles
  int prevPlayFieldByte = prevPlayFieldRow * PLAYFIELD_WIDTH,
      nextPlayFieldByte = nextPlayFieldRow * PLAYFIELD_WIDTH,
      prevTileRow = ((background.y_page == 0 ? map->height : background.y_page) - 1) * map->width,
      nextTileRow = ((background.y_page + VIEWPORT_HEIGHT) % map->height) * map->width,
      nextTileByte,
      prevTileByte;

  if (prevPlayFieldByte >= PLAYFIELD_SIZE) {
    prevPlayFieldByte -= PLAYFIELD_SIZE;
  }

  if (nextPlayFieldByte >= PLAYFIELD_SIZE) {
    nextPlayFieldByte -= PLAYFIELD_SIZE;
  }

  prevPlayFieldByte += offsetCol;
  nextPlayFieldByte += offsetCol;

  for (byte x = 0; x < PLAYFIELD_WIDTH; x++) {
    carry = (background.x_page + x) % map->width;
    nextTileByte = (nextTileRow + carry);
    prevTileByte = (prevTileRow + carry);

    _memcpy(&background_data[prevPlayFieldByte++], &map->data[prevTileByte], 1);
    _memcpy(&background_data[nextPlayFieldByte++], &map->data[nextTileByte], 1);

    if (nextPlayFieldByte % PLAYFIELD_WIDTH == 0) {
      prevPlayFieldByte -= PLAYFIELD_WIDTH;
      nextPlayFieldByte -= PLAYFIELD_WIDTH;
    }
  }
}

void engine_background_adjust_cols(map_game_ptr map) {
  byte offsetRow = background.y_page % PLAYFIELD_HEIGHT,
      offsetCol = background.x_page % PLAYFIELD_WIDTH,
      currentPlayFieldByte = offsetRow * PLAYFIELD_WIDTH;

  int nextTileByte,
      prevTileByte,
      currentTileByte = background.y_page * map->width,
      nextPlayFieldByte = currentPlayFieldByte + ((offsetCol + VIEWPORT_WIDTH) % PLAYFIELD_WIDTH),
      prevPlayFieldByte = offsetCol == 0
                          ? currentPlayFieldByte + PLAYFIELD_WIDTH - 1
                          : currentPlayFieldByte + (offsetCol - 1);

  if (prevPlayFieldByte < 0) {
    prevPlayFieldByte += PLAYFIELD_WIDTH;
  }

  nextTileByte = currentTileByte + ((background.x_page + VIEWPORT_WIDTH) % map->width);
  prevTileByte = background.x_page == 0
                 ? currentTileByte + PLAYFIELD_WIDTH - 1
                 : currentTileByte + (background.x_page - 1);

  for (byte x = 0; x < PLAYFIELD_HEIGHT; x++) {
    _memcpy(&background_data[nextPlayFieldByte], &map->data[nextTileByte], 1);
    _memcpy(&background_data[prevPlayFieldByte], &map->data[prevTileByte], 1);

    nextPlayFieldByte = (nextPlayFieldByte + PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    prevPlayFieldByte = (prevPlayFieldByte + PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    nextTileByte = (nextTileByte + map->width) % map->size;
    prevTileByte = (prevTileByte + map->width) % map->size;
  }
}

void engine_background_update(map_game_ptr map) {
  if (background.x != background.x_old) {
    background.x_old = background.x;
    engine_background_adjust_cols(map);
    background.need_render = 1;
  }

  if (background.y != background.y_old) {
    background.y_old = background.y;
    engine_background_adjust_rows(map);
    background.need_render = 1;
  }
}

void engine_background_reload(map_game_ptr map) {
  byte x, y, currentPlayFieldByte, nextPlayFieldByte,
      offsetCol = background.x_page % PLAYFIELD_WIDTH;

  int nextTileByte;

  for (y = 0; y < PLAYFIELD_HEIGHT; y++) {
    currentPlayFieldByte = ((background.y_page + y) * PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    currentPlayFieldByte += offsetCol;

    nextPlayFieldByte = currentPlayFieldByte;

    // Use the fact that 32 * x = x << 5
    nextTileByte = ((background.y_page + y) % map->height) * map->width;
    nextTileByte += background.x_page;

    for (x = 0; x < PLAYFIELD_WIDTH; x++) {
      _memcpy(&background_data[nextPlayFieldByte++], &map->data[nextTileByte++], 1);

      if ((nextPlayFieldByte % PLAYFIELD_WIDTH) == 0) {
        nextPlayFieldByte -= PLAYFIELD_WIDTH;
      }

      if ((nextTileByte % map->width) == 0) {
        nextTileByte -= map->width;
      }
    }
  }
}

//void engine_background_adjust(background_game_ptr background, map_game_ptr map) {}
//void engine_background_draw(background_game_ptr background) {}

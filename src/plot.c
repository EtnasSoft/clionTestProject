#include "types.h"
#include "plot.h"
#include "assets.h"
#include "i2c_driver.h"
#include "i2c_hardware.h"
#include "screen_driver.h"
#include <string.h>

void plot_point(byte x, byte y) {
  byte bTemp[8] = { 0 };
  byte currentRow = y >> 3;
  byte currentCol = x >> 3;
  byte offsetY = y & 7;
  byte offsetX = x & 7;
  byte currentBackgroundData = background_data[(currentRow * PLAYFIELD_WIDTH) + currentCol];

  // Use the fact that x * MODULE = x << 3
  memcpy_P(bTemp, &ucTiles[(currentBackgroundData << 3) + offsetX], 1);
  bTemp[0] |= (1 << offsetY);
  screen_driver_set_position(x, currentRow);
  i2c_driver_write_data(bTemp, 1);
}

// Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void draw_to(int x, int y, int x1, int y1) {
  int sx, sy, e2, err;
  byte dx = abs(x1 - x);
  byte dy = abs(y1 - y);

  if (x < x1) sx = 1; else sx = -1;
  if (y < y1) sy = 1; else sy = -1;

  err = dx - dy;

  for (;;) {
    plot_point(x, y);
    if (x == x1 && y == y1) return;
    e2 = err << 1;

    if (e2 > -dy) {
      err = err - dy;
      x = x + sx;
    }

    if (e2 < dx) {
      err = err + dx;
      y = y + sy;
    }
  }
}

void plot_text(int x, int y, PGM_P s) {
  byte bTemp[8] = { 0 };
  int p = (int)s;
  byte currentRow = y >> 3;
  while (1) {
    char c = pgm_read_byte(p++);
    if (c == 0) return;

    for (uint8_t col = 0 ; col < 6; col++) {
      screen_driver_set_position(x, currentRow);
      memcpy_P(bTemp, &charMap[c-32][col], 1);
      i2c_driver_write_data(bTemp, 1);
      x++;
    }
  }
}

void plot_big_text(int x, int y, PGM_P s) {
  byte bTemp[16] = { 0 };
  int p = (int)s;
  byte currentRow = y >> 3;

  while (1) {
    char c = pgm_read_byte(p++);
    if (c == 0) return;

    for (uint8_t col = 0 ; col < 6; col++) {
      int bits = stretch(pgm_read_byte(&charMap[c-32][col]));
      for (int i=2; i--;) {
        screen_driver_set_position(x, currentRow);
        bTemp[0] = bits;
        i2c_driver_write_data(bTemp, 2);

        screen_driver_set_position(x, currentRow + 1);
        bTemp[0] = bits >> 8;
        i2c_driver_write_data(bTemp, 2);

        x++;
      }
    }
  }
}

int stretch (int x) {
  x = (x & 0xF0)<<4 | (x & 0x0F);
  x = (x<<2 | x) & 0x3333;
  x = (x<<1 | x) & 0x5555;
  return x | x<<1;
}

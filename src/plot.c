// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"
#include "plot.h"
#include "assets.h"
#include "i2c_driver.h"
#include "i2c_hardware.h"
#include "screen_driver.h"
#include <string.h>


// DEFINES
// ////////////////////////////////////////////////////////////////////
#define FONT_SIZE_NORMAL 6
#define FONT_SIZE_BIG 12


// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
void plot_point(byte x, byte y) {
  byte bTemp[8] = { 0 };
  byte currentRow = y >> 3;
  byte currentCol = x >> 3;
  byte offsetY = y & 7;
  byte offsetX = x & 7;
  byte currentBackgroundData = background_data[(currentRow * PLAYFIELD_WIDTH) + currentCol];

  // Use the fact that x * MODULE = x << 3
  _memcpy(bTemp, &ucTiles[(currentBackgroundData << 3) + offsetX], 1);
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

void plot_char(byte x, byte y, byte c) {
  byte bTemp[FONT_SIZE_NORMAL] = { 0 };

  for (uint8_t col = 0 ; col < FONT_SIZE_NORMAL; col++) { // 6 bytes => each character is 6 bytes
    screen_driver_set_position(x++, y >> 3);
    _memcpy(bTemp, &charMap[c - 32][col], 1); // 32 ASCII constant (avoid the first 32 ASCII characters in table)
    i2c_driver_write_data(bTemp, 1);
  }
}

void plot_char_big(byte x, byte y, byte c) {
  byte bTemp[FONT_SIZE_BIG] = { 0 };

  for (byte col = 0 ; col < FONT_SIZE_NORMAL; col++) {
    int bits = stretch(pgm_read_byte(&charMap[c - 32][col]));

    for (byte i = 2; i--;) {
      screen_driver_set_position(x, y >> 3);
      bTemp[0] = bits;
      i2c_driver_write_data(bTemp, 2);

      screen_driver_set_position(x, (y >> 3) + 1);
      bTemp[0] = bits >> 8;
      i2c_driver_write_data(bTemp, 2);

      x++;
    }
  }
}

void plot_text(byte x, byte y, PGM_P s) {
  while (1) {
    char c = pgm_read_byte(s++);
    if (c == 0) return;

    plot_char(x, y, c);
    x += FONT_SIZE_NORMAL;
  }
}

void plot_big_text(byte x, byte y, PGM_P s) {
  while (1) {
    char c = pgm_read_byte(s++);
    if (c == 0) return;

    plot_char_big(x, y, c);
    x += FONT_SIZE_BIG;
  }
}

/**
 * Plot a number.
 * Range is 0 to 999
 *
 * @param x
 * @param y
 * @param number
 */
void plot_integer(byte x, byte y, int number) {
  _Bool dig = 0;
  unsigned int j = 1000;
  y >>= 3;

  do {
    char c = (abs(number) / j) % 10 + '0';

    if (c == '0' && !dig && j != 1) {
      c = ' ';
    } else {
      dig = 1;
    }

    plot_char(x, y, c);
    j /= 10;
    x += FONT_SIZE_NORMAL;
  } while (j);
}

int stretch (int x) {
  x = (x & 0xF0) << 4 | (x & 0x0F);
  x = (x << 2 | x) & 0x3333;
  x = (x << 1 | x) & 0x5555;
  return x | x << 1;
}

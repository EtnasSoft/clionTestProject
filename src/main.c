// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <avr/interrupt.h>
#include "types.h"
#include "assets.h"
#include "main.h"
#include "i2c_driver.h"
#include "i2c_hardware.h"
#include "screen_driver.h"
#include "engine.h"
#include "assets_manager.h"
#include "controls.h"
#include "plot.h"
#include "player.h"

// DEFINES
// ////////////////////////////////////////////////////////////////////
#define EncoderClick A0 // A0 PB5, pin 1 (RESET)
#define DIGITAL

// GLOBALS (Player and Game vars)
// ////////////////////////////////////////////////////////////////////
gfx_object_ptr player = object_list; // By default, player points to object_list[0]
_Bool render_ready = 1;
const int EncoderA = 2; // PB2, pin 7 (INT0)
const int EncoderB = 1; // PB1, pin 6

volatile int a0;
volatile int c0;

// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
// Draw 1 character space that's vertically shifted
void draw_shifted_char(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff) {
  byte c, c2, z;

  for (z = 0; z < (8 - bXOff); z++) {
    c = pgm_read_byte(s1++);
    c >>= bYOff; // shift over
    c2 = pgm_read_byte(s2++);
    c2 <<= (8 - bYOff);
    *d++ = (c | c2);
  }
}

// Draw the sprites visible on the current line
void draw_sprites(byte y, byte *pBuf, gfx_object *pList, byte bCount) {
  byte i, x, bSize, bSprite, *s, *d;
  byte cOld, cNew, mask, bYOff, bWidth;

  gfx_object *pObject;
  for (i = 0; i < bCount; i++) {
    pObject = &pList[i];
    bSprite = pObject->bType;          // index
    bSize = (bSprite & 0x80) ? 16 : 8; // big or small sprite

    // see if it's visible
    if (pObject->y >= y + 8) // past bottom
      continue;
    if (pObject->y + bSize <= y) // above top
      continue;
    if (pObject->x >= 128) // off right edge
      continue;

    // It's visible on this line; draw it
    bSprite &= 0x7f;       // sprite index
    d = &pBuf[pObject->x]; // destination pointer

    if (bSize == 16) {
      s = (byte *)&ucBigSprites[bSprite * 64];

      if (pObject->y + 8 <= y) {
        // special case - only bottom half drawn
        s += 16;
      }

      bYOff = pObject->y & 7;
      bWidth = 16;

      if (128 - pObject->x < 16) {
        bWidth = 128 - pObject->x;
      }

      // 4 possible cases:
      // byte aligned - single source, not shifted
      // single source (shifted, top or bottom row)
      // double source (middle) both need shifting
      if (bYOff == 0) {
        // simplest case
        for (x = 0; x < bWidth; x++) {
          cOld = d[0];
          mask = pgm_read_byte(s);
          cNew = pgm_read_byte(s + 32);
          s++;
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        }
      } else if (pObject->y + 8 < y)  {
        // only bottom half of sprite drawn
        for (x = 0; x < bWidth; x++) {
          mask = pgm_read_byte(s);
          cNew = pgm_read_byte(s + 32);
          s++;
          mask >>= (8 - bYOff);
          mask |= (0xff << bYOff);
          cNew >>= (8 - bYOff);
          cOld = d[0];
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        }                        // for x
      } else if (pObject->y > y) {
        // only top half of sprite drawn
        for (x = 0; x < bWidth; x++) {
          mask = pgm_read_byte(s);
          cNew = pgm_read_byte(s + 32);
          s++;
          mask <<= bYOff;
          mask |= (0xff >> (8 - bYOff)); // exposed bits set to 1
          cNew <<= bYOff;
          cOld = d[0];
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        }    // for x
      } else {
        // most difficult - part of top and bottom drawn together
        byte mask2, cNew2;
        for (x = 0; x < bWidth; x++) {
          mask = pgm_read_byte(s);
          mask2 = pgm_read_byte(s + 16);
          cNew = pgm_read_byte(s + 32);
          cNew2 = pgm_read_byte(s + 48);
          s++;
          mask >>= (8 - bYOff);
          cNew >>= (8 - bYOff);
          mask2 <<= bYOff;
          cNew2 <<= bYOff;
          mask |= mask2; // combine top and bottom
          cNew |= cNew2;
          cOld = d[0];
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        } // for x
      }
    } else {
      // 8x8 sprite
      s = (byte *)&ucSprites[bSprite * 16];
      bYOff = pObject->y & 7;
      bWidth = 8;
      if (128 - pObject->x < 8)
        bWidth = 128 - pObject->x;
      for (x = 0; x < bWidth; x++) {
        mask = pgm_read_byte(s);
        cNew = pgm_read_byte(s + 8);
        s++;
        if (bYOff) // needs to be shifted
        {
          if (pObject->y > y) {
            mask <<= bYOff;
            mask |= (0xff >> (8 - bYOff)); // exposed bits set to 1
            cNew <<= bYOff;
          } else {
            mask >>= (8 - bYOff);
            mask |= (0xff << bYOff);
            cNew >>= (8 - bYOff);
          }
        } // needs to be shifted
        cOld = d[0];
        cOld &= mask;
        cOld |= cNew;
        *d++ = cOld;
      } // for x
    }
  } // for each sprite
}

// Draw the playfield and sprites
void draw_play_field() {
  byte *s, *sNext, *d, x, y,
      bTemp[SCREEN_WIDTH];

  int tx, c, ty, bXOff, bYOff, iOffset, iOffset2;

  bXOff = background.x_offset;
  bYOff = background.y_offset;

  ty = background.y_page;

  // -------------------------------------------------------

  // draw the 8 rows
  for (y = 0; y < VIEWPORT_HEIGHT; y++) {
    memset(bTemp, 0, sizeof(bTemp));

    ty = ty % PLAYFIELD_HEIGHT;
    tx = background.x_page;

    // Draw the playfield characters at the given scroll position
    d = bTemp;

    if (bYOff) {
      for (x = 0; x < VIEWPORT_WIDTH; x++) {
        if (tx >= PLAYFIELD_WIDTH) {
          tx %= PLAYFIELD_WIDTH; // wrap around
        }

        iOffset = tx + (ty * PLAYFIELD_WIDTH);

        if (iOffset >= PLAYFIELD_SIZE) {    // past bottom
          iOffset %= PLAYFIELD_SIZE;
        }

        iOffset2 = (iOffset + PLAYFIELD_WIDTH); // next line

        if (iOffset2 >= PLAYFIELD_SIZE) {    // past bottom
          iOffset2 %= PLAYFIELD_SIZE;
        }

        c = background_data[iOffset];
        s = (byte *)&ucTiles[(c * MODULE) + bXOff];

        c = background_data[iOffset2];
        sNext = (byte *)&ucTiles[(c * MODULE) + bXOff];

        draw_shifted_char(s, sNext, d, bXOff, bYOff);

        d += (MODULE - bXOff);
        bXOff = 0;
        tx++;
      }

      // partial character left to draw
      if (d != &bTemp[SCREEN_WIDTH]) {
        bXOff = (byte)(&bTemp[SCREEN_WIDTH] - d);

        if (tx >= PLAYFIELD_WIDTH) {
          tx %= PLAYFIELD_WIDTH;
        }

        iOffset = tx + ty * PLAYFIELD_WIDTH;
        iOffset2 = iOffset + PLAYFIELD_WIDTH; // next line

        if (iOffset2 >= PLAYFIELD_SIZE) {    // past bottom
          iOffset2 %= PLAYFIELD_SIZE;
        }

        c = background_data[iOffset];
        s = (byte *)&ucTiles[c * MODULE];

        c = background_data[iOffset2];
        sNext = (byte *)&ucTiles[c * MODULE];

        draw_shifted_char(s, sNext, d, MODULE - bXOff, bYOff);
      }
      // simpler case of vertical offset of 0 for each character
    } else {
      //-----------------------------------
      // NON BYOFF; SCROLL BLOCK COMPLETED!
      //-----------------------------------

      // Filling each col of the SCREEN_WIDTH
      for (x = 0; x < VIEWPORT_WIDTH; x++) {
        if (tx >= PLAYFIELD_WIDTH) {
          tx %= PLAYFIELD_WIDTH;
        }

        iOffset = tx + (ty * PLAYFIELD_WIDTH);

        if (iOffset >= PLAYFIELD_SIZE) {    // past bottom
          iOffset %= PLAYFIELD_SIZE;
        }

        c = background_data[iOffset];
        s = (byte *)&ucTiles[(c * MODULE) + bXOff];
        memcpy_P(d, s, MODULE - bXOff);

        d += (MODULE - bXOff);
        bXOff = 0;

        tx++;
      }

      // partial character left to draw
      if (d != &bTemp[SCREEN_WIDTH]) {
        bXOff = (byte)(&bTemp[SCREEN_WIDTH] - d);

        if (tx >= PLAYFIELD_WIDTH) {
          tx %= PLAYFIELD_WIDTH;
        }

        iOffset = tx + ty * PLAYFIELD_WIDTH;

        c = background_data[iOffset];
        s = (byte *)&ucTiles[c * MODULE];
        memcpy_P(d, s, bXOff);
      }
    }

    // Draw sprites
    draw_sprites(y << 3, bTemp, object_list, NUMBER_OF_SPRITES); // y * 8 => y << 3

    // Send it to the display
    screen_driver_set_position(0, y);
    i2c_driver_write_data(bTemp, SCREEN_WIDTH);
    ty++;
  }

  render_ready = 1;
}

void reload_play_field(void) {
  byte x, y, currentPlayFieldByte, nextPlayFieldByte,
      currentRow = background.y_page,
      currentCol = background.x_page,
      offsetCol = currentCol % PLAYFIELD_WIDTH;

  int nextTileByte;

  for (y = 0; y < PLAYFIELD_HEIGHT; y++) {
    currentPlayFieldByte = ((currentRow + y) * PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    currentPlayFieldByte += offsetCol;

    nextPlayFieldByte = currentPlayFieldByte;

    // Use the fact that 32 * x = x << 5
    nextTileByte = ((currentRow + y) % TILEMAP_HEIGHT) * TILEMAP_WIDTH;
    nextTileByte += currentCol;

    for (x = 0; x < PLAYFIELD_WIDTH; x++) {
      memcpy_P(&background_data[nextPlayFieldByte++], &map.data[nextTileByte++], 1);

      if ((nextPlayFieldByte % PLAYFIELD_WIDTH) == 0) {
        nextPlayFieldByte -= PLAYFIELD_WIDTH;
      }

      if ((nextTileByte % TILEMAP_WIDTH) == 0) {
        nextTileByte -= TILEMAP_WIDTH;
      }
    }
  }

  draw_play_field();
}

// TODO: Estos dos metodos tienen que irse a engine.c
void adjust_play_field_rows(void) {
  int currentRow = background.y_page,
      currentCol = background.x_page,
      offsetCol = currentCol % PLAYFIELD_WIDTH,
      offsetRow = currentRow % PLAYFIELD_HEIGHT,

      prevPlayFieldRow = (offsetRow == 0 ? PLAYFIELD_HEIGHT : offsetRow) - 1,
      nextPlayFieldRow = (offsetRow + VIEWPORT_HEIGHT) % PLAYFIELD_HEIGHT,
      prevPlayFieldByte = prevPlayFieldRow * PLAYFIELD_WIDTH,
      nextPlayFieldByte = nextPlayFieldRow * PLAYFIELD_WIDTH;

  // Tiles
  int prevTileRow = ((currentRow == 0 ? TILEMAP_HEIGHT : currentRow) - 1) * TILEMAP_WIDTH,
      nextTileRow = ((currentRow + VIEWPORT_HEIGHT) % TILEMAP_HEIGHT) * TILEMAP_WIDTH,
      nextTileByte,
      prevTileByte,
      carry;

  if (prevPlayFieldByte >= PLAYFIELD_SIZE) {
    prevPlayFieldByte -= PLAYFIELD_SIZE;
  }

  if (nextPlayFieldByte >= PLAYFIELD_SIZE) {
    nextPlayFieldByte -= PLAYFIELD_SIZE;
  }

  prevPlayFieldByte += offsetCol;
  nextPlayFieldByte += offsetCol;

  for (byte x = 0; x < PLAYFIELD_WIDTH; x++) {
    carry = (currentCol + x) % TILEMAP_WIDTH;
    nextTileByte = (nextTileRow + carry);
    prevTileByte = (prevTileRow + carry);

    memcpy_P(&background_data[prevPlayFieldByte++], &map.data[prevTileByte], 1);
    memcpy_P(&background_data[nextPlayFieldByte++], &map.data[nextTileByte], 1);

    if (nextPlayFieldByte % PLAYFIELD_WIDTH == 0) {
      prevPlayFieldByte -= PLAYFIELD_WIDTH;
      nextPlayFieldByte -= PLAYFIELD_WIDTH;
    }
  }
}

void adjust_play_field_cols(void) {
  byte currentRow = background.y_page,
      currentCol = background.x_page,
      offsetRow = currentRow % PLAYFIELD_HEIGHT,
      offsetCol = currentCol % PLAYFIELD_WIDTH,
      currentPlayFieldByte = offsetRow * PLAYFIELD_WIDTH;

  int nextTileByte,
      prevTileByte,
      currentTileByte = currentRow * TILEMAP_WIDTH,
      nextPlayFieldByte = currentPlayFieldByte + ((offsetCol + VIEWPORT_WIDTH) % PLAYFIELD_WIDTH),
      prevPlayFieldByte = offsetCol == 0
        ? currentPlayFieldByte + PLAYFIELD_WIDTH - 1
        : currentPlayFieldByte + (offsetCol - 1);

  if (prevPlayFieldByte < 0) {
    prevPlayFieldByte += PLAYFIELD_WIDTH;
  }

  nextTileByte = currentTileByte + ((currentCol + VIEWPORT_WIDTH) % TILEMAP_WIDTH);
  prevTileByte = currentCol == 0
     ? currentTileByte + PLAYFIELD_WIDTH - 1
     : currentTileByte + (currentCol -1);

  for (byte x = 0; x < PLAYFIELD_HEIGHT; x++) {
    memcpy_P(&background_data[nextPlayFieldByte], &map.data[nextTileByte], 1);
    memcpy_P(&background_data[prevPlayFieldByte], &map.data[prevTileByte], 1);

    nextPlayFieldByte = (nextPlayFieldByte + PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    prevPlayFieldByte = (prevPlayFieldByte + PLAYFIELD_WIDTH) % PLAYFIELD_SIZE;
    nextTileByte = (nextTileByte + TILEMAP_WIDTH) % TILEMAP_SIZE;
    prevTileByte = (prevTileByte + TILEMAP_WIDTH) % TILEMAP_SIZE;
  }
}

#ifdef ANALOG
// Interrupt handler **********************************************
// Called when encoder value changes
// Button interrupt, INT0, PB2, pin7
void move_background_to(_Bool increment) {
  // LINEAL
  /*if (increment) {
    (background.direction == 1)
      ? controls_move_background_to_up(&background)
      : controls_move_background_to_left(&background);
  } else {
    (background.direction == 1)
      ? controls_move_background_to_down(&background)
      : controls_move_background_to_right(&background);
  }*/
  //  if (increment) {
  //    if (player->x < 64) {
  //      player->x += player->x_speed;
  //    } else {
  //      controls_move_background_to_right(&background);
  //    }
  //  } else {
  //    if (player->x > 56) {
  //      player->x -= player->x_speed;
  //    } else {
  //      controls_move_background_to_left(&background);
  //    }
  //  }
  if (
    (render_ready)
  ) {
    render_ready = 0;
    if (increment) {
      controls_move_background_to_right(&background);
      if (check_collision()) { // collision
        controls_move_background_to_left(&background);
      }
    } else {
      controls_move_background_to_left(&background);
      if (check_collision()) { // collision
        controls_move_background_to_right(&background);
      }
    }
  }
}

void move_background() {
  int a = PINB>>EncoderA & 1;
  int b = PINB>>EncoderB & 1;

  // A changed
  if (a != a0) {
    a0 = a;
    if (b != c0) {
      c0 = b;
      move_background_to(a == b);
    }
  }
}
#endif

void init_sprites() {
  memset(object_list, 0, sizeof(object_list));
  player_init(player);
}

_Bool check_collision() {
  int current_row = (background.y_page + player->y_page % PLAYFIELD_HEIGHT) * PLAYFIELD_WIDTH,
    base = (background.x_page + player->x_page) % PLAYFIELD_WIDTH,
    prev_pos_top = current_row + base,
    prev_pos_bottom = prev_pos_top + PLAYFIELD_WIDTH,
    next_pos_top = current_row + ((base + 2) % PLAYFIELD_WIDTH),
    next_pos_bottom = next_pos_top + PLAYFIELD_WIDTH;

  if (
      (background_data[prev_pos_top] == 1) ||
      (background_data[next_pos_top] == 1) ||
      (background_data[next_pos_bottom] == 1) ||
      (background_data[prev_pos_bottom] == 1) ||
      (player->on_ground == 0 && check_ground())
  ) {
    return 1;
  }

  return 0;
}

_Bool check_ground() {
  // "2" is the player height in 'pages' => 16 >> 3 = 2
  int current_row = ((background.y_page + player->y_page + 2) % PLAYFIELD_HEIGHT) * PLAYFIELD_WIDTH,
      base_1 = (background.x_page + player->x_page) % PLAYFIELD_WIDTH,
      base_2 = (base_1 + 2) % PLAYFIELD_WIDTH,
      tile_floor_left = current_row + base_1,
      tile_floor_right = current_row + base_2;

  if (background_data[tile_floor_left] == 1 || background_data[tile_floor_right] == 1) {
    return 1;
  }

  return 0;
}

void setup() {
  delay(50); // wait for the OLED to fully power up

#ifdef ANALOG
  attachInterrupt(0, move_background, CHANGE); //INT0, PB2, pin7
#endif

  // Initializing screen drivers
  i2c_driver_init(SSD1306_SA);
  screen_driver_init(0, 0);
  //screen_driver_fill(0x00);

  // Initializing the background and loading the current level
  engine_background_init(&background);
  engine_background_set_pos(48, 0);
  assets_manager_map_init(&map, &level);

  // Initializing sprites and main routines
  init_sprites();
  reload_play_field();
  draw_play_field(); // Needed for draw sprites.
  engine_init_frame_control();

  // Some text...
  //plot_text(30, 22, PSTR("hello world"));
  //plot_big_text(40, 35, PSTR("momo"));

  //draw_to(0, 64, 125, 1);
  //plot_point(24,20);
  //plot_point(10,4);
}

void loop() {
  _Bool need_render = 0;

  while (1) {
    //Prevent the game from running too fast
    if (!engine_next_frame()) {
      return;
    }

    if (analogRead(EncoderClick) < 940) {
      player_start_jump(player);
    } else {
      player_end_jump(player);
    }

#ifdef DIGITAL
    if (digitalRead(2) == HIGH && render_ready == 1) {
      render_ready = 0;
      controls_move_background_to_right(&background);

      if (check_collision()) {
        controls_move_background_to_left(&background);
        render_ready = 1;
      }
    }

    if (digitalRead(1) == HIGH && render_ready == 1) {
      render_ready = 0;
      controls_move_background_to_left(&background);

      if (check_collision()) {
        controls_move_background_to_right(&background);
        render_ready = 1;
      }
    }

      /*if ((analogRead(2) >= 0) && (analogRead(2) < 100)){
        render_ready = 0;
        controls_move_background_to_right(&background);
      }

      if ((analogRead(2) > 200) && (analogRead(2) < 500)){
        render_ready = 0;
        controls_move_background_to_left(&background);
      }*/
#endif

    if (background.x > TILEMAP_MAX_WIDTH_SCROLL) {
      engine_background_set_pos(0, background.y);
      reload_play_field();
    } else if (background.x < 0) {
      engine_background_set_pos(TILEMAP_MAX_WIDTH_SCROLL, background.y);
      reload_play_field();
    }

    if (background.y > TILEMAP_MAX_HEIGHT_SCROLL) {
      engine_background_set_pos(background.x, 0);
      reload_play_field();
    } else if (background.y < 0) {
      engine_background_set_pos(background.x, TILEMAP_MAX_HEIGHT_SCROLL);
      reload_play_field();
    }

    // TODO: este check collision es constante; los calculos deben cachearse en el objeto player
    // o background
    if (
        (player->on_ground == 0) || (player->on_ground == 1 && !check_collision())
    ) {
      player_apply_gravity(player);
    }

    if (check_ground()) {
      player_fix_ground_position(player);
    }

    // TODO: Estos dos checks tienen que irse a engice.c con un 'updateBackground()'.
    // Dependen de adjust_play_field_cols y adjust_play_field_rows, ambos también se van a engine.c
    // También depende de need_render, y vamos a tener que hacer dos: un background.need_render en engine.c
    // y otro en player.need_render. Luego, comprobariamos todos (n) en una función:
    // check_for_render() { return background.need_render || player.need_render || ...}
    if (background.x != background.x_old) {
      background.x_old = background.x;
      adjust_play_field_cols();
      need_render = 1;
    }

    if (background.y != background.y_old) {
      background.y_old = background.y;
      adjust_play_field_rows();
      need_render = 1;
    }

    if (player->x_old != player->x) {
      player->x_old = player->x;
      need_render = 1;
    }

    if (player->y_old != player->y) {
      player->y_old = player->y;
      need_render = 1;
    }

    if (need_render) {
      draw_play_field();
    }
  }
}

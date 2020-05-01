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
    bSprite = pObject->bType; // sprite index
    bSize = (bSprite & 0x80) ? 16 : 8; // big or small sprite

    // If sprite is not visible, we forces the next iteration of the loop.
    if (pObject->y >= y + 8) // past bottom (8 = row height)
      continue; // next iteration
    if (pObject->y + bSize <= y) // above top
      continue; // next iteration
    if (pObject->x >= 128) // off right edge (128 = resolution width or SCREEN_WIDTH)
      continue; // next iteration

    // It's visible on this line; draw it
    bSprite &= 0x7f;       // sprite index (0x7f => 127) "How many indexes is above 128"
    d = &pBuf[pObject->x]; // destination pointer

    if (bSize == 16) {
      // A 16x16px sprite has 32 mask bytes followed by 32 pattern bytes. 64 bytes in total.
      s = (byte *)&ucBigSprites[bSprite * 64]; // 64 is the sum of 32bytes (mask) + 32bytes (pattern):

      if (pObject->y + 8 <= y) {
        // special case - only bottom half drawn
        s += 16; // Half of 32
      }

      bYOff = pObject->y & 7;
      bWidth = 16; // 16 => sprite width. Better in pObject->width.

      // Check if sprite is touching the sides of the screen.
      if (128 - pObject->x < 16) { // 128 SCREEN_WIDTH, 16 pObject->width OR sprite width
        bWidth = 128 - pObject->x; // bWidth is only for print the portion of the sprite visible in the loop.
      }

      // 4 possible cases:
      // byte aligned - single source, not shifted
      // single source (shifted, top or bottom row)
      // double source (middle) both need shifting
      if (bYOff == 0) {
        // simplest case
        for (x = 0; x < bWidth; x++) {
          cOld = d[0];
          mask = pgm_read_byte(s); // Mask is the first 32 bytes.
          cNew = pgm_read_byte(s + 32); // Pattern is the next 32.
          s++;
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        }
      } else if (pObject->y + 8 < y)  { // 8 is the half of sprite height (in 16x16px).
        // only bottom half of sprite drawn
        for (x = 0; x < bWidth; x++) {
          mask = pgm_read_byte(s); // Mask is the first 32 bytes.
          cNew = pgm_read_byte(s + 32); // Pattern is the next 32.
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
          mask = pgm_read_byte(s); // Mask is the first 32 bytes.
          cNew = pgm_read_byte(s + 32); // Pattern is the next 32.
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
          mask2 = pgm_read_byte(s + 16); // 16 is half of 32 bytes needed in 16x16 sprite
          cNew = pgm_read_byte(s + 32); // 32 is the size of a block (in bytes) in 16x16 sprite
          cNew2 = pgm_read_byte(s + 48); // 48 is 16 (first half) + 32 (block size).
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
      // A 8x8px sprite has 8 mask bytes followed by 8 pattern bytes. 16 bytes in total.
      s = (byte *)&ucSprites[bSprite * 16]; // 16 is the sum of 8bytes (mask) + 8bytes (pattern)
      bYOff = pObject->y & 7;
      bWidth = 8; // 8 => sprite width. Better in pObject->width.

      // Check if sprite is touching the sides of the screen.
      if (128 - pObject->x < 8) { // 128 SCREEN_WIDTH, 8 sprite width
        bWidth = 128 - pObject->x; // bWidth is only for print the portion of the sprite visible in the loop.
      }

      for (x = 0; x < bWidth; x++) {
        mask = pgm_read_byte(s); // // Mask is the first 8 bytes.
        cNew = pgm_read_byte(s + 8); // Pattern is the next 8.
        s++;

        if (bYOff) {
          // needs to be shifted
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
  engine_background_reload();
  draw_play_field();
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
      (player->on_ground == 0 && check_ground_for(player))
  ) {
    return 1;
  }

  return 0;
}

_Bool check_ground_for(gfx_object_ptr sprite) {
  // "2" is the player height in 'pages' => 16 >> 3 = 2
  int current_row = ((background.y_page + sprite->y_page + 2) % PLAYFIELD_HEIGHT) * PLAYFIELD_WIDTH,
      sprite_top_left_in_grid = (background.x_page + sprite->x_page) % PLAYFIELD_WIDTH,
      sprite_top_right_in_grid = (sprite_top_left_in_grid + 2) % PLAYFIELD_WIDTH,
      tile_floor_left = current_row + sprite_top_left_in_grid,
      tile_floor_right = current_row + sprite_top_right_in_grid;

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

    // TODO: este check collision es constante;
    //  los calculos deben cachearse en el objeto player o background
    if (
        (player->on_ground == 0) || (player->on_ground == 1 && !check_collision())
    ) {
      player_apply_gravity(player);
    }

    // Check ground se hace a cada frame, y es costoso...
    if (check_ground_for(player)) {
      player_fix_ground_position(player);
    }

    engine_background_update();
    player_update(player);

    if (background.need_render || player->need_render) {
      draw_play_field();
    }
  }
}

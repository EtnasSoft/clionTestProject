#ifndef CLIONTESTPROJECT_TYPES_H
#define CLIONTESTPROJECT_TYPES_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>


// TEST DEFINES
// ////////////////////////////////////////////////////////////////////
#ifdef TEST
#include <string.h>
#define _PROGMEM
#define _memcpy memcpy
#else
#include <avr/pgmspace.h>
#define _PROGMEM PROGMEM
#define _memcpy memcpy_P
#endif


// DEFINES
// ////////////////////////////////////////////////////////////////////
#define SSD1306_SCL PORTB4 // SCL, Pin 3
#define SSD1306_SDA PORTB3 // SDA, Pin 2
#define SSD1306_SA 0x3C // Slave Address (60)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MODULE 8 // Module for tiles (bits) used for screen division / pages
#define EDGES 2 // Used only for pseudo buffer screen purposes

#define VIEWPORT_WIDTH SCREEN_WIDTH / MODULE      // 16
#define VIEWPORT_HEIGHT SCREEN_HEIGHT / MODULE    // 8

// MAXIMUM: 16rows x 18cols => 288 byte + 128 byte (SSD1306 page)
#define PLAYFIELD_WIDTH (VIEWPORT_WIDTH + EDGES) // AXIS X: vertical cols, map width size; min 16+2 (SCREEN_WIDTH / 8) + Edges
#define PLAYFIELD_HEIGHT (VIEWPORT_HEIGHT + EDGES) // AXIS Y: horizontal rows, map height size; min 8+2 (SCREEN_HEIGHT / 8) + Edges
#define PLAYFIELD_SIZE (PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT) // AXIS Y: horizontal rows, map height size; min 8+2 (SCREEN_HEIGHT / 8) + Edges

#define DIRECT_PORT
#define I2CPORT PORTB
#define SAFE_DELAY 0

// A bit set to 1 in the DDR is an output, 0 is an INPUT
#define I2CDDR DDRB
#define NUMBER_OF_SPRITES 1


// TYPEDEFS
// ////////////////////////////////////////////////////////////////////
typedef uint8_t byte;

// uint8_t  -> 1 byte   -> 2^8    -> max. 256
// int      -> 2 bytes  -> 2^16   -> max. 65536

// Define the structure of the FX Objects
typedef struct gfx_object_typ {
  byte width;
  byte height;
  int x;
  int y;
  int current_row_in_grid;
  int current_col_in_grid;
  int x_old;
  int y_old;
  int x_offset;
  int y_offset;
  int x_page;
  int y_page;
  int current_top_left_pos_in_grid;
  int current_top_right_pos_in_grid;
  byte bType;
  byte x_speed;
  int y_speed; // can be negative
  byte gravity;
  _Bool on_ground;
  _Bool is_jumping; // prevent keep jumping while jump button is pressed
  _Bool need_render;
} gfx_object, *gfx_object_ptr;



// Define the structure of the background
typedef struct background_game_typ {
  int x;
  int y;
  int x_old;
  int y_old;
  int x_offset;
  int y_offset;
  int x_page;
  int y_page;
  byte speed;
  _Bool direction;
  _Bool need_render;
} background_game, *background_game_ptr;

typedef struct map_game_typ {
  byte width;
  byte height;
  int size;
  int max_scroll_x;
  int max_scroll_y;
  byte *data;
} map_game, *map_game_ptr;

// GLOBALS (Player and Game vars)
// ////////////////////////////////////////////////////////////////////
background_game background; // Define the background
map_game map; // Define the map
gfx_object object_list[NUMBER_OF_SPRITES]; // Define the game object list
byte background_data[PLAYFIELD_SIZE];


// GRAPHICS MODE
// ////////////////////////////////////////////////////////////////////
//
// MODE 0   No scroll
// MODE 1   Infinite Scroll vertical
// MODE 2   Infinite Scroll horizontal
// MODE 3   Infinite Scroll vertical and horizontal
// MODE 4   Scroll vertical (not infinite)
// MODE 5   Scroll horizontal (not infinite)
// MODE 6   Scroll vertical and horizontal (not infinite)
//
#define MODE_3

#endif // CLIONTESTPROJECT_TYPES_H

#ifndef CLIONTESTPROJECT_TYPES_H
#define CLIONTESTPROJECT_TYPES_H

#include <stdint.h>

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
#define SSD1306_SA 0x3C // Slave Address

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MODULE 8 // Module for tiles (bits) used for screen division / pages
#define EDGES 2 // Used only for pseudo buffer screen purposes

#define VIEWPORT_WIDTH SCREEN_WIDTH / MODULE      // 16
#define VIEWPORT_HEIGHT SCREEN_HEIGHT / MODULE    // 8

// MAXIMUM: 16rows x 18cols => 288 byte + 128 byte (SSD1306 page)
#define PLAYFIELD_COLS (VIEWPORT_WIDTH + EDGES) // AXIS X: vertical cols, map width size; min 16+2 (SCREEN_WIDTH / 8) + Edges
#define PLAYFIELD_ROWS (VIEWPORT_HEIGHT + EDGES) // AXIS Y: horizontal rows, map height size; min 8+2 (SCREEN_HEIGHT / 8) + Edges

// ALIAS
#define SCREEN_BUFFER_WIDTH PLAYFIELD_COLS
#define SCREEN_BUFFER_HEIGHT PLAYFIELD_ROWS
#define SCREEN_BUFFER_SIZE (PLAYFIELD_COLS * PLAYFIELD_ROWS)

#define TILEMAP_HEIGHT 29
#define TILEMAP_WIDTH PLAYFIELD_COLS

#define DIRECT_PORT
#define I2CPORT PORTB
#define SAFE_DELAY 0

// A bit set to 1 in the DDR is an output, 0 is an INPUT
#define I2CDDR DDRB
#define NUMBER_OF_SPRITES 1


// TYPEDEFS
// ////////////////////////////////////////////////////////////////////
typedef uint8_t byte;

// Define the structure of the config
typedef struct {
  int x;
  int y;
} config_t;

// Define the structure of the FX Objects
typedef struct tag_gfx_object {
  byte x;
  byte y;
  byte bType;
} GFX_OBJECT;

// TODO: Los tipos int son excesivos. Por encima del byte (uint8_t), estaría:
// - uint16_t con 65535; más que suficiente.

// Define the structure of the background
typedef struct background_game_typ {
  int x;
  int y;
  byte speed;
  _Bool direction;
  byte data[SCREEN_BUFFER_SIZE];
} background_game, *background_game_ptr;

typedef struct map_game_typ {
  byte width;
  byte height;
  int size;
  byte *data;
} map_game, *map_game_ptr;

// GLOBALS
// ////////////////////////////////////////////////////////////////////
background_game background;
map_game map;
//static GFX_OBJECT object_list[NUMBER_OF_SPRITES];

#endif // CLIONTESTPROJECT_TYPES_H

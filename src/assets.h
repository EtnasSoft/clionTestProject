#ifndef CLIONTESTPROJECT_ASSETS_H
#define CLIONTESTPROJECT_ASSETS_H

#include "types.h"

// 8x8px sprite: phantom (Pac Man)
// 8 bytes of mask followed by 8 bytes of pattern
const byte ucSprites[] _PROGMEM = {
    0x7C, 0xF6, 0x66, 0xFF, 0x7F, 0xF6, 0x66, 0xFC,
    0x7C, 0xF6, 0x66, 0xFF, 0x7F, 0xF6, 0x66, 0xFC
};

// 16x16px Mario Bros
// 32 bytes of mask followed by 32 bytes of pattern
const byte ucBigSprites[] _PROGMEM = {
    0xff, 0xff, 0xff, 0x0f, 0x07, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x07, 0x07, 0xaf, 0xff, 0xff, 0xff,
    0xff, 0x73, 0x21, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x01, 0x23, 0x7f, 0xff, 0xff,

    0x00, 0x00, 0x00, 0x00, 0x60, 0xb0, 0xf8, 0x98,
    0xb8, 0xd0, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x8c, 0xd6, 0xf2, 0x3f, 0x1f,
    0x3c, 0xf2, 0xdc, 0x80, 0x00, 0x00, 0x00, 0x00,
};

const byte ucTiles[] _PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Empty  (0)
    0xaa, 0xc1, 0xe8, 0xd5, 0xe8, 0xd5, 0xbe, 0x7f, // Brick (for demo!)
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Fill   (1)
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Fill   (2)
    0x7f, 0x21, 0x7d, 0x3d, 0x7d, 0x3f, 0x55, 0x00, // BRICK  (3)
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // =      (4)
    0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 0x00, // /      (5)
    0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00, // \      (6)

    0x54, 0x00, 0x05, 0x00, 0x51, 0xa8, 0xf1, 0x18, // ? Box 1/4 (7)
    0x11, 0xa8, 0x51, 0xe0, 0x01, 0x04, 0x01, 0xfe, // ? Box 2/4 (8)
    0xd5, 0x80, 0xa0, 0x80, 0x80, 0x80, 0x80, 0x8a, // ? Box 3/4 (9)
    0xb5, 0xb7, 0x81, 0x81, 0x80, 0xa0, 0x80, 0xff, // ? Box 4/4 (10)

    0x6a, 0x81, 0x80, 0xb5, 0x8c, 0x81, 0xc0, 0xff, // Mini Question box (11)
    0xaa, 0xc1, 0xe8, 0xd5, 0xe8, 0xd5, 0xbe, 0x7f, // Mini Brick bezeled (12)

    0x2c, 0x5e, 0xa6, 0xe0, 0xc0, 0x0c, 0xcc, 0xee, // Mini Floating Wall Left Corner (13)
    0xae, 0x0e, 0xe0, 0xea, 0xee, 0x0c, 0xe0, 0xee, // Mini Floating Wall Middel (14)
    0xde, 0x18, 0xc2, 0x9e, 0xda, 0x74, 0x38, 0x00, // Mini Floating Wall Right Corner (15)

    0xaa, 0x5f, 0xaa, 0x5f, 0xaa, 0x5f, 0xaa, 0x5f, // Gradient 100-75% (16)
    0x8a, 0x00, 0x2a, 0x00, 0x8a, 0x00, 0x2a, 0x00, // Gradient 75-25% (17)
};

// TODO: HEIGHT and WIDTH have to be the first two values of the array, like in Arduboy system...
const byte tileMap[TILEMAP_HEIGHT * TILEMAP_WIDTH] _PROGMEM = {


    /* 00 */ 0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 01 */ 0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 02 */ 0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 03 */ 0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 04 */ 0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 05 */ 0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 06 */ 0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 07 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,

    /* 08 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,  // -- FLOOR
    /* 09 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  0,
    /* 10 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  0,
    /* 11 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  0,
    /* 12 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  0,
    /* 13 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  0,
    /* 14 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,

    /* 15 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  0,
    /* 16 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  0,
    /* 17 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  0,
    /* 18 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  0,
    /* 19 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  0, // ?
    /* 21 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 22 */ 0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 23 */ 0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 24 */ 0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 25 */ 0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 26 */ 0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 27 */ 0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 28 */ 0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    /* 29 */ 0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
};

#endif // CLIONTESTPROJECT_ASSETS_H

#ifndef CLIONTESTPROJECT_ASSETS_H
#define CLIONTESTPROJECT_ASSETS_H

#include "types.h"

// El maximo para un mapa es de 128x8, o su equivalente en bits (1024).
// El siguiente escalado, 256x8 o 128*16 no cabe por muy poco... (2048)
// El límite del ATTiny85 es de 8192 bytes (+-8KB).
// Con el añadido de una dataflash de 4MB el problema se resolvería
// http://www.technoblogy.com/show?2XJD

// Las variables son static para declararlas globales y permitir así que
// otros ficheros las carguen. De no ser static, darían el error:
// multiple definition of...
// https://stackoverflow.com/questions/14526153/multiple-definition-of-a-global-variable

// 8x8px sprite: phantom (Pac Man)
// 8 bytes of mask followed by 8 bytes of pattern
static const byte ucSprites[] _PROGMEM = {
    0x7C, 0xF6, 0x66, 0xFF, 0x7F, 0xF6, 0x66, 0xFC,
    0x7C, 0xF6, 0x66, 0xFF, 0x7F, 0xF6, 0x66, 0xFC
};

// 16x16px Mario Bros
// 32 bytes of mask followed by 32 bytes of pattern
static const byte ucBigSprites[] _PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const byte ucTiles[] _PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Empty  (0)
    0x6a, 0x81, 0x80, 0xb5, 0x8c, 0x81, 0xc0, 0xff, // Mini Question box (11)
    0xaa, 0xc1, 0xe8, 0xd5, 0xe8, 0xd5, 0xbe, 0x7f, // Brick (for demo!)
    /*
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
    */
};

static const byte level[] _PROGMEM = {
    128, 8,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

/*static const byte level[] _PROGMEM = {
    64, 16,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
    0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,0,1,1,1,0,0,1,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,
    0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
    0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};*/

/*static const byte level[] _PROGMEM = {
    // Size (width x height)
    18, 29,

    // DATA
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,
    0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
    0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,
    0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,
    0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,
    0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,0,1,1,1,0,0,1,0,1,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,0,1,
    0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,
    0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,0,1,
    0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1,
    0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,
    0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,
    0,1,0,1,1,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,
    0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};*/

// Character set for text - stored in program memory
static const byte charMap[96][6] PROGMEM = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 },
    { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 },
    { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 },
    { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 },
    { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 },
    { 0x36, 0x49, 0x56, 0x20, 0x50, 0x00 },
    { 0x00, 0x08, 0x07, 0x03, 0x00, 0x00 },
    { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 },
    { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 },
    { 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x00 },
    { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 },
    { 0x00, 0x80, 0x70, 0x30, 0x00, 0x00 },
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 },
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },
    { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 },
    { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 },
    { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 },
    { 0x72, 0x49, 0x49, 0x49, 0x46, 0x00 },
    { 0x21, 0x41, 0x49, 0x4D, 0x33, 0x00 },
    { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 },
    { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 },
    { 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x00 },
    { 0x41, 0x21, 0x11, 0x09, 0x07, 0x00 },
    { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 },
    { 0x46, 0x49, 0x49, 0x29, 0x1E, 0x00 },
    { 0x00, 0x00, 0x14, 0x00, 0x00, 0x00 },
    { 0x00, 0x40, 0x34, 0x00, 0x00, 0x00 },
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 },
    { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 },
    { 0x02, 0x01, 0x59, 0x09, 0x06, 0x00 },
    { 0x3E, 0x41, 0x5D, 0x59, 0x4E, 0x00 },
    { 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00 },
    { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 },
    { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 },
    { 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 },
    { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 },
    { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 },
    { 0x3E, 0x41, 0x41, 0x51, 0x73, 0x00 },
    { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 },
    { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 },
    { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 },
    { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 },
    { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 },
    { 0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x00 },
    { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 },
    { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 },
    { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 },
    { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 },
    { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 },
    { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 },
    { 0x03, 0x01, 0x7F, 0x01, 0x03, 0x00 },
    { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 },
    { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 },
    { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 },
    { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 },
    { 0x03, 0x04, 0x78, 0x04, 0x03, 0x00 },
    { 0x61, 0x59, 0x49, 0x4D, 0x43, 0x00 },
    { 0x00, 0x7F, 0x41, 0x41, 0x41, 0x00 },
    { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 },
    { 0x00, 0x41, 0x41, 0x41, 0x7F, 0x00 },
    { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 },
    { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 },
    { 0x00, 0x03, 0x07, 0x08, 0x00, 0x00 },
    { 0x20, 0x54, 0x54, 0x78, 0x40, 0x00 },
    { 0x7F, 0x28, 0x44, 0x44, 0x38, 0x00 },
    { 0x38, 0x44, 0x44, 0x44, 0x28, 0x00 },
    { 0x38, 0x44, 0x44, 0x28, 0x7F, 0x00 },
    { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 },
    { 0x00, 0x08, 0x7E, 0x09, 0x02, 0x00 },
    { 0x18, 0xA4, 0xA4, 0x9C, 0x78, 0x00 },
    { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 },
    { 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00 },
    { 0x20, 0x40, 0x40, 0x3D, 0x00, 0x00 },
    { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 },
    { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 },
    { 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 },
    { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 },
    { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 },
    { 0xFC, 0x18, 0x24, 0x24, 0x18, 0x00 },
    { 0x18, 0x24, 0x24, 0x18, 0xFC, 0x00 },
    { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 },
    { 0x48, 0x54, 0x54, 0x54, 0x24, 0x00 },
    { 0x04, 0x04, 0x3F, 0x44, 0x24, 0x00 },
    { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 },
    { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 },
    { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 },
    { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 },
    { 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x00 },
    { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 },
    { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 },
    { 0x00, 0x00, 0x77, 0x00, 0x00, 0x00 },
    { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 },
    { 0x02, 0x01, 0x02, 0x04, 0x02, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 }
};

#endif // CLIONTESTPROJECT_ASSETS_H

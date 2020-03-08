#include <Arduino.h>
#include <avr/interrupt.h>
#include "types.h"
#include "assets.h"
#include "main.h"
#include "i2c_driver.h"
#include "i2c_hardware.h"
#include "screen_driver.h"

// Rotary encoder **********************************************
#define EncoderClick A0 // A0 PB5, pin 1 (RESET)
const int EncoderA = 2; // PB2, pin 7 (INT0)
const int EncoderB = 1; // PB1, pin 6

volatile int a0;
volatile int c0;

// Player and Game vars **********************************************
bool backgroundDirection = 0;
byte backgroundSpeed = 1;

static byte bPlayfield[PLAYFIELD_ROWS * PLAYFIELD_COLS];
static int iScrollX, iScrollY;

// Draw 1 character space that's vertically shifted
void DrawShiftedChar(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff) {
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
void DrawSprites(byte y, byte *pBuf, GFX_OBJECT *pList, byte bCount) {
  byte i, x, bSize, bSprite, *s, *d;
  byte cOld, cNew, mask, bYOff, bWidth;

  GFX_OBJECT *pObject;
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
      if (pObject->y + 8 <= y) // special case - only bottom half drawn
        s += 16;
      bYOff = pObject->y & 7;
      bWidth = 16;
      if (128 - pObject->x < 16)
        bWidth = 128 - pObject->x;
      // 4 possible cases:
      // byte aligned - single source, not shifted
      // single source (shifted, top or bottom row)
      // double source (middle) both need shifting
      if (bYOff == 0) // simplest case
      {
        for (x = 0; x < bWidth; x++) {
          cOld = d[0];
          mask = pgm_read_byte(s);
          cNew = pgm_read_byte(s + 32);
          s++;
          cOld &= mask;
          cOld |= cNew;
          *d++ = cOld;
        }
      } else if (pObject->y + 8 < y) // only bottom half of sprite drawn
      {
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
      } else if (pObject->y > y) // only top half of sprite drawn
      {
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
      } else // most difficult - part of top and bottom drawn together
      {
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
    } else // 8x8 sprite
    {
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
void DrawPlayfield(byte bScrollX, byte bScrollY) {
  byte bTemp[SCREEN_WIDTH]; // holds data for the current scan line
  byte x, y, tx;
  int ty, bXOff, bYOff;
  byte c, *s, *sNext, *d;
  int iOffset, iOffset2, cIndex, cIndex2;

  // Solo es cero cuando el scroll completa un MODULO su eje X (8 unidades)
  bXOff = bScrollX & (MODULE - 1);
  bYOff = bScrollY & (MODULE - 1);

  // Incrementa una unidad cada vez que el scroll completa un MODULO en Y
  ty = (bScrollY >> 3) + (EDGES / 2);

  adjustPlayField();

  // -------------------------------------------------------

  // draw the 8 rows
  for (y = 0; y < VIEWPORT_HEIGHT; y++) {
    memset(bTemp, 0, sizeof(bTemp));

    if (ty >= PLAYFIELD_ROWS) {
      ty -= PLAYFIELD_ROWS;
    }

    tx = (bScrollX >> 3) + (EDGES / 2);

    // Draw the playfield characters at the given scroll position
    d = bTemp;

    // partial characters vertically means a lot more work :(
    // Only for vertical scroll:
    if (bYOff) {
      for (x = 0; x < VIEWPORT_WIDTH; x++) {
        if (tx >= PLAYFIELD_COLS) {
          tx -= PLAYFIELD_COLS; // wrap around
        }

        iOffset = (tx + ty * PLAYFIELD_COLS);
        iOffset2 = (iOffset + PLAYFIELD_COLS); // next line

        if (iOffset2 >= (PLAYFIELD_ROWS * PLAYFIELD_COLS)) {    // past bottom
          iOffset2 -= (PLAYFIELD_ROWS * PLAYFIELD_COLS);
        }

        cIndex = iOffset % (PLAYFIELD_ROWS * PLAYFIELD_COLS);
        c = bPlayfield[cIndex];
        s = (byte *)&ucTiles[(c * MODULE) + bXOff];

        cIndex2 = iOffset2 % (PLAYFIELD_ROWS * PLAYFIELD_COLS);
        c = bPlayfield[cIndex2];
        sNext = (byte *)&ucTiles[(c * MODULE) + bXOff];
        // ------------------------------------------------------------------------------

        DrawShiftedChar(s, sNext, d, bXOff, bYOff);

        d += (MODULE - bXOff);
        bXOff = 0;
        tx++;
      }

      // partial character left to draw
      if (d != &bTemp[SCREEN_WIDTH]) {
        bXOff = (byte)(&bTemp[SCREEN_WIDTH] - d);
        if (tx >= PLAYFIELD_COLS)
          tx -= PLAYFIELD_COLS;
        iOffset = tx + ty * PLAYFIELD_COLS;
        iOffset2 = iOffset + PLAYFIELD_COLS; // next line
        if (iOffset2 >= (PLAYFIELD_ROWS * PLAYFIELD_COLS))     // past bottom
          iOffset2 -= (PLAYFIELD_ROWS * PLAYFIELD_COLS);
        c = bPlayfield[iOffset];
        s = (byte *)&ucTiles[c * MODULE];
        c = bPlayfield[iOffset2];
        sNext = (byte *)&ucTiles[c * MODULE];
        DrawShiftedChar(s, sNext, d, MODULE - bXOff, bYOff);
      }
      // simpler case of vertical offset of 0 for each character
    } else {
      //-----------------------------------
      // NON BYOFF; SCREEN BYTE COMPLETED!
      //-----------------------------------

      // Filling each col of the SCREEN_WIDTH
      for (x = 0; x < VIEWPORT_WIDTH; x++) {
        if (tx >= PLAYFIELD_COLS) {
          tx -= PLAYFIELD_COLS;
        }

        iOffset = tx + (ty * PLAYFIELD_COLS);
        cIndex = iOffset % (PLAYFIELD_ROWS * PLAYFIELD_COLS);
        c = bPlayfield[cIndex];
        s = (byte *)&ucTiles[(c * MODULE) + bXOff];
        _memcpy(d, s, MODULE - bXOff);
        d += (MODULE - bXOff);
        bXOff = 0;
        tx++;
      }

      // partial character left to draw
      // De momento el codigo no pasa por aqui.
      if (d != &bTemp[SCREEN_WIDTH]) {
        bXOff = (byte)(&bTemp[SCREEN_WIDTH] - d);
        if (tx >= PLAYFIELD_COLS) {
          tx -= PLAYFIELD_COLS;
        }
        iOffset = tx + ty * PLAYFIELD_COLS;
        c = bPlayfield[iOffset];
        s = (byte *)&ucTiles[c * MODULE];
        _memcpy(d, s, bXOff);
      }
    }

    //DrawSprites(y * VIEWPORT_HEIGHT, bTemp, object_list, NUMBER_OF_SPRITES);
    // Send it to the display
    screen_driver_set_position(0, y);
    i2c_driver_write_data(bTemp, SCREEN_WIDTH);
    ty++;
  }
}

void reloadPlayField() {
  byte x, y, *d, bitStart,
      iStart = iScrollY >> 3;

  for (y = 0; y < PLAYFIELD_ROWS; y++) {
    bitStart = ((iStart + y) * PLAYFIELD_COLS) % (PLAYFIELD_COLS * PLAYFIELD_ROWS);
    d = &bPlayfield[bitStart];

    for (x = 0; x < PLAYFIELD_COLS; x++) {
      _memcpy(d++, &tileMap[((iStart + y + 1) % TILEMAP_HEIGHT)  * PLAYFIELD_COLS + x], 1);
    }
  }
}

void adjustPlayField() {
  byte *d1, *d2;
  byte currentRow = (iScrollY >> 3) + (EDGES / 2);

  int playFieldLength = PLAYFIELD_ROWS * PLAYFIELD_COLS;

  int nextPlayfieldBit = (currentRow + VIEWPORT_HEIGHT) * PLAYFIELD_COLS,
      cNextPlayfieldBit = nextPlayfieldBit % playFieldLength,
      cNextRow = (currentRow + VIEWPORT_HEIGHT + 1) % TILEMAP_HEIGHT;

  int prevPlayfieldBit = currentRow * PLAYFIELD_COLS,
      cPrevPlayfieldBit = prevPlayfieldBit % playFieldLength,
      cPrevRow = (currentRow + 1) % TILEMAP_HEIGHT;

  d1 = &bPlayfield[cNextPlayfieldBit];
  d2 = &bPlayfield[cPrevPlayfieldBit];

  // The formula for get [y][x] in a linear array (1D) is:
  // y * COLS + x
  // Note that y must be `1 index`.
  for (byte x1 = 0; x1 < PLAYFIELD_COLS; x1++) {
    _memcpy(d1 + x1, &tileMap[cNextRow * PLAYFIELD_COLS + x1], 1);
    _memcpy(d2 + x1, &tileMap[cPrevRow * PLAYFIELD_COLS + x1], 1);
  }
}

// Interrupt handler **********************************************
// Called when encoder value changes
// Button interrupt, INT0, PB2, pin7
void moveBackgroundTo(_Bool increment) {
  // LINEAL
  if (increment) {
//    iScrollX += backgroundSpeed;
    iScrollY = backgroundDirection
      ? iScrollY + backgroundSpeed
      : iScrollY - backgroundSpeed;
  } else {
//    iScrollX -= backgroundSpeed;
    iScrollY = backgroundDirection
      ? iScrollY - backgroundSpeed
      : iScrollY + backgroundSpeed;
  }

  // DIAGONAL
  /*if (increment) {
    iScrollY += backgroundSpeed;
    iScrollX = backgroundDirection
               ? iScrollX + backgroundSpeed
               : iScrollX - backgroundSpeed;
  } else {
    iScrollY -= backgroundSpeed;
    iScrollX = backgroundDirection
               ? iScrollX - backgroundSpeed
               : iScrollX + backgroundSpeed;
  }*/
}

// TODO
// Esto deberia ser un metodo 'read_spinner' de un fichero spinner.c y desde ahí, llamar al controls.h:
// if (a==b) { controls_move_background_to_left(); } else { controls_move_background_to_right(); }
void moveBackground() {
  int a = PINB>>EncoderA & 1;
  int b = PINB>>EncoderB & 1;

  // A changed
  if (a != a0) {
    a0 = a;
    if (b != c0) {
      c0 = b;
      moveBackgroundTo(a == b);
    }
  }
}

void setup() {
  delay(50); // wait for the OLED to fully power up
  screen_driver_init(0, 0);

  attachInterrupt(0, moveBackground, CHANGE); //INT0, PB2, pin7

  iScrollX = 0;
  iScrollY = 0;

  reloadPlayField();

  /*memset(object_list, 0, sizeof(object_list));

  object_list[0].bType = 0x80; // big sprite
  object_list[0].x = 14;
  object_list[0].y = 40;*/
}

void loop() {
  // put your main code here, to run repeatedly:
  int speed = 0;

  while (1) {
    if (backgroundDirection) {
      //iScrollX += backgroundSpeed;
//      iScrollY += backgroundSpeed;
    } else {
      //iScrollX -= backgroundSpeed;
//      iScrollY -= backgroundSpeed;
    }


    DrawPlayfield(iScrollX, iScrollY);

    // Desde aquí se puede definir la velocidad a la que responde el juego:
    // Estaría bien sacar el valor a una variable:
    // (++speed % 3) => Modulo 3 (33% speed)
    // (++speed & 3) => Modulo 4 (25% speed)

    //if ((++speed % 3) == 0) { // Modulo 3 (33% speed)
    if (1 == 1) { // Modulo 3 (33% speed)
      if (speed > 10000) {
        speed = 0;
      }

      if (iScrollX >= PLAYFIELD_COLS * MODULE) {
        iScrollX = 0;
        reloadPlayField();
      } else if (iScrollX < 0) {
        iScrollX = (PLAYFIELD_COLS * MODULE) - 1;
        reloadPlayField();
      }

      if (iScrollY >= 240) {
        iScrollY = 8;
        reloadPlayField();
      } else if (iScrollY < 0) {
        iScrollY = 232;
        reloadPlayField();
      }

      if (analogRead(EncoderClick) < 940) {
        backgroundDirection = !backgroundDirection;
      }
    }
  }
}

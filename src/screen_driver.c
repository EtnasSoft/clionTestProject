// INCLUDES
// ////////////////////////////////////////////////////////////////////
// https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
#include "types.h"
#include "screen_driver.h"

#ifndef TEST
#include <Arduino.h>
#include "i2c_driver.h"
#else
#include <string.h>
#include "../test/support/avr/stub_io.h"
#include "../test/support/Arduino/wiring.h"
#endif

/*
https://github.com/aklomp/attiny85-ssd1306/blob/master/src/ssd1306.c
https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/OLEDDisplay.h
*/


// DEFINES
// ////////////////////////////////////////////////////////////////////
// Command registers:
#define SSD1306_SETCONTRAST 		0x81
#define SSD1306_DISPLAYALLON_RESUME 	0xA4
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_DISPLAYOFF 		0xAE
#define SSD1306_DISPLAYON 		0xAF
#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETCOMPINS 		0xDA
#define SSD1306_SETVCOMDETECT 		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETPRECHARGE 		0xD9
#define SSD1306_ENABLE_CHARGE_PUMP	0x8D
#define SSD1306_SETMULTIPLEX 		0xA8
#define SSD1306_SETSTARTLINE 		0x40
#define SSD1306_MEMORYMODE 		0x20
#define SSD1306_HV_COLUMN_ADDRESS	0x21
#define SSD1306_HV_PAGE_ADDRESS		0x22
#define SSD1306_PAM_PAGE_START		0xB0
#define SSD1306_COMSCANDEC 		0xC8
#define SSD1306_SEGREMAP 		0xA0


// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
void screen_driver_init(byte flip, byte invert) {
  unsigned char oled_initbuf[] = {
      0x00,
      SSD1306_DISPLAYOFF,
      SSD1306_SETDISPLAYCLOCKDIV,	0xF0,
      SSD1306_SETMULTIPLEX,		0x3F,
      SSD1306_SETPRECHARGE,		0xF1,
      SSD1306_SETDISPLAYOFFSET,	        0x00,
      SSD1306_SETSTARTLINE,		0x00,
      0xA1,       // Scan direction AXIS X: [0xA0, 0xA1]
      SSD1306_ENABLE_CHARGE_PUMP,	0x14,
      SSD1306_MEMORYMODE,		0x00, //0x00: horizontal, 0x01: vertical, 0x02: Page Addressing Mode      SSD1306_SEGREMAP + 1,
      SSD1306_COMSCANDEC,
      SSD1306_SETCOMPINS,		0x10,
      SSD1306_SETCONTRAST,		0xCF,
      SSD1306_SETVCOMDETECT,		0x40,
      SSD1306_DISPLAYALLON_RESUME,
      SSD1306_NORMALDISPLAY,
      SSD1306_HV_COLUMN_ADDRESS,	0, 127,
      SSD1306_HV_PAGE_ADDRESS,	        0, 7,
      SSD1306_DISPLAYON,

  };

  I2CDDR &= ~(1 << SSD1306_SDA);
  I2CDDR &= ~(1 << SSD1306_SCL); // let them float high
  I2CPORT |= (1 << SSD1306_SDA); // set both lines to get pulled up
  I2CPORT |= (1 << SSD1306_SCL);

  i2c_driver_write(oled_initbuf, sizeof(oled_initbuf));

  if (invert) {
    screen_driver_invert();
  }

  // rotate display 180
  if (flip) {
    screen_driver_rotate();
  }
}

void screen_driver_invert(void) {
    unsigned char uc[2];
    uc[0] = 0x00; // Command Introducer
    uc[1] = SSD1306_INVERTDISPLAY; // Inverse Display command
    i2c_driver_write(uc, 2);
}

void screen_driver_rotate(void) {
    unsigned char uc[2];
    uc[0] = 0x00; // Command Introducer
    uc[1] = SSD1306_SEGREMAP; // Set Segment Remap command
    i2c_driver_write(uc, 2);

    uc[1] = 0xc0; // Set Output Scan Direction
    i2c_driver_write(uc, 2);
}

void screen_driver_write_command(unsigned char c) {
  unsigned char buf[2];
  buf[0] = 0x00; // Command Introducer
  buf[1] = c;

  i2c_driver_write(buf, 2);
}

void screen_driver_write_command2(unsigned char c, unsigned char d) {
  unsigned char buf[3];
  buf[0] = 0x00; // Command Introducer
  buf[1] = c;
  buf[2] = d;

  i2c_driver_write(buf, 3);
}

void screen_driver_shutdown(void) {
  screen_driver_write_command(SSD1306_DISPLAYOFF); // Set Display OFF
}

void screen_driver_set_contrast(unsigned char contrast) {
  // 256 contrast steps from 00h to FFh
  screen_driver_write_command2(SSD1306_SETCONTRAST, contrast); // Set Contrast Control
}

void screen_driver_set_position(byte x, byte y) {
  screen_driver_write_command(SSD1306_PAM_PAGE_START | y); // go to page Y
  screen_driver_write_command(0x00 | (x & 0xf));        // lower col addr
  screen_driver_write_command(0x10 | ((x >> 4) & 0xf)); // upper col addr
}

/**
 * screen_driver_fill
 *
 * Patterns:
 * 0x00 Blank
 * 0xff Fill
 * 0xaa odd lines
 * 0x55 even lines
 *
 * @param ucData
 */
void screen_driver_fill(unsigned char data) {
  byte x, y;
  unsigned char temp[VIEWPORT_WIDTH];

  memset(temp, data, VIEWPORT_WIDTH);
  for (y = 0; y < VIEWPORT_HEIGHT; y++) {
    screen_driver_set_position(0, y);
    for (x = 0; x < VIEWPORT_HEIGHT; x++) {
      i2c_driver_write_data(temp, VIEWPORT_WIDTH);
    }
  }
}

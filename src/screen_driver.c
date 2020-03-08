/**
 * https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */
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

Command registers:
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


*/

void screen_driver_init(int flip, int invert) {
  unsigned char oled_initbuf[] = {
      0x00,       // Starting to send a commands sequence or script
      0xAE,       // Display off
      0xA8, 0x3F, // Set MUX Ratio 0x3F = 63; 0-63 (64) rows
      0xD3, 0x00, // Set display offset to 0
      0x40,       // Set Display Start Line to 0
      0xA1,       // Scan direction AXIS X: [0xA0, 0xA1]
      0xC8,       // Scan direction AXIS Y (invert screen): [0xC0, 0xC8]
      0xDA, 0x12, // Mapping COM pins
      0x81, 0xAA, // Setting constrast control
      0xA4,       // Disable Entire Display ON (GDDRAM)
      0xA6,       // Set normal display mode
      0xD5, 0x80, // Set OSC Frequency
      0x8D, 0x14, // Enable charge pump regulator
      0xAF,       // Set display on

      /*0x21, 0x00, 0x7f, 0x22, 0x00, 0x07,*/

      0x20, 0x02  // Set memory addressing mode [0x00 - Horizontal, 0x01 - Verticial, 0x02 - Page]
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
    uc[1] = 0xa7; // Inverse Display command
    i2c_driver_write(uc, 2);
}

void screen_driver_rotate(void) {
    unsigned char uc[2];
    uc[0] = 0x00; // Command Introducer
    uc[1] = 0xa0; // Set Segment Remap command
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
  screen_driver_write_command(0xAE); // Set Display OFF
}

void screen_driver_set_contrast(unsigned char contrast) {
  // 256 contrast steps from 00h to FFh
  screen_driver_write_command2(0x81, contrast); // Set Contrast Control
}

void screen_driver_set_position(int x, int y) {
  /*
    command_introducer = 0x00;
    command_start_page_address = 0xb0;
    command_set_higher_column = 0x10;
    command_set_lower_column = 0x0F;
  */
  screen_driver_write_command(0xb0 | y);                // go to page Y
  screen_driver_write_command(0x00 | (x & 0xf));        // lower col addr
  screen_driver_write_command(0x10 | ((x >> 4) & 0xf)); // upper col addr
}

void screen_driver_fill(unsigned char data) {
  int x;
  unsigned char temp[SCREEN_WIDTH];

  memset(temp, data, SCREEN_WIDTH);

  for (x = 0; x < VIEWPORT_HEIGHT; x++) {
    screen_driver_set_position(0, x);
    i2c_driver_write_data(temp, SCREEN_WIDTH);
  }
}


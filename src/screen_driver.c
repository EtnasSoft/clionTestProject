#include "types.h"
#include "config.h"
#include "i2c_driver.h"
#include "screen_driver.h"

#ifdef TEST
#include "../test/support/avr/stub_io.h"
#include "../test/support/Arduino/wiring.h"
#else
#include <Arduino.h>
#endif

void screen_init(int bFlip, int bInvert) {
  unsigned char uc[4];
  // TODO: sacar todos estos valores a #defines, que queda chulo!
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

  if (bInvert) {
    screen_invert();
  }

  // rotate display 180
  if (bFlip) {
    screen_rotate();
  }
}

void screen_invert(void) {
    unsigned char uc[2];
    uc[0] = 0;    // command
    uc[1] = 0xa7; // invert command
    i2c_driver_write(uc, 2);
}

void screen_rotate(void) {
    unsigned char uc[2];
    uc[0] = 0; // command
    uc[1] = 0xa0;
    i2c_driver_write(uc, 2);

    uc[1] = 0xc0;
    i2c_driver_write(uc, 2);
}

void screen_shutdown(void) {

}

void screen_write_command(unsigned char c) {

}

void screen_write_command2(unsigned char c, unsigned char d) {

}

void screen_set_contrast(unsigned char ucContrast) {

}

void screen_set_position(int x, int y) {

}

void screen_fill(unsigned char ucData) {

}


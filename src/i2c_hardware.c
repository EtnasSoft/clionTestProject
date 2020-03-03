#include "types.h"
#include "config.h"
#include "i2c_hardware.h"

#ifdef TEST
#include "../test/support/avr/stub_io.h"
#include "../test/support/Arduino/wiring.h"
#else
#include <Arduino.h>
//#include <wiring.c>
// #include "../test/support/avr/stub_io.h"
// #include "../test/support/Arduino/wiring.h"
#endif

void test_delay_microseconds(int delaytime) {
  delayMicroseconds(delaytime);
}

void i2c_hardware_begin(byte addr) {
//  I2CPORT |= ((1 << SSD1306_SDA) + (1 << SSD1306_SCL));
  I2CPORT |= ((1 << SSD1306_SDA) + (1 << SSD1306_SCL)); // esto es mio para ir probando...
//  I2CDDR |= ((1 << SSD1306_SDA) + (1 << SSD1306_SCL));
//  I2CPORT &= ~(1 << SSD1306_SDA); // data line low first
 delayMicroseconds(SAFE_DELAY);
//  I2CPORT &= ~(1 << SSD1306_SCL); // then clock line low is a START signal
 i2c_hardware_byte_out(addr << 1);     // send the slave address
}

void i2c_hardware_write(byte *data, int datalen) {}

void i2c_hardware_byte_out(byte b) {
  // byte i;
  // byte bOld = I2CPORT & ~((1 << SSD1306_SDA) | (1 << SSD1306_SCL));

  //for (i = 0; i < 8; i++) {
    // bOld &= ~(1 << SSD1306_SDA);
    // if (b & 0x80)
    //   bOld |= (1 << SSD1306_SDA);
    // I2CPORT = bOld;
    // delayMicroseconds(SAFE_DELAY);
    // I2CPORT |= (1 << SSD1306_SCL);
    // delayMicroseconds(SAFE_DELAY);
    // I2CPORT = bOld;
    // b <<= 1;
  //}

  // I2CPORT = bOld & ~(1 << SSD1306_SDA); // set data low
  delayMicroseconds(SAFE_DELAY);
  // I2CPORT |= (1 << SSD1306_SCL); // toggle clock
  delayMicroseconds(SAFE_DELAY);
  // I2CPORT = bOld;
}

void i2c_hardware_end(void) {}

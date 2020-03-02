#include "types.h"
#include "i2c_hardware.h"
#include "config.h"

#ifdef TEST
#include "../test/support/Arduino/wiring.h"
#else
#include <wiring.c> // En realidad, sería Arduino.h y lo que sea...
#endif

void test_delay_microseconds(int delaytime) {
  delayMicroseconds(delaytime);
}

void i2c_hardware_begin(byte addr) {
//  I2CPORT |= ((1 << SSD1306_SDA) + (1 << SSD1306_SCL));
//  I2CDDR |= ((1 << SSD1306_SDA) + (1 << SSD1306_SCL));
//  I2CPORT &= ~(1 << SSD1306_SDA); // data line low first
//  delayMicroseconds(SAFE_DELAY);
//  I2CPORT &= ~(1 << SSD1306_SCL); // then clock line low is a START signal
//  i2cByteOut(addr << 1);     // send the slave address
}

void i2c_hardware_write(byte *data, int datalen) {}

void i2c_hardware_byte_out(byte b) {}

void i2c_hardware_end(void) {}

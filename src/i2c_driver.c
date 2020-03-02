#include "config.h"
#include "i2c_driver.h"
#include "i2c_hardware.h"

void i2c_driver_init(byte slave_address) {

}

void i2c_driver_write(byte *data, int datalen) {
    i2c_hardware_begin(SSD1306_SA);
    i2c_hardware_write(data, datalen);
    i2c_hardware_end();
}

void i2c_driver_write_data(byte *data, int datalen) {
    i2c_hardware_begin(SSD1306_SA);
    i2c_hardware_byte_out(0x40);
    i2c_hardware_write(data, datalen);
    i2c_hardware_end();
}
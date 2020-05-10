// INCLUDES
// ////////////////////////////////////////////////////////////////////
#ifndef TEST
#include "i2c_driver.h"
#include "i2c_hardware.h"
#endif


// GLOBALS
// ////////////////////////////////////////////////////////////////////
byte slave_address;


// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
void i2c_driver_init(byte addr) {
    slave_address = addr;
}

void i2c_driver_write(byte *data, int datalen) {
    i2c_hardware_begin(slave_address);
    i2c_hardware_write(data, datalen);
    i2c_hardware_end();
}

void i2c_driver_write_data(byte *data, int datalen) {
    i2c_hardware_begin(slave_address);
    i2c_hardware_byte_out(0x40);
    i2c_hardware_write(data, datalen);
    i2c_hardware_end();
}

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H
#include "types.h"

void i2c_driver_init(byte slave_address);
void i2c_driver_write(byte *data, int datalen);
void i2c_driver_write_data(byte *data, int datalen);

#endif // I2C_DRIVER_H

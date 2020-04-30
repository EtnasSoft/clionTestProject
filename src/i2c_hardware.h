#ifndef I2C_HARDWARE_H
#define I2C_HARDWARE_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"


// PROTOTYPES
// ////////////////////////////////////////////////////////////////////
void i2c_hardware_begin(byte slave_address);
void i2c_hardware_write(byte *data, int datalen);
void i2c_hardware_byte_out(byte b);
void i2c_hardware_end(void);

#endif // I2C_HARDWARE_H

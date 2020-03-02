#include "unity.h"
#include "types.h"
#include "i2c_driver.h"
#include "mock_i2c_hardware.h"

byte slave_address;
byte data[4];

void setUp(void) {
    slave_address = 0x3C;
}

void tearDown(void) {}

void test_i2c_driver_NeedToImplement(void) {
  TEST_IGNORE_MESSAGE("Need to Implement i2c_driver");
}

void test_i2c_driver_write_should_run_the_correct_sequence(void) {
    int datalen = 2;
    data[0] = 0x00;
    data[1] = 0xff;

    // Sequence: begin, write, end;
    i2c_hardware_begin_Expect(slave_address);
    i2c_hardware_write_Expect(data, datalen);
    i2c_hardware_end_Expect();

    i2c_driver_write(data, datalen);
}

void test_i2c_driver_data_write_should_run_the_correct_sequence(void) {
    int datalen = 2;
    data[0] = 0x00;
    data[1] = 0xff;

    byte byte_out_command = 0x40;

    // Sequence: begin, write, end;
    i2c_hardware_begin_Expect(slave_address);
    i2c_hardware_byte_out_Expect(byte_out_command);
    i2c_hardware_write_Expect(data, datalen);
    i2c_hardware_end_Expect();

    i2c_driver_write_data(data, datalen);
}
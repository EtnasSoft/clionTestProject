#include "types.h"
#include "i2c_hardware.h"
#include "../test/support/avr/stub_io.h"
#include "mock_wiring.h"

#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_delay_microseconds_should_be_mocked(void) {
  int expected_time = 100;

  delayMicroseconds_Expect(expected_time);
  test_delay_microseconds(expected_time);
}

void test_i2c_hardware_begin_should_wait_for_a_safe_delay() {
  byte addr = 0x40;
  unsigned int save_delay = 0;

  delayMicroseconds_Expect(save_delay);
  delayMicroseconds_Ignore();

  i2c_hardware_begin(addr);
}

void test_log() {

}
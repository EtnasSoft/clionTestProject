#include "types.h"
#include "i2c_hardware.h"
#include "mock_wiring.h"
#include "unity.h"

/*
 hay que crear la cadena completa para los metodos del AVR
 Es decir, enlazar correctamente los iotn85, iotnx5, sfr_defs... para que al cargar uno, se carguen todos como ocurre
 en la cadena real...
 */


void setUp(void) {}

void tearDown(void) {}

void test_i2c_hardware_NeedToImplement(void) {
  TEST_IGNORE_MESSAGE("Need to Implement i2c_hardware");
}

void test_delay_microseconds_should_be_mocked(void) {
  int expected_time = 100;

  delayMicroseconds_Expect(expected_time);
  test_delay_microseconds(expected_time);
}

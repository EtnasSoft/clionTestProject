#include "unity.h"
#include "screen_driver.h"
#include "mock_i2c_driver.h"

int bFlip;
int bInvert;

void setUp(void) {
    // TODO Muchas dudas con respecto a la notacion hungara!!!
    bFlip = 0;
    bInvert = 0;
}

void tearDown(void) {}

void test_screen_init_should_sends_the_correct_sequence_to_driver(void) {
    unsigned char uc[2];
    int number_of_commands = 22;

    i2c_driver_write_Expect(uc, number_of_commands);
    i2c_driver_write_IgnoreArg_data();

    screen_init(bFlip, bInvert);
}

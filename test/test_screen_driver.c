#include "types.h"
#include "mock_i2c_driver.h"
#include "screen_driver.h"
#include "unity.h"
#include <string.h>

int flip;
int invert;
int number_of_init_commands;

void setUp(void) {
  flip = 0;
  invert = 0;
  number_of_init_commands = 22;
}

void tearDown(void) {}

void test_screen_init_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];

  i2c_driver_write_Expect(uc, number_of_init_commands);
  i2c_driver_write_IgnoreArg_data();

  screen_driver_init(flip, invert);
}

void test_screen_init_inverted_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];

  // First call (can be ignored)
  i2c_driver_write_Expect(uc, number_of_init_commands);
  i2c_driver_write_IgnoreArg_data();

  // The call for invert command
  uc[0] = 0; // Command Introducer
  uc[1] = 0xa7; // Inverse Display command
  i2c_driver_write_Expect(uc, 2);

  screen_driver_init(flip, 0x01);
}

void test_screen_init_rotated_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];
  unsigned char uc2[2];
  unsigned char uc3[2];

  // First call (can be ignored)
  i2c_driver_write_Expect(uc, number_of_init_commands);
  i2c_driver_write_IgnoreArg_data();

  // Then rotate:
  // First call
  uc2[0] = 0; // Command Introducer
  uc2[1] = 0xa0; // Set Segment Remap command
  i2c_driver_write_Expect(uc2, 2);

  // Second call
  uc3[0] = 0; // Command Introducer
  uc3[1] = 0xc0; // Set Output Scan Direction
  i2c_driver_write_Expect(uc3, 2);

  screen_driver_init(0x01, invert);
}

void test_screen_init_inverted_and_rotated_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];
  unsigned char uc2[2];
  unsigned char uc3[2];
  unsigned char uc4[2];

  // First call (can be ignored)
  i2c_driver_write_Expect(uc, number_of_init_commands);
  i2c_driver_write_IgnoreArg_data();

  // Then invert:
  uc2[0] = 0; // Command Introducer
  uc2[1] = 0xa7; // Inverse Display command
  i2c_driver_write_Expect(uc2, 2);

  // Then rotate:
  // First call
  uc3[0] = 0; // Command Introducer
  uc3[1] = 0xa0; // Set Segment Remap command
  i2c_driver_write_Expect(uc3, 2);

  // Second call for rotate
  uc4[0] = 0; // Command Introducer
  uc4[1] = 0xc0; // Set Output Scan Direction
  i2c_driver_write_Expect(uc4, 2);

  screen_driver_init(0x01, 0x01);
}

void test_screen_invert_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];

  // The call for invert command
  uc[0] = 0; // Command Introducer
  uc[1] = 0xa7; // Inverse Display command
  i2c_driver_write_Expect(uc, 2);

  screen_driver_invert();
}

void test_screen_rotate_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char uc[2];
  unsigned char uc2[2];

  // First call
  uc[0] = 0; // Command Introducer
  uc[1] = 0xa0; // Set Segment Remap command
  i2c_driver_write_Expect(uc, 2);

  // Second call
  uc2[0] = 0; // Command Introducer
  uc2[1] = 0xc0; // Set Output Scan Direction
  i2c_driver_write_Expect(uc2, 2);

  screen_driver_rotate();
}

void test_screen_driver_write_command_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char command = 0xc0;

  unsigned char buf[2];
  buf[0] = 0x00; // Command Introducer
  buf[1] = command;

  i2c_driver_write_Expect(buf, 2);

  screen_driver_write_command(command);
}

void test_screen_driver_write_command2_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char command1 = 0xc0;
  unsigned char command2 = 0xc0;

  unsigned char buf[3];
  buf[0] = 0x00; // Command Introducer
  buf[1] = command1;
  buf[2] = command2;

  i2c_driver_write_Expect(buf, 3);

  screen_driver_write_command2(command1, command2);
}

void test_screen_driver_shutdown_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char shutdown_command = 0xAE; // Set Display OFF
  unsigned char buf[2];
  buf[0] = 0x00; // Command Introducer
  buf[1] = shutdown_command;

  i2c_driver_write_Expect(buf, 2);

  screen_driver_write_command(shutdown_command);
}

void test_screen_driver_set_contrast_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char contrast = 0xCC; // 256 contrast steps from 00h to FFh
  unsigned char buf[3];
  buf[0] = 0x00; // Command Introducer
  buf[1] = 0x81; // Set Contrast Control
  buf[2] = contrast;

  i2c_driver_write_Expect(buf, 3);

  screen_driver_set_contrast(contrast);
}

void test_screen_driver_set_position_should_sends_the_correct_sequence_to_driver(void) {
  int x = 0;
  int y = 0;

  unsigned char command_introducer = 0x00;
  unsigned char command_start_page_address = 0xb0;
  unsigned char command_set_higher_column = 0x10;
  unsigned char command_set_lower_column = 0x0F;

  unsigned char buf[2];
  unsigned char buf2[2];
  unsigned char buf3[2];

  buf[0] = 0x00; // Command Introducer
  buf[1] = command_start_page_address | y;
  i2c_driver_write_Expect(buf, 2);

  buf2[0] = 0x00; // Command Introducer
  buf2[1] = command_introducer | (x & command_set_lower_column);
  i2c_driver_write_Expect(buf2, 2);

  buf3[0] = 0x00; // Command Introducer
  buf3[1] = command_set_higher_column | ((x >> 4) & command_set_lower_column);
  i2c_driver_write_Expect(buf3, 2);

  screen_driver_set_position(x, y);
}

void test_screen_driver_fill_should_sends_the_correct_sequence_to_driver(void) {
  unsigned char data = 0xff;
  unsigned char temp[SCREEN_WIDTH];
  memset(temp, data, SCREEN_WIDTH);

  // Testear esto es muy complejo sin contadores:
  // Se llama a la funciones:
  //  - screen_driver_set_position
  //  - i2c_driver_write_data
  // 8 veces (VIEWPORT_HEIGHT), y screen_driver_set_position, llama a su ves, 3 veces al
  //  - i2c_driver_write

  i2c_driver_write_Ignore();

  // VIEWPORT_HEIGHT times...
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);
  i2c_driver_write_data_Expect(temp, SCREEN_WIDTH);

  screen_driver_fill(data);
}

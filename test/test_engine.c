#include "unity.h"
#include "types.h"
#include "engine.h"
#include "helpers.h"
#include "assets_manager.h"
#include "mock_stub_pgmspace.h"
#include "./fixtures/assetsFixture.h"

#define EMULATOR 1

int default_background_x;
int default_background_y;
int default_background_x_old;
int default_background_y_old;
int default_background_x_offset;
int default_background_y_offset;
int default_background_x_page;
int default_background_y_page;
byte default_background_speed;
_Bool default_background_direction;

void setUp(void) {
  engine_background_init(&background);

  default_background_x = 0;
  default_background_y = 0;
  default_background_x_old = 0;
  default_background_y_old = 0;
  default_background_x_offset = 0;
  default_background_y_offset = 0;
  default_background_x_page = 0;
  default_background_y_page = 0;
  default_background_speed = 1;
  default_background_direction = 0;
}

void tearDown(void) {}

void test_engine_background_init_should_create_a_background(void) {
  TEST_ASSERT_EQUAL_INT(default_background_x, background.x);
  TEST_ASSERT_EQUAL_INT(default_background_y, background.y);
  TEST_ASSERT_EQUAL_INT(default_background_x_old, background.x_old);
  TEST_ASSERT_EQUAL_INT(default_background_y_old, background.y_old);
  TEST_ASSERT_EQUAL_INT(default_background_x_offset, background.x_offset);
  TEST_ASSERT_EQUAL_INT(default_background_y_offset, background.y_offset);
  TEST_ASSERT_EQUAL_INT(default_background_x_page, background.x_page);
  TEST_ASSERT_EQUAL_INT(default_background_y_page, background.y_page);
  TEST_ASSERT_EQUAL_INT(default_background_speed, background.speed);
}

void test_engine_background_set_should_set_the_given_params_in_background(void) {
  int new_background_x = 150;
  int new_background_y = 10;
  int new_background_speed = 1;
  int new_background_direction = 1;

  const background_game new_params = {
      .x = new_background_x,
      .y = new_background_y,
      .speed = new_background_speed,
      .direction = new_background_direction
  };

  engine_background_set(&new_params);

  TEST_ASSERT_EQUAL_INT(new_background_x, background.x);
  TEST_ASSERT_EQUAL_INT(new_background_y, background.y);
  TEST_ASSERT_EQUAL_INT(new_background_speed, background.speed);
}

void test_engine_background_config_get_should_retrieves_the_current_background_params(void) {
  TEST_ASSERT_EQUAL_INT(default_background_x, engine_background_get()->x);
  TEST_ASSERT_EQUAL_INT(default_background_y, engine_background_get()->y);
  TEST_ASSERT_EQUAL_INT(default_background_x_old, engine_background_get()->x_old);
  TEST_ASSERT_EQUAL_INT(default_background_y_old, engine_background_get()->y_old);
  TEST_ASSERT_EQUAL_INT(default_background_x_page, engine_background_get()->x_page);
  TEST_ASSERT_EQUAL_INT(default_background_y_page, engine_background_get()->y_page);
  TEST_ASSERT_EQUAL_INT(default_background_speed, engine_background_get()->speed);
  TEST_ASSERT_EQUAL_INT(default_background_direction, engine_background_get()->direction);
}

void test_engine_background_set_pos_should_compute_the_correct_values(void) {
  int new_background_x = 10;
  int new_background_y = 20;
  int expected_background_x_offset = new_background_x & 7;
  int expected_background_y_offset = new_background_y & 7;
  int expected_background_x_page = new_background_x >> 3;
  int expected_background_y_page = new_background_y >> 3;

  engine_background_set_pos(new_background_x, new_background_y);

  TEST_ASSERT_EQUAL_INT(expected_background_x_offset, background.x_offset);
  TEST_ASSERT_EQUAL_INT(expected_background_y_offset, background.y_offset);
  TEST_ASSERT_EQUAL_INT(expected_background_x_page, background.x_page);
  TEST_ASSERT_EQUAL_INT(expected_background_y_page, background.y_page);
}

void test_engine_reset_background_set_pos_should_compute_the_correct_values(void) {
  int new_background_x = 0;
  int new_background_y = 0;
  int expected_background_x_offset = 0;
  int expected_background_y_offset = 0;
  int expected_background_x_page = 0;
  int expected_background_y_page = 0;

  engine_background_set_pos(new_background_x, new_background_y);

  TEST_ASSERT_EQUAL_INT(expected_background_x_offset, background.x_offset);
  TEST_ASSERT_EQUAL_INT(expected_background_y_offset, background.y_offset);
  TEST_ASSERT_EQUAL_INT(expected_background_x_page, background.x_page);
  TEST_ASSERT_EQUAL_INT(expected_background_y_page, background.y_page);
}

void test_engine_background_reload_at_zero_position_should_realocate_the_proper_bytes(void) {

}

void test_engine_background_reload_at_final_position_should_relocate_the_proper_bytes(void) {

}

void test_engine_background_adjust_should_relocate_bytes_into_screen(void) {

}

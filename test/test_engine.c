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
int default_background_old_x;
int default_background_old_y;
byte default_background_speed;
_Bool default_background_direction;

void setUp(void) {
  engine_background_init(&background);

  default_background_x = 0;
  default_background_y = 0;
  default_background_old_x = 0;
  default_background_old_y = 0;
  default_background_speed = 1;
  default_background_direction = 0;
}

void tearDown(void) {}

void test_engine_background_init_should_create_a_background(void) {
  TEST_ASSERT_EQUAL_INT(default_background_x, background.x);
  TEST_ASSERT_EQUAL_INT(default_background_y, background.y);
  TEST_ASSERT_EQUAL_INT(default_background_old_x, background.old_x);
  TEST_ASSERT_EQUAL_INT(default_background_old_y, background.old_y);
  TEST_ASSERT_EQUAL_INT(default_background_speed, background.speed);
  TEST_ASSERT_EQUAL_INT(default_background_direction, background.direction);
}

void test_engine_background_set_should_set_the_given_params_in_background(void) {
  int new_background_x = 666;
  int new_background_y = 999;
  int new_background_speed = 10;
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
  TEST_ASSERT_EQUAL_INT(new_background_direction, background.direction);
}

void test_engine_background_config_get_should_retrieves_the_current_background_params(void) {
  TEST_ASSERT_EQUAL_INT(default_background_x, engine_background_get()->x);
  TEST_ASSERT_EQUAL_INT(default_background_y, engine_background_get()->y);
  TEST_ASSERT_EQUAL_INT(default_background_speed, engine_background_get()->speed);
  TEST_ASSERT_EQUAL_INT(default_background_direction, engine_background_get()->direction);
}

void test_engine_background_reload_at_zero_position_should_realocate_the_proper_bytes(void) {

}

void test_engine_background_reload_at_final_position_should_relocate_the_proper_bytes(void) {

}

void test_engine_background_adjust_should_relocate_bytes_into_screen(void) {

}

#include "unity.h"
#include "types.h"
#include "engine.h"

int default_background_x;
int default_background_y;
byte default_background_speed;
_Bool default_background_direction;

void setUp(void) {
  default_background_x = 0;
  default_background_y = 0;
  default_background_speed = 0;
  default_background_direction = 0;
}

void tearDown(void) {}

void test_engine_background_init_should_create_a_background(void) {
  engine_background_init(&background);

  TEST_ASSERT_EQUAL_INT(default_background_x, background.x);
  TEST_ASSERT_EQUAL_INT(default_background_y, background.y);
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

  engine_background_init(&background);
  engine_background_set(&new_params);

  TEST_ASSERT_EQUAL_INT(new_background_x, background.x);
  TEST_ASSERT_EQUAL_INT(new_background_y, background.y);
  TEST_ASSERT_EQUAL_INT(new_background_speed, background.speed);
  TEST_ASSERT_EQUAL_INT(new_background_direction, background.direction);
}

void test_engine_background_config_get_should_retrieves_the_current_background_params(void) {
  engine_background_init(&background);

  TEST_ASSERT_EQUAL_INT(default_background_x, engine_background_get()->x);
  TEST_ASSERT_EQUAL_INT(default_background_y, engine_background_get()->y);
  TEST_ASSERT_EQUAL_INT(default_background_speed, engine_background_get()->speed);
  TEST_ASSERT_EQUAL_INT(default_background_direction, engine_background_get()->direction);
}

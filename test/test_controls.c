#include "unity.h"

#include "types.h"
#include "controls.h"

void setUp(void) {
  background.x = 10;
  background.y = 10;
  background.speed = 1;
  background.direction = 0;
}

void tearDown(void) {}

void test_controls_move_background_to_up_should_increments_its_y_axis_value(void) {
  int expected_y_pos = background.y + background.speed;

  controls_move_background_to_up(&background);
  TEST_ASSERT_EQUAL_INT(expected_y_pos, background.y);

  controls_move_background_to_up(&background);
  TEST_ASSERT_EQUAL_INT(++expected_y_pos, background.y);

  controls_move_background_to_up(&background);
  TEST_ASSERT_EQUAL_INT(++expected_y_pos, background.y);
}

void test_controls_move_background_to_down_should_decrements_its_y_axis_value(void) {
  int expected_y_pos = background.y - background.speed;

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(expected_y_pos, background.y);

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(--expected_y_pos, background.y);

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(--expected_y_pos, background.y);
}

void test_controls_move_background_to_left_should_decrements_its_y_axis_value(void) {
  int expected_x_pos = background.x - background.speed;

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(expected_x_pos, background.x);

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(--expected_x_pos, background.x);

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(--expected_x_pos, background.x);
}

void test_controls_move_background_to_right_should_increments_its_y_axis_value(void) {
  int expected_x_pos = background.x + background.speed;

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(expected_x_pos, background.x);

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(++expected_x_pos, background.x);

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(++expected_x_pos, background.x);
}

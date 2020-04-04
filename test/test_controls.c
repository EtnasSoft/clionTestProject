#include "unity.h"
#include "types.h"
#include "controls.h"
#include "engine.h"

void setUp(void) {
  background.x = 10;
  background.y = 10;
  background.x_old = 10;
  background.y_old = 10;
  background.x_offset = background.x & 7;
  background.y_offset = background.y & 7;
  background.x_page = background.x >> 3;
  background.y_page = background.y >> 3;
  background.speed = 1;
  background.direction = 0;

  engine_background_set(&background);
}

void tearDown(void) {}

void test_controls_move_background_to_up_should_increments_its_y_axis_value(void) {
  int expected_y_pos = background.y + background.speed;
  int expected_y_page = expected_y_pos >> 3;
  int expected_y_offset = expected_y_pos & 7;

  controls_move_background_to_up(&background);

  TEST_ASSERT_EQUAL_INT(expected_y_pos, background.y);
  TEST_ASSERT_EQUAL_INT(expected_y_offset, background.y_offset);
  TEST_ASSERT_EQUAL_INT(expected_y_page, background.y_page);
  TEST_ASSERT_NOT_EQUAL(background.y_old, background.y);

  controls_move_background_to_up(&background);
  TEST_ASSERT_EQUAL_INT(++expected_y_pos, background.y);
  TEST_ASSERT_NOT_EQUAL(background.y_old, background.y);

  controls_move_background_to_up(&background);
  TEST_ASSERT_EQUAL_INT(++expected_y_pos, background.y);
  TEST_ASSERT_NOT_EQUAL(background.y_old, background.y);
}

void test_controls_move_background_to_down_should_decrements_its_y_axis_value(void) {
  int expected_y_pos = background.y - background.speed;
  int expected_y_page = expected_y_pos >> 3;
  int expected_y_offset = expected_y_pos & 7;

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(expected_y_pos, background.y);
  TEST_ASSERT_EQUAL_INT(expected_y_offset, background.y_offset);
  TEST_ASSERT_EQUAL_INT(expected_y_page, background.y_page);
  TEST_ASSERT_NOT_EQUAL(background.y_old, background.y);

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(--expected_y_pos, background.y);

  controls_move_background_to_down(&background);
  TEST_ASSERT_EQUAL_INT(--expected_y_pos, background.y);
}

void test_controls_move_background_to_left_should_decrements_its_y_axis_value(void) {
  int expected_x_pos = background.x - background.speed;
  int expected_x_page = expected_x_pos >> 3;
  int expected_x_offset = expected_x_pos & 7;

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(expected_x_pos, background.x);
  TEST_ASSERT_EQUAL_INT(expected_x_offset, background.x_offset);
  TEST_ASSERT_EQUAL_INT(expected_x_page, background.x_page);
  TEST_ASSERT_NOT_EQUAL(background.x_old, background.x);

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(--expected_x_pos, background.x);

  controls_move_background_to_left(&background);
  TEST_ASSERT_EQUAL_INT(--expected_x_pos, background.x);
}

void test_controls_move_background_to_right_should_increments_its_y_axis_value(void) {
  int expected_x_pos = background.x + background.speed;
  int expected_x_page = expected_x_pos >> 3;
  int expected_x_offset = expected_x_pos & 7;

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(expected_x_pos, background.x);
  TEST_ASSERT_EQUAL_INT(expected_x_offset, background.x_offset);
  TEST_ASSERT_EQUAL_INT(expected_x_page, background.x_page);
  TEST_ASSERT_NOT_EQUAL(background.x_old, background.x);

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(++expected_x_pos, background.x);

  controls_move_background_to_right(&background);
  TEST_ASSERT_EQUAL_INT(++expected_x_pos, background.x);
}

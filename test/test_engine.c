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
byte default_background_speed;
_Bool default_background_direction;

void setUp(void) {
  engine_background_init(&background);

  default_background_x = 0;
  default_background_y = 0;
  default_background_speed = 0;
  default_background_direction = 0;
}

void tearDown(void) {
//  engine_background_init(&background);
//  engine_background_reload(&background, &map);
}

void test_engine_background_init_should_create_a_background(void) {
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
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[0], 1, level_1[0]);
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[1], 1, level_1[1]);

  assets_manager_map_init(&map, level_1);
  engine_background_reload(&background, &map);

  // Check first byte in background
  TEST_ASSERT_EQUAL_INT(level_1[2], background.data[0]);

  // Check last byte in background
  TEST_ASSERT_EQUAL_INT(level_1[SCREEN_BUFFER_SIZE + 1], background.data[SCREEN_BUFFER_SIZE - 1]);

#ifdef EMULATOR
//  helpers_print_map(&map);
//  helpers_print_background(&background, &map);
#endif
}

void test_engine_background_reload_at_final_position_should_relocate_the_proper_bytes(void) {
  byte page_to_check = 3;

  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[0], 1, level_1[0]);
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[1], 1, level_1[1]);

  assets_manager_map_init(&map, level_1);
  background.y = VIEWPORT_HEIGHT * page_to_check;

  engine_background_reload(&background, &map);

  byte background_y_page = (background.y >> 3) - 1;
  int expected_first_byte_pos = (background_y_page + SCREEN_BUFFER_HEIGHT) * SCREEN_BUFFER_WIDTH;
  int new_byte_to_check = background_y_page * SCREEN_BUFFER_WIDTH;

  // Check the first new byte in each row of the background
  while (page_to_check > 0) {
    /*printf("Background_y_page: %i, background_data_pos: %i\n",
      background_y_page,
      new_byte_to_check + 1);

    printf("Expected new byte pos: %i. Byte there: %i. Byte to check: %i\n\n",
      expected_first_byte_pos,
      map.data[expected_first_byte_pos],
      background.data[new_byte_to_check]);*/

    TEST_ASSERT_EQUAL_INT(map.data[expected_first_byte_pos],
      background.data[new_byte_to_check]);

    expected_first_byte_pos -= SCREEN_BUFFER_WIDTH;
    new_byte_to_check -= SCREEN_BUFFER_WIDTH;
    page_to_check -= 1;
  }
#ifdef EMULATOR
  //helpers_print_map(&map);
  helpers_print_background(&background, &map);
#endif
}

void test_engine_background_adjust_should_relocate_bytes_into_screen(void) {
  byte page_to_check = 1;

  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[0], 1, level_1[0]);
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[1], 1, level_1[1]);

  assets_manager_map_init(&map, level_1);

  // Reload background from previous tests
  background.y = 0;
  engine_background_reload(&background, &map);

  // Adjust background
  background.y = VIEWPORT_HEIGHT * page_to_check;
  engine_background_adjust(&background, &map);

#ifdef EMULATOR
//  helpers_print_map(&map);
  // helpers_print_background(&background, &map);
#endif
}

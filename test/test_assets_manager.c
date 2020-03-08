#include "unity.h"
#include "types.h"
#include "helpers.h"
#include "assets_manager.h"
#include "mock_stub_pgmspace.h"
#include "./fixtures/assetsFixture.h"

#define EMULATOR true

byte* levels[1];

void setUp(void) {
  levels[0] = level_1;
}

void tearDown(void) {}

void test_assets_manager_map_init_should_be_create_the_map() {
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[0], 1, level_1[0]);
  pgm_read_byte_ExpectWithArrayAndReturn((byte *)&level_1[1], 1, level_1[1]);

  assets_manager_map_init(&map, levels[0]);

  TEST_ASSERT_EQUAL_INT(level_1[0], map.width);
  TEST_ASSERT_EQUAL_INT(level_1[1], map.height);
  TEST_ASSERT_EQUAL_INT(level_1[0] * level_1[1], map.size);

#ifdef EMULATOR
  helpers_print_map(&map);
#endif
}

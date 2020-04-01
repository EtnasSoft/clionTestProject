#include "unity.h"
#include "types.h"
#include "helpers.h"
#include "assets_manager.h"
#include "./fixtures/assetsFixture.h"

#define EMULATOR true

const uint8_t* const levels[] = {level_1};

void setUp(void) {
}

void tearDown(void) {}

void test_assets_manager_map_init_should_be_create_the_map() {
  assets_manager_map_init(&map, levels[0]);

  TEST_ASSERT_EQUAL_INT(level_1[0], map.width);
  TEST_ASSERT_EQUAL_INT(level_1[1], map.height);
  TEST_ASSERT_EQUAL_INT(level_1[0] * level_1[1], map.size);

#ifdef EMULATOR
  helpers_print_map(&map);
#endif
}

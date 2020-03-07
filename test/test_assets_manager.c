#include "unity.h"
#include "types.h"
#include "assets_manager.h"
#include "mock_stub_pgmspace.h"
#include "./fixtures/assetsFixture.h"

byte* levels[1];
byte default_map_width;
byte default_map_height;
byte default_map_data;

void setUp(void) {
  levels[0] = level_1;
}

void tearDown(void) {}

void test_assets_manager_map_init_should_be_create_the_map() {
  pgm_read_byte_ExpectWithArrayAndReturn((byte *) &level_1[0], 1, level_1[0]);
  pgm_read_byte_ExpectWithArrayAndReturn((byte *) &level_1[1], 1, level_1[1]);

  assets_manager_map_init(&map, levels[0]);

  TEST_ASSERT_EQUAL_INT(level_1[0], map.width);
  TEST_ASSERT_EQUAL_INT(level_1[1], map.height);

#ifdef EMULATOR
  printf("Map height: %i\n", map.height);
  printf("Map width: %i\n", map.width);
  for(int x = 0; x < (map.width * map.height); x++) {
    printf("%i", map.data[x]);
    if (x > 0 && (x + 1) % map.width == 0) {
      printf("\n");
    } else {
      printf(", ");
    }
  }
}
#endif

void test_assets_manager_map_load_should_be_load_the_given_map_data() {
//  assets_manager_map_init(&map, levels[0]);
//  assets_manager_load_map(levels[1]);


}

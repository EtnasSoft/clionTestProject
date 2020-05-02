#include "types.h"
//#include "plot.h"

#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_plot_NeedToImplement(void) {
  printf("_Bool size: %i\n", sizeof(_Bool));
  printf("int size: %i\n", sizeof(int));
  printf("uint8_t size: %i\n", sizeof(uint8_t));
  printf("uint16_t size: %i\n", sizeof(uint16_t));

  TEST_IGNORE_MESSAGE("Need to Implement plot");
}

#include "unity.h"
#include "config.h"

const config_t *globalConfig;

void setUp(void) {
  globalConfig = config_get();
}

void tearDown(void) {}

void testConfigShouldInitializeWithDefaultValues() {
  TEST_ASSERT_NOT_NULL(globalConfig);
}

void testConfigShouldReturnTheRequestedValue() {
  TEST_ASSERT_EQUAL(0, globalConfig->x);
  TEST_ASSERT_EQUAL(0, globalConfig->y);
}

void testConfigShouldBeSaveTheGivenValueIntoGlobalConfig() {
  const int newX = 666;
  const int newY = 999;

  const config_t newConfig = {
      .x = newX,
      .y = newY
  };

  config_set(&newConfig);

  TEST_ASSERT_EQUAL(newX, globalConfig->x);
  TEST_ASSERT_EQUAL(newY, globalConfig->y);
}

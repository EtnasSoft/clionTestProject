#include "config.h"

static config_t globalConfig = {};

const config_t *config_get(void) {
  return &globalConfig;
}

int config_set(const config_t *newconfig) {
  globalConfig = *newconfig;

  return 0;
}

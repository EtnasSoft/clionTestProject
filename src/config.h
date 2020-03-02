#ifndef CONFIG_H
#define CONFIG_H
#include "types.h"

// Prototypes
const config_t *config_get(void);
int config_set(const config_t *newconfig);

#endif // CONFIG_H

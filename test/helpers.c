#include "types.h"
#include "helpers.h"
#include <stdio.h>

void helpers_print_map(map_game_ptr map) {
  printf("Map height: %i\n", map->height);
  printf("Map width: %i\n", map->width);
  printf("Map size: %i\n", map->size);

  for (int x = 0; x < map->size; x++) {
    printf("%i", map->data[x]);
    if (x > 0 && (x + 1) % map->width == 0) {
      printf("\n");
    } else {
      printf(", ");
    }
  }

  printf("-----------------------------------------------------\n\n");
}

/*void helpers_print_background(background_game_ptr background, map_game_ptr map) {
  printf("Background X: %i\n", background->x);
  printf("Background Y: %i\n", background->y);
  printf("Background iStart: %i\n", background->y >> 3);

  printf("SCREEN_BUFFER_WIDTH: %i\n", SCREEN_BUFFER_WIDTH);
  printf("SCREEN_BUFFER_HEIGHT: %i\n", SCREEN_BUFFER_HEIGHT);

  for (int x = 0; x < (SCREEN_BUFFER_SIZE); x++) {
    printf("%i", background->data[x]);
    if (x > 0 && (x + 1) % SCREEN_BUFFER_WIDTH == 0) {
      printf("\n");
    } else {
      printf(", ");
    }
  }

  printf("-----------------------------------------------------\n\n");
}
*/

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"
#include "controls.h"


// FUNCTIONS
// ////////////////////////////////////////////////////////////////////
void controls_move_background_to_up(background_game_ptr background) {
  background->y += background->speed;
  background->y_offset = background->y & 7;
  background->y_page = background->y >> 3;
}

void controls_move_background_to_down(background_game_ptr background) {
  background->y -= background->speed;
  background->y_offset = background->y & 7;
  background->y_page = background->y >> 3;
}

void controls_move_background_to_left(background_game_ptr background) {
  background->x -= background->speed;
  background->x_offset = background->x & 7;
  background->x_page = background->x >> 3;
}

void controls_move_background_to_right(background_game_ptr background) {
  background->x += background->speed;
  background->x_offset = background->x & 7;
  background->x_page = background->x >> 3;
}

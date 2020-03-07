#include "controls.h"

void controls_move_background_to_up(background_game_ptr background) {
  background->y += background->speed;
}

void controls_move_background_to_down(background_game_ptr background) {
  background->y -= background->speed;
}

void controls_move_background_to_left(background_game_ptr background) {
  background->x -= background->speed;
}

void controls_move_background_to_right(background_game_ptr background) {
  background->x += background->speed;
}

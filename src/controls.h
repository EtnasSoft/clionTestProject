#ifndef CONTROLS_H
#define CONTROLS_H

#include "types.h"

void controls_move_background();

void controls_move_background_to_up(background_game_ptr background);
void controls_move_background_to_down(background_game_ptr background);
void controls_move_background_to_left(background_game_ptr background);
void controls_move_background_to_right(background_game_ptr background);

#endif // CONTROLS_H

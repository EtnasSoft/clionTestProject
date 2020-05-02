#ifndef PLOT_H
#define PLOT_H

// INCLUDES
// ////////////////////////////////////////////////////////////////////
#include "types.h"


// PROTOTYPES
// ////////////////////////////////////////////////////////////////////
void plot_point(byte x, byte y);
void draw_to(int x, int y, int x1, int y1);
void plot_char(byte x, byte y, byte c);
void plot_char_big(byte x, byte y, byte c);
void plot_text(byte x, byte y, PGM_P s);
void plot_big_text(byte x, byte y, PGM_P s);
void plot_integer (byte x, byte y, int number);
int stretch (int x);

#endif // PLOT_H

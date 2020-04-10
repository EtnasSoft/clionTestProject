#ifndef PLOT_H
#define PLOT_H
#include "types.h"

void plot_point(byte x, byte y);
void draw_to(int x, int y, int x1, int y1);
void plot_text(int x, int y, PGM_P s);
void plot_big_text(int x, int y, PGM_P s);
int stretch (int x);

#endif // PLOT_H

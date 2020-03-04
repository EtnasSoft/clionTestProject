#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

void screen_init(int bFlip, int bInvert);
void screen_invert(void);
void screen_rotate(void);
void screen_shutdown(void);
void screen_write_command(unsigned char c);
void screen_write_command2(unsigned char c, unsigned char d);
void screen_set_contrast(unsigned char ucContrast);
void screen_set_position(int x, int y);
void screen_fill(unsigned char ucData);

#endif // SCREEN_DRIVER_H

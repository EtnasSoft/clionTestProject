#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

void screen_driver_init(int flip, int invert);
void screen_driver_invert(void);
void screen_driver_rotate(void);
void screen_driver_shutdown(void);
void screen_driver_write_command(unsigned char c);
void screen_driver_write_command2(unsigned char c, unsigned char d);
void screen_driver_set_contrast(unsigned char contrast);
void screen_driver_set_position(int x, int y);
void screen_driver_fill(unsigned char data);

#endif // SCREEN_DRIVER_H

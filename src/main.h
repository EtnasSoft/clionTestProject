#ifndef MAIN_H
#define MAIN_H

#include "types.h"

// Prototypes **********************************************
void gameLoop(void);
void reloadPlayField();
void adjustPlayField();
void oledWriteCommand(unsigned char c);
void i2cByteOut(byte b);
void i2cBegin(byte addr);
void i2cWrite(byte *pData, byte bLen);
void i2cEnd();
void I2CWrite(byte *pData, int iLen);
void I2CWriteData(byte *pData, int iLen);
void oledInit(int bFlip, int bInvert);
void oledShutdown();
void oledWriteCommand(unsigned char c);
void oledWriteCommand2(unsigned char c, unsigned char d);
void oledSetContrast(unsigned char ucContrast);
void oledSetPosition(int x, int y);
void oledFill(unsigned char ucData);
void DrawShiftedChar(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
void DrawSprites(byte y, byte *pBuf, GFX_OBJECT *pList, byte bCount);
void DrawPlayfield(byte bScrollX, byte bScrollY);
void moveBackgroundTo(_Bool increment);
void moveBackground();
void setup();
void loop();
void reloadPlayField();
void adjustPlayField();

#endif // MAIN_H

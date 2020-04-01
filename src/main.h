#ifndef MAIN_H
#define MAIN_H

#include "types.h"

// Prototypes **********************************************
void gameLoop(void);
void reloadPlayField(void);
void adjustPlayField(void);
void i2cByteOut(byte b);
void i2cBegin(byte addr);
void i2cWrite(byte *pData, byte bLen);
void i2cEnd(void);
void I2CWrite(byte *pData, int iLen);
void I2CWriteData(byte *pData, int iLen);
void oledInit(int bFlip, int bInvert);
void oledShutdown(void);
void oledWriteCommand(unsigned char c);
void oledWriteCommand2(unsigned char c, unsigned char d);
void oledSetContrast(unsigned char ucContrast);
void oledSetPosition(int x, int y);
void oledFill(unsigned char ucData);
void DrawShiftedChar(byte *s1, byte *s2, byte *d, byte bXOff, byte bYOff);
void DrawSprites(byte y, byte *pBuf, GFX_OBJECT *pList, byte bCount);
void DrawPlayField(background_game background);
void moveBackgroundTo(_Bool increment);
void moveBackground(void);
void setup(void);
void loop(void);
void adjustPlayFieldRows(void);
void adjustPlayFieldCols(void);

#endif // MAIN_H

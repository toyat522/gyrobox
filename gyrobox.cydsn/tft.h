#ifndef TFT_H
#define TFT_H

#include "project.h"

#include "mpu.h"

void write8_a0(uint8 data);
void write8_a1(uint8 data);
void writeM8_a1(uint8 *pData, int N);
uint8 read8_a1(void);
void readM8_a1(uint8 *pData, int N);
void tftStart(void);
void TFT_SetOrientation(int mode);
void TFT_MatchDeviceOrientation(Orientation_t orientation);

#endif // TFT_H

/* [] END OF FILE */
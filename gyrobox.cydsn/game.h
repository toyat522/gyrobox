#ifndef GAME_H
#define GAME_H

#include "mpu.h"

void InitGame();
void ResetGame();
void EndGame();
void ProcessMap();
void GameLoop(MPU_DATA_t mpuData);

#endif

/* [] END OF FILE */
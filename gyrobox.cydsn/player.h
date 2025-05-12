#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

#include "mpu.h"

int GetPlayerX();
int GetPlayerY();
void InitPlayer(int startX, int startY);
uint8_t IsPlayerObsCollide();
uint8_t IsPlayerWallCollide();
uint8_t IsPlayerEndCollide();
void UpdatePlayer(MPU_DATA_t mpuData);
void DrawPlayer();

#endif  // PLAYER_H

/* [] END OF FILE */
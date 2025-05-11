#include "player.h"

#include "mpu.h"

static int x, y;

int getX() {
    return x;
}

int getY() {
    return y;
}

void updateGame(MPU_DATA_t mpuData) {
    mpuData.xAccel;
}
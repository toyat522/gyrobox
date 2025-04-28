#ifndef MPU_H
#define MPU_H

#include <stdint.h>

// Struct to hold the measurements from the MPU
typedef struct MPU_DATA {
  float x_gyro;
  float y_gyro;
  float z_gyro;
  float x_accel;
  float y_accel;
  float z_accel;
} MPU_DATA_t;

void MPU_Init();
MPU_DATA_t MPU_Read();
void MPU_ByteWrite(uint8_t reg, uint8_t byte);

#endif // MPU_H

/* [] END OF FILE */
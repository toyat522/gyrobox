#ifndef MPU_H
#define MPU_H

#include <stdint.h>

// Enum for discrete device orientation (defined
// based on which side is facing DOWN)
typedef enum {
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
  BACK,
  FRONT,
} Orientation_t;

// Struct to hold the measurements from the MPU
typedef struct {
  float xGyro;
  float yGyro;
  float zGyro;
  float xAccel;
  float yAccel;
  float zAccel;
} MPU_DATA_t;

// Struct to hold device rotation data
typedef struct {
  float r;
  float p;
  float y;
} ROT_DATA_t;

void MPU_Init();
MPU_DATA_t MPU_Read();
void MPU_ByteWrite(uint8_t reg, uint8_t byte);
ROT_DATA_t AccelToRot(MPU_DATA_t mpuData);
Orientation_t GetOrientation(MPU_DATA_t mpuData);
const char* OrientationToString(Orientation_t orientation);

#endif // MPU_H

/* [] END OF FILE */
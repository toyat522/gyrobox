#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include "mpu.h"

// Enum for device states
typedef enum {
  TIMER_DONE,
  TIMER_SET,
  AUDIO,
  GAME,
  TEMP,
} States_t;

States_t GetNextState(States_t prevState, Orientation_t orientation,
                      uint8_t isGamePlaying);
int TimerValueToCount(int *timerValue);
void CountToTimerValue(int *timerValue, int count);

#endif // UTILS_H

/* [] END OF FILE */
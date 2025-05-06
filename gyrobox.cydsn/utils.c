#include "utils.h"

/*
This function converts an array containing timer values
{tens digit of minutes, ones digit of minutes,
 tens digit of seconds, ones digit of seconds}
to seconds.
*/
int TimerValueToCount(int *timerValue) {
  int minutes = timerValue[0] * 10 + timerValue[1];
  int seconds = timerValue[2] * 10 + timerValue[3];
  return minutes * 60 + seconds;
}

/* [] END OF FILE */
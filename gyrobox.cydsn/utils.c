#include "utils.h"

#include <project.h>

/*
This function determines the state of the device.
*/
States_t GetNextState(States_t prevState, Orientation_t orientation,
                      uint8_t isGamePlaying) {
  if (TimerISR_IsCountDone())
    return TIMER_DONE;
  else if (isGamePlaying && prevState == GAME)
    return GAME;
  else if (orientation == BOTTOM)
    return TEMP;
  else if (orientation == RIGHT)
    return TIMER_SET;
  else if (orientation == TOP)
    return AUDIO;
  else if (orientation == BACK)
    return GAME;
  else
    return prevState;
}

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

/*
This function mutates an array containing timer values
{tens digit of minutes, ones digit of minutes,
 tens digit of seconds, ones digit of seconds}
from seconds
*/
void CountToTimerValue(int *timerValue, int count) {
  int minutes = count / 60;
  int seconds = count % 60;

  timerValue[0] = minutes / 10;
  timerValue[1] = minutes % 10;
  timerValue[2] = seconds / 10;
  timerValue[3] = seconds % 10;
}

/* [] END OF FILE */
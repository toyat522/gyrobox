#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H

#include <stdint.h>

void AudioSampleISR_StartAudio(unsigned char *newWav, unsigned int newLen);
void AudioSampleISR_StopAudio();

uint8_t TimerISR_IsTimerOn();
uint8_t TimerISR_IsCountDone();
uint8_t TimerISR_IsCountDoneOnce();
int TimerISR_GetCount();
int TimerISR_GetMaxCount();
void TimerISR_StopTimer();
void TimerISR_StartTimer(int val);

#endif // CYAPICALLBACKS_H

/* [] END OF FILE */

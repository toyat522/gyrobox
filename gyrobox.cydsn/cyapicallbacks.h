#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H
    
uint8_t TimerISR_IsCountDone();
uint8_t TimerISR_IsCountDoneOnce();
int TimerISR_GetCount();
void TimerISR_StopTimer();
void TimerISR_StartTimer(int val);
    
#endif

/* [] END OF FILE */

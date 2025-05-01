#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H
    
void TimerISR_SetMaxCount(int val);
uint8_t TimerISR_IsCountDone();
uint8_t TimerISR_IsCountDoneOnce();
int TimerISR_GetCount();
void TimerISR_ResetCount();
    
#endif

/* [] END OF FILE */

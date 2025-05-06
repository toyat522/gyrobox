#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

uint8_t IsBtnPressed();
uint8_t IsBtnPressedOnce();
uint16_t GetPotentiometer();
uint8_t GetDigit(int numDigits);
void ControlsUpdate();

#endif // CONTROLS_H

/* [] END OF FILE */
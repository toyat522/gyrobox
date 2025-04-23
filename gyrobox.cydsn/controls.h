#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

uint8_t IsBtnPressed();
uint8_t IsBtnPressedOnce();
int GetPotentiometer();
void ControlsUpdate();

#endif // CONTROLS_H

/* [] END OF FILE */
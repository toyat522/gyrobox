#include "controls.h"

#include "project.h"

// Tracks button state for debouncing and edge detection
static uint8_t currBtn = 0;
static uint8_t prevBtn = 0;

/*
This function returns true when the button is pressed.
*/
uint8_t IsBtnPressed() {
  return currBtn;
}

/*
This function returns true for only one cycle on the rising
edge of the button press.
*/
uint8_t IsBtnPressedOnce() {
  return currBtn && !prevBtn;
}

/*
This function updates the states for button controls.
It should only be called once at the end of the firmware loop.
*/
void ControlsUpdate() {
  prevBtn = currBtn;
  currBtn = ControlSW_Read();
}

/*
This function outputs the ADC result with no high or negative
values to get potentiometer output.
*/
int GetPotentiometer() {
  // Wait until ADC result is ready
  PotADC_IsEndConversion(PotADC_WAIT_FOR_RESULT);

  uint16_t adcResult = PotADC_GetResult16();
  if (adcResult & 0x8000) {
    // Ignore negative ADC results
    adcResult = 0;
  } else if (adcResult >= 0xfff) {
    // Ignore high ADC results
    adcResult = 0xfff;
  }
  return adcResult;
}

/* [] END OF FILE */
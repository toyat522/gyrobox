#include "controls.h"

#include "project.h"

// Maximum ADC value
const int MAX_ADC_VALUE = 0xfff;

// Tracks button and potentiometer states
static uint8_t currBtn = 0;
static uint8_t prevBtn = 0;
static uint16_t currPot = 0;

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
This function outputs the ADC result with no high or negative
values to get potentiometer output.
*/
uint16_t GetPotentiometer() {
  return currPot;
}

/*
This function converts the raw potentiometer value to a value from
0 to numDigits-1.
*/
uint8_t GetDigit(int numDigits) {
  return (currPot * numDigits) / (MAX_ADC_VALUE + 1);
}

/*
This function updates the states for button controls.
It should only be called once at the end of the firmware loop.
*/
void ControlsUpdate() {
  // Set previous and current button states
  prevBtn = currBtn;
  currBtn = ControlSW_Read();

  // Wait until ADC result is ready
  PotADC_IsEndConversion(PotADC_WAIT_FOR_RESULT);

  // Get raw potentiometer output
  uint16_t adcResult = PotADC_GetResult16();
  if (adcResult & 0x8000) {
    // Ignore negative ADC results
    adcResult = 0;
  } else if (adcResult >= MAX_ADC_VALUE) {
    // Ignore high ADC results
    adcResult = MAX_ADC_VALUE;
  }

  // Set potentiometer value
  currPot = adcResult;
}

/* [] END OF FILE */
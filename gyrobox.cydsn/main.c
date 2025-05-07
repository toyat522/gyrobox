#include <stdio.h>

#include "GUI.h"
#include "config.h"
#include "device.h"
#include "serial.h"
#include "utils.h"

int main() {
  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS operation for serial debugging
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  // Start the I2C and SPI masters
  I2C_Start();
  SPIM_Start();

  // Start the emWin graphic library
  GUI_Init();

  // Set the orientation of the TFT display
  TFT_MatchDeviceOrientation(BOTTOM);

  // Start and configure the gyro + accelorometer
  MPU_Init();

  // Start ADC conversions
  PotADC_Start();
  PotADC_StartConvert();

  // Start audio mux and DACs
  AudioMux_Start();
  WaveDAC_Start();
  StopAudio();

  // Store previous device state
  States_t prevState = TEMP;

  // Variables used in the different states
  int timerDigitIdx = 0;
  int timerValue[] = {0, 0, 0, 0, 0};
  uint8_t isDisplayC = 1;

  // Main loop
  for (;;) {

    // Collect device orientation data
    MPU_DATA_t mpuData = MPU_Read();
    Orientation_t orientation = GetOrientation(mpuData);

    // Determine the next device state
    States_t state = GetNextState(prevState, orientation);

    // If device state changed, rotate TFT
    if (prevState != state) {
      TFT_MatchDeviceOrientation(orientation);
    }

    // Run different functions based on the current state
    if (state == TIMER_DONE) {

      // Display timer end screen on TFT on the first iteration
      if (TimerISR_IsCountDoneOnce()) {
        GUI_SetFont(&GUI_Font24_1);
        GUI_SetColor(GUI_WHITE);
        GUI_DispStringAt("Timer completed!", 10, 70);
        GUI_DispStringAt("Press button to stop.", 10, 94);
      }

      // Make beeping sound
      Beep();

      // If button pressed, reset timer count and exit of out TimerLoop
      if (IsBtnPressed()) {
        TimerISR_StopTimer();
      }

    } else if (state == TIMER_SET) {

      if (TimerISR_IsTimerOn()) {

        // If timer is already running, display the current value
        int timeRemaining = TimerISR_GetMaxCount() - TimerISR_GetCount();
        CountToTimerValue(timerValue, timeRemaining);

        // Stop timer on button press
        if (IsBtnPressedOnce()) {
          TimerISR_StopTimer();
          memset(timerValue, 0, sizeof(timerValue));
          timerDigitIdx = 0;
        }

      } else {

        // Initialize state variables on state change
        if (prevState != state) {
          memset(timerValue, 0, sizeof(timerValue));
          timerDigitIdx = 0;
        }

        // Increment timer digit on button press
        if (IsBtnPressedOnce()) {
          timerDigitIdx++;

          // Once all four digits have been entered, start timer
          if (timerDigitIdx == 4) {
            int count = TimerValueToCount(timerValue);
            if (count == 0) {
              // If the timer value is 0, restart
              timerDigitIdx = 0;
            } else {
              // Otherwise, start the timer
              TimerISR_StartTimer(count);
            }
          }
        }

        // Update digit value based on potentiometer
        timerValue[timerDigitIdx] =
            timerDigitIdx % 2 ? GetDigit(10) : GetDigit(6);

      } 

      // Show timer value on the TFT display
      GUI_SetFont(&GUI_Font32_1);
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringAt(":", 45, 50);
      int i;
      for (i = 0; i < 4; i++) {
        // Highlight the current digit to modify
        if (timerDigitIdx == i)
          GUI_SetColor(GUI_RED);
        else
          GUI_SetColor(GUI_WHITE);

        // Display individual digits
        char timerStr[2];
        snprintf(timerStr, sizeof(timerStr), "%d", timerValue[i]);
        int offset = i >= 2 ? 8 : 0;
        GUI_DispStringAt(timerStr, 10 + 18 * i + offset, 50);
      }

    } else if (state == TEMP) {

      // Toggle display units on button press
      if (IsBtnPressedOnce()) {
        isDisplayC = !isDisplayC;
      }

      // Get temperature in celsius and fahrenheit
      float tempC = GetTemp();
      float tempF = CelsiusToFahrenheit(tempC);

      // Print result on device screen based on isDisplayC
      GUI_SetFont(&GUI_Font32_1);
      GUI_SetColor(GUI_WHITE);
      char tempStr[32];
      if (isDisplayC) {
        snprintf(tempStr, sizeof(tempStr), "%.0f degrees C    ", tempC);
      } else {
        snprintf(tempStr, sizeof(tempStr), "%.0f degrees F    ", tempF);
      }
      GUI_DispStringAt(tempStr, 10, 50);

    } else if (state == AUDIO) {

    }

    // Update previous device state
    prevState = state;

    // Update the button press states
    ControlsUpdate();
  }
}

/* [] END OF FILE */
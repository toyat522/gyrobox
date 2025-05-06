#include <stdio.h>

#include "GUI.h"
#include "config.h"
#include "device.h"
#include "serial.h"
#include "utils.h"

// Enum for device states
typedef enum {
  TIMER,
  AUDIO,
  GAME,
  TEMP,
} States_t;

/*
This function is the loop which runs when the timer finishes countdown.
*/
void TimerLoop() {
  // Display timer end screen on TFT on the first iteration
  if (TimerISR_IsCountDoneOnce()) {
    GUI_Clear();
    GUI_DispStringAt("Timer completed!", 10, 70);
    GUI_DispStringAt("Press button to stop.", 10, 90);
  }

  // Make beeping sound
  Beep();

  // If button pressed, reset timer count and exit of out TimerLoop
  if (IsBtnPressed()) {
    TimerISR_StopTimer();
  }

  // Update the button press states
  ControlsUpdate();
}

int main() {
  // Enable global interrupts
  CyGlobalIntEnable;

  // Initialize timer
  Timer_Start();

  // Start USBFS operation for serial debugging
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  // Start the I2C and SPI masters
  I2C_Start();
  SPIM_Start();

  // Start the emWin graphic library
  GUI_Init();

  // Set the orientation of the TFT display
  GUI_SetFont(&GUI_Font8x16);
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

  // Store device states
  States_t state = TIMER;
  States_t prevState = TIMER;

  // Variables used in the different states
  int timerDigitIdx = 0;
  int timerValue[] = {0, 0, 0, 0, 0};
  uint8_t isDisplayC = 0;

  // Main loop
  for (;;) {

    // Run the TimerLoop when the timer finishes countdown
    if (TimerISR_IsCountDone()) {
      TimerLoop();
      continue;
    }

    // Collect device orientation data
    MPU_DATA_t mpuData = MPU_Read();
    Orientation_t orientation = GetOrientation(mpuData);

    if (orientation == BOTTOM) {
      // Enable timer mode
      state = TIMER;

      // Initialize state variables on device flip
      if (prevState != state) {
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
          timerDigitIdx % 2 ? GetDigit(6) : GetDigit(10);

      // Show timer value on the TFT display
      char timerStr[32];
      snprintf(timerStr, sizeof(timerStr), "%d%d:%d%d",
          timerValue[0], timerValue[1], timerValue[2], timerValue[3]);
      GUI_DispStringAt(timerStr, 10, 50);

    } else if (orientation == RIGHT) {
      // Enable temperature display mode
      state = TEMP;

      // Toggle display units on button press
      if (IsBtnPressedOnce()) {
        isDisplayC = !isDisplayC;
      }

      // Get temperature in celsius and fahrenheit
      float tempC = GetTemp();
      float tempF = CelsiusToFahrenheit(tempC);

      // Print result on device screen based on isDisplayC
      char tempStr[32];
      if (isDisplayC) {
        snprintf(tempStr, sizeof(tempStr), "%.0f degrees C    ", tempC);
      } else {
        snprintf(tempStr, sizeof(tempStr), "%.0f degrees F    ", tempF);
      }
      GUI_DispStringAt(tempStr, 10, 50);

    } else if (orientation == TOP) {
      // Enable audio player mode
      state = AUDIO;

    } else if (orientation == BACK) {
      // Enable game mode
      state = GAME;
    }

    // If device state changed, rotate TFT
    if (prevState != state) {
      TFT_MatchDeviceOrientation(orientation);
    }

    // Update previous device state
    prevState = state;

    // Update the button press states
    ControlsUpdate();
  }
}

/* [] END OF FILE */
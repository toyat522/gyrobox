#include <stdio.h>

#include "config.h"
#include "device.h"
#include "GUI.h"
#include "serial.h"

int main() {

  // Initialize the interrupt and timer
  TimerISR_SetMaxCount(10);
  TimerISR_Start();
  Timer_Start();

  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS operation with 5V operation
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  // Start the I2C Master
  I2C_Start();

  // Start the SPI Master
  SPIM_Start();

  // Start the emWin graphic library
  GUI_Init();

  // Set the orientation of the TFT display
  TFT_SetOrientation(1);

  // Clear the TFT screen
  GUI_Clear();
  GUI_SetFont(&GUI_Font8x16);

  // Start and configure the gyro + accelorometer
  MPU_Init();

  // Start ADC conversions
  PotADC_Start();
  PotADC_StartConvert();

  // Start audio mux and DACs
  AudioMux_Start();
  WaveDAC_Start();

  // Initially stop audio
  StopAudio();

  // Store previous device orientation
  Orientation_t prevOrientation = BOTTOM;

  // Main loop
  for (;;) {
    float tempC = GetTemp();
    int pot = GetPotentiometer();
    MPU_DATA_t mpuData = MPU_Read();
    ROT_DATA_t rotData = AccelToRot(mpuData);
    Orientation_t orientation = GetOrientation(mpuData);
    int count = TimerISR_GetCount();

    // Clear the screen the first time timer completes
    if (TimerISR_IsCountDoneOnce()) {
      GUI_Clear();

      // Display timer end screen on TFT
      GUI_SetFont(&GUI_Font8x16);
      GUI_DispStringAt("Timer completed!", 10, 70);
      GUI_DispStringAt("Rotate device to stop.", 10, 90);
    }

    if (TimerISR_IsCountDone()) {

      // Make beeping noise
      SendWav1();
      CyDelay(200);
      StopAudio();
      CyDelay(100);
      SendWav1();
      CyDelay(200);
      StopAudio();
      CyDelay(200);

    } else {

      SerialPrintln("Hello World!");
      SerialPrint("Temperature: ");
      SerialPrintlnf(tempC);
      SerialPrint("Potentiometer: ");
      SerialPrintlnf(pot);

      char tempStr[32];
      snprintf(tempStr, sizeof(tempStr),
              "Temperature: %.2f      ", tempC);
      GUI_DispStringAt(tempStr, 10, 50);

      char potStr[32];
      snprintf(potStr, sizeof(potStr),
              "Potentiometer: %d      ", pot);
      GUI_DispStringAt(potStr, 10, 70);

      char gyroStr[32];
      snprintf(gyroStr, sizeof(gyroStr),
              "Gyro: (%.2f, %.2f, %.2f)      ",
              mpuData.xGyro, mpuData.yGyro, mpuData.zGyro);
      GUI_DispStringAt(gyroStr, 10, 90);

      char accelStr[32];
      snprintf(accelStr, sizeof(accelStr),
              "Accel: (%.2f, %.2f, %.2f)      ",
              mpuData.xAccel, mpuData.yAccel, mpuData.zAccel);
      GUI_DispStringAt(accelStr, 10, 110);

      char rotStr[32];
      snprintf(rotStr, sizeof(rotStr),
              "RPY: (%.2f, %.2f, %.2f)      ",
              rotData.r, rotData.p, rotData.y);
      GUI_DispStringAt(rotStr, 10, 130);

      char oriStr[32];
      snprintf(oriStr, sizeof(oriStr),
              "Orientation: %s      ",
              OrientationToString(orientation));
      GUI_DispStringAt(oriStr, 10, 150);

      char countStr[32];
      snprintf(countStr, sizeof(countStr),
              "Count: %d      ",
              count);
      GUI_DispStringAt(countStr, 10, 170);

    }

    // If device orientation changed, rotate TFT
    if (prevOrientation != orientation) {
      GUI_Clear();
      switch (orientation) {
      case TOP:
        TFT_SetOrientation(3);
        break;
      case BOTTOM:
        TFT_SetOrientation(1);
        break;
      case LEFT:
        TFT_SetOrientation(2);
        break;
      case RIGHT:
        TFT_SetOrientation(0);
        break;
      default:
        TFT_SetOrientation(1);
      }
      TimerISR_ResetCount();
    }

    // Update the button press states
    ControlsUpdate();

    // Update the previous state
    prevOrientation = orientation;

    CyDelay(100);
  }
}

/* [] END OF FILE */
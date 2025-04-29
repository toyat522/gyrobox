#include <stdio.h>

#include "config.h"
#include "device.h"
#include "GUI.h"
#include "serial.h"

int main() {

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

  GUI_Clear();
  GUI_SetFont(&GUI_Font8x16);

  // Start and configure the gyro + accelorometer
  MPU_Init();

  // Start ADC conversions
  PotADC_Start();
  PotADC_StartConvert();

  // Start audio multiplexer
  AudioMux_Start();

  //SendWav1();

  // Main loop
  for (;;) {
    float tempC = GetTemp();
    int pot = GetPotentiometer();
    MPU_DATA_t mpuData = MPU_Read();

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
             mpuData.x_gyro, mpuData.y_gyro, mpuData.z_gyro);
    GUI_DispStringAt(gyroStr, 10, 90);

    char accelStr[32];
    snprintf(accelStr, sizeof(accelStr),
             "Accel: (%.2f, %.2f, %.2f)      ",
             mpuData.x_accel, mpuData.y_accel, mpuData.z_accel);
    GUI_DispStringAt(accelStr, 10, 110);

    // Update the button press states
    ControlsUpdate();

    CyDelay(100);
  }
}

/* [] END OF FILE */
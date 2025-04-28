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
  GUI_Clear();
  GUI_SetFont(&GUI_Font8x16);

  // Start ADC conversions
  PotADC_Start();
  PotADC_StartConvert();

  // Main loop
  for (;;) {
    float tempC = GetTemp();
    int pot = GetPotentiometer();

    if (IsBtnPressedOnce()) {
      SerialPrintln("Hello World!");
      SerialPrint("Temperature: ");
      SerialPrintlnf(tempC);
      SerialPrint("Potentiometer: ");
      SerialPrintlnf(pot);

      char tempStr[32];
      snprintf(tempStr, sizeof(tempStr), "Temperature: %.2f", tempC);
      GUI_DispStringAt(tempStr, 50, 50);

      char potStr[32];
      snprintf(potStr, sizeof(potStr), "Potentiometer: %d", pot);
      GUI_DispStringAt(potStr, 50, 70);
    }

    // Update the button press states
    ControlsUpdate();
  }
}

/* [] END OF FILE */
#include <stdio.h>

#include "config.h"
#include "device.h"
#include "serial.h"

int main() {

  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS operation with 5V operation
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  // Start the I2C Master
  I2C_Start();

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
    }

    // Update the button press states
    ControlsUpdate();
  }
}

/* [] END OF FILE */
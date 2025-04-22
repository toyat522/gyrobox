#include <stdio.h>

#include "config.h"
#include "project.h"
#include "serial.h"
#include "sht40.h"

int main() {

  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS operation with 5V operation
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  // Start the I2C Master
  I2C_Start();

  // Main loop
  for (;;) {
    SerialPrintln("Hello World!");

    float tempC = GetTemp();
    SerialPrintlnf(tempC);

    CyDelay(1000);
  }
}

/* [] END OF FILE */
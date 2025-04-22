#include "config.h"
#include "project.h"
#include "serial.h"

int main() {

  // Enable global interrupts
  CyGlobalIntEnable;

  // Start USBFS operation with 5V operation
  USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

  for (;;) {
    SerialPrintln("Hello World!");

    CyDelay(1000);
  }
}

/* [] END OF FILE */
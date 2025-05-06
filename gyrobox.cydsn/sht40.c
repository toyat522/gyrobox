#include "sht40.h"

#include "config.h"
#include "project.h"

/*
This function retrieves the current temperature in degrees
Celcius from the SHT40.
*/
float GetTemp() {
  // Prevent interrupt call during I2C communication
  uint8_t state = CyEnterCriticalSection();

  // Select the I2C bus for SHT40
  I2C_BusSelect_Write(1);

  // Write high precision T&RH command to SHT40
  I2C_MasterSendStart(SHT40_ADDR, 0);
  I2C_MasterWriteByte(0xFD);
  I2C_MasterSendStop();

  // Read first 2 bytes for temperature data
  CyDelay(10);
  I2C_MasterSendStart(SHT40_ADDR, 1);
  uint8_t topByte = I2C_MasterReadByte(I2C_ACK_DATA);
  uint8_t bottomByte = I2C_MasterReadByte(I2C_NAK_DATA);
  I2C_MasterSendStop();

  // Exit critical section
  CyExitCriticalSection(state);

  // Perform the calculations to retrieve temperature
  uint16_t tempTicks = ((uint16_t)topByte << 8) | (uint16_t)bottomByte;
  return -45 + 175 * tempTicks / 65535.0;
}

/*
This function converts degrees F to C.
*/
float FahrenheitToCelsius(float f) {
  return (f - 32) * 5 / 9;
}

/*
This function converts degrees C to F.
*/
float CelsiusToFahrenheit(float c) {
  return 1.8 * c + 32;
}

/* [] END OF FILE */
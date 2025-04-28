#include "serial.h"

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "project.h"

/*
This function waits until USB UART is ready, then sends the string
to the host without appending any newline characters.
*/
void SerialPrint(const char *str) {
  // Don't do anything if serial is not configured
  if (USBUART_GetConfiguration() == 0) return;

  // Calculate the length of the string
  uint16_t len = 0;
  while (str[len] != '\0') {
    len++;
  }

  // Wait until USB component is ready to send data
  while (USBUART_CDCIsReady() == 0u) {
  }

  // Send the string to the host
  USBUART_PutData((uint8 *)str, len);
}

/*
This function sends the provided float to the host.
*/
void SerialPrintf(const float val) {
  // Don't do anything if serial is not configured
  if (USBUART_GetConfiguration() == 0) return;

  // Calculate length required to convert float to string
  int len = snprintf(NULL, 0, "%f", val);

  // Allocate the required length and perform conversion
  char *str = malloc(len + 1);
  snprintf(str, len + 1, "%f", val);

  // Print the float converted to a string
  SerialPrint(str);

  // Free the allocated memory
  free(str);
}

/*
This function sends the provided string to the host, followed by
a carriage return and line feed ("\r\n").
*/
void SerialPrintln(const char *str) {
  // Don't do anything if serial is not configured
  if (USBUART_GetConfiguration() == 0) return;

  // Print the string followed by a newline
  SerialPrint(str);

  // Wait until USB component is ready to send data
  while (USBUART_CDCIsReady() == 0u) {
  }

  // Send carriage return line feed (CRLF)
  uint8 crlf[2] = {'\r', '\n'};
  USBUART_PutData(crlf, 2);
}

/*
This function sends the provided float to the host, followed by
a carriage return and line feed ("\r\n").
*/
void SerialPrintlnf(const float val) {
  // Don't do anything if serial is not configured
  if (USBUART_GetConfiguration() == 0) return;

  // Print the string followed by a newline
  SerialPrintf(val);

  // Wait until USB component is ready to send data
  while (USBUART_CDCIsReady() == 0u) {
  }

  // Send carriage return line feed (CRLF)
  uint8 crlf[2] = {'\r', '\n'};
  USBUART_PutData(crlf, 2);
}

/* [] END OF FILE */
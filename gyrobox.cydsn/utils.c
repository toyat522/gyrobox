#include "utils.h"

/*
This function waits until the USB UART is ready, then sends the string
to the host without appending any newline characters.
*/
void SerialPrint(const char *str) {
  // Calculate the length of the string
  uint16 len = 0;
  while (str[len] != '\0') {
    len++;
  }

  // Wait until USB component is ready to send data
  while (0u == USBUART_CDCIsReady()) {
  }

  // Send the string to the host
  USBUART_PutData((uint8 *)str, len);
}

/*
This function sends the provided string to the host, followed by
a carriage return and line feed ("\r\n").
*/
void SerialPrintln(const char *str) {
  // Print the string followed by a newline
  SerialPrint(str);

  // Wait until USB component is ready to send data
  while (0u == USBUART_CDCIsReady()) {
  }

  // Send carriage return line feed (CRLF)
  uint8 crlf[2] = {'\r', '\n'};
  USBUART_PutData(crlf, 2);
}
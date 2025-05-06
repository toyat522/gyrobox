#include "tft.h"

#include "GUI.h"
#include "project.h"

#define TFT_WIDTH 320
#define TFT_HEIGHT 320

/*
This function writes an 8-bit value to the TFT with the
D/C line low.
*/
void write8_a0(uint8 data) {
  // Lower the chip select for TFT
  CS_Select_Write(0);

  // Set DC line low and transmit data
  TFT_DC_Write(0x00);
  SPIM_WriteTxData(data);

  // Wait for data to be sent
  while (!(SPIM_ReadTxStatus() & 0x01)) {
  };
}

/*
This function writes an 8-bit value to the TFT with the
D/C line high.
*/
void write8_a1(uint8 data) {
  // Lower the chip select for TFT
  CS_Select_Write(0);

  // Set DC line high and transmit data
  TFT_DC_Write(0x01);
  SPIM_WriteTxData(data);

  // Wait for data to be sent
  while (!(SPIM_ReadTxStatus() & 0x01)) {
  };
}

/*
This function writes multiple bytes to the TFT with the
D/C line high.
*/
void writeM8_a1(uint8 *pData, int N) {
  // Lower the chip select for TFT
  CS_Select_Write(0);

  // Set DC line high
  TFT_DC_Write(0x01);
  int i;
  for (i = 0; i < N; i++) {
    // Send data to transmit buffer
    SPIM_WriteTxData(pData[i]);

    // Wait for data to be sent
    while (!(SPIM_ReadTxStatus() & 0x01)) {
    };
  }
}

/*
Unimplemented read function.
*/
uint8 read8_a1(void) {
  for (;;) {
  }
};

/*
Unimplemented read function.
*/
void readM8_a1(uint8 *pData, int N) {
  for (;;) {
  }
}

/*
This function must be called to initialize the TFT.
*/
void tftStart(void) {
  CS_Select_Write(0); // Lower the chip select for TFT
  write8_a0(0x01);    // Send software reset command
  CyDelay(10);        // Must wait >5ms after command
  write8_a0(0x36);    // Send memory access control command
  write8_a1(0x88);
  write8_a0(0x3A);    // Send COLMOD: pixel format set command
  write8_a1(0x55);
  write8_a0(0x11);    // Send sleep out command
  write8_a0(0x29);    // Send display on command
  CyDelay(250);       // Delay to allow all changes to take effect
}

/*
This function sets the orientation of the TFT display.
*/
void TFT_SetOrientation(int mode) {
  CS_Select_Write(0); // Lower the chip select for TFT
  write8_a0(0x36);    // Send memory access control command
  switch (mode) {
    case 0: write8_a1(0x48); break; // Portrait
    case 1: write8_a1(0x28); break; // Landscape
    case 2: write8_a1(0x88); break; // Inverted Portrait
    case 3: write8_a1(0xE8); break; // Inverted Landscape
  }

  // Reconfigure emWin to update size of TFT display
  if (mode % 2)
    LCD_SetSizeEx(0, TFT_WIDTH, TFT_HEIGHT);
  else
    LCD_SetSizeEx(0, TFT_HEIGHT, TFT_WIDTH);
}

/*
This function matches the TFT orientation to the device orientation.
*/
void TFT_MatchDeviceOrientation(Orientation_t orientation) {
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
}

/* [] END OF FILE */
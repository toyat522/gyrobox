#include "mpu.h"

#include "project.h"

/*
This function must be called before using the MPU to initialize.
*/
void MPU_Init() {
  MPU_ByteWrite(0x1A, 0x06); // Disable frame sync, set DLPF to 260 Hz
  MPU_ByteWrite(0x6B, 0x00); // Takes the MPU out of sleep mode
  MPU_ByteWrite(0x1B, 0x00); // Sets the gyros to 250 degrees/second range
  MPU_ByteWrite(0x1C, 0x00); // Sets the accelerometer to +/- 2 deg range
  MPU_ByteWrite(0x23, 0x00); // Disables FIFO buffer
}

/*
This function reads gyro and accelerometer data from the MPU.
*/
MPU_DATA_t MPU_Read() {
  // Select the I2C bus for MPU
  I2C_BusSelect_Write(0);

  // Send the MPU the gyro register address to read from it
  I2C_MasterSendStart(0x68, 0);
  I2C_MasterWriteByte(0x43); // Gyro register starts from 0x43

  // Receive data from the MPU
  I2C_MasterSendRestart(0x68, 1);

  // Read bytes 0-4 and acknowledge
  uint8_t rawdata[6] = {0};
  rawdata[0] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[1] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[2] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[3] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[4] = I2C_MasterReadByte(I2C_ACK_DATA);

  // Read byte 5 and send NAK to signal end of reading data
  rawdata[5] = I2C_MasterReadByte(I2C_NAK_DATA);

  // Send stop byte to finish the transaction.
  I2C_MasterSendStop();

  // Concatenate the upper and lower byte of each value
  uint16_t hold = ((uint16_t)rawdata[0] << 8) | (uint16_t)rawdata[1];

  // Interpret each value and put it into the mpu_data structure
  MPU_DATA_t mpu_data;
  mpu_data.x_gyro = (int16_t)hold / 131.0;

  hold = ((uint16_t)rawdata[2] << 8) | (uint16_t)rawdata[3];
  mpu_data.y_gyro = (int16_t)hold / 131.0;

  hold = ((uint16_t)rawdata[4] << 8) | (uint16_t)rawdata[5];
  mpu_data.z_gyro = (int16_t)hold / 131.0;

  // Send the MPU the accelerometer register address to read from it
  I2C_MasterSendStart(0x68, 0);
  I2C_MasterWriteByte(0x3B); // Accelerometer register starts from 0x3B

  // Receive data from the MPU
  I2C_MasterSendRestart(0x68, 1);

  // Read bytes 0-4 and acknowledge
  rawdata[0] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[1] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[2] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[3] = I2C_MasterReadByte(I2C_ACK_DATA);
  rawdata[4] = I2C_MasterReadByte(I2C_ACK_DATA);

  // Read byte 5 and send NAK to signal end of reading data
  rawdata[5] = I2C_MasterReadByte(I2C_NAK_DATA);

  // Send stop byte to finish the transaction.
  I2C_MasterSendStop();

  // Interpret each value and put it into the mpu_data structure
  hold = ((uint16_t)rawdata[0] << 8) | (uint16_t)rawdata[1];
  mpu_data.x_accel = (int16_t)hold / 16384.0;

  hold = ((uint16_t)rawdata[2] << 8) | (uint16_t)rawdata[3];
  mpu_data.y_accel = (int16_t)hold / 16384.0;

  hold = ((uint16_t)rawdata[4] << 8) | (uint16_t)rawdata[5];
  mpu_data.z_accel = (int16_t)hold / 16384.0;

  return mpu_data;
}

/*
This function sends a byte to the specified address in the MPU.
*/
void MPU_ByteWrite(uint8_t reg, uint8_t byte) {
  // Select the I2C bus for MPU
  I2C_BusSelect_Write(0);

  // Start a write to the MPU to begin the transaction
  I2C_MasterSendStart(0x68, 0);

  // Send the address of the register we want to write to
  I2C_MasterWriteByte(reg);

  // Send the given byte
  I2C_MasterWriteByte(byte);

  // Send the stop signal to end the transaction
  I2C_MasterSendStop();
}
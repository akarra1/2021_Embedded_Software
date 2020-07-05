#ifndef _IMU_
#define _IMU_

//headers:
#include "Wire.h"
#include "Energia.h"

//Functions:
void I2C_init();
void getI2CData();
int16_t convert_16bit(int8_t high, uint8_t low);
float accel_conversion(int16_t rawaccel, int axis);
float gyro_conversion(int16_t rawgyro);
void printData(float accelx, float accely, float accelz, float gyrox, float gyroy, float gryoz);

//pin declerations
int sda = P2_2; //I2C pins
int scl = P2_1;

//registers
#define lsm9ds1_ag 0x6B //device reg

#define gyro_control1 0x10 //Control of the gyro reg
#define accel_control4 0x1E //Control of the accel reg4
#define accel_control6 0x20 //Control of the accel reg6
#define accel_control5 0x1F //Control of the accel reg5
#define accel_control7 0x21 //Control of the accel reg7
#define accel_control8 0x22 //Control of the accel reg8, which covers some interrupts and shutdowns
#define mag_pwr 0x16 //the register that powers the mangetometer
#define strtw 0xD6 //send before write?
#define strtr 0xD5
#define fifo_ctrl 0x2E
#define fifo_src 0x2F

#define gyroX0 0x18 //accel output registerss
#define gyroX1 0x19
#define gyroY0 0x1A
#define gyroY1 0x1B
#define gyroZ0 0x1C
#define gyroZ1 0x1D

#define accelX0 0x28 //accel output registerss
#define accelX1 0x29
#define accelY0 0x2A
#define accelY1 0x2B
#define accelZ0 0x2C
#define accelZ1 0x2D

#define status_reg 0x27

#endif

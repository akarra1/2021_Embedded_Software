#ifndef _IMU_
#define _IMU_
/*
   Written and maintained by: 
   Andrew Kettle
   August 1st, 2020
*/
#include <stdint.h>

//registers
#define lsm9ds1_ag 0x6B //device reg, 7 bit address
#define gyro_control1 0x10 //Control of the gyro reg
#define accel_control4 0x1E //Control of the accel reg4
#define accel_control6 0x20 //Control of the accel reg6
#define accel_control5 0x1F //Control of the accel reg5
#define accel_control7 0x21 //Control of the accel reg7
#define accel_control8 0x22 //Control of the accel reg8, which covers some interrupts and shutdowns
#define accel_control9 0x23 
#define mag_pwr 0x16 //the register that powers the mangetometer
#define strtw 0xD6 //send before write?
#define strtr 0xD5
#define fifo_ctrl 0x2E
#define fifo_src 0x2F

#define gyroX0 0x18 //gyro output registerss
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

class IMU {

   public:
      //constuctor
      IMU() //default constructor
      {
         accel_x_axis = 0.0;
         accel_y_axis = 0.0;
         accel_z_axis = 0.0;
         gyro_x_axis = 0.0;
         gyro_y_axis = 0.0;
         gyro_z_axis = 0.0;
      }
      //Functions:
      void IMU_init();
      void getAccelData();
      void getGyroData(); 
      int16_t convert_16bit(int8_t high, uint8_t low);
      float accel_conversion(int16_t rawaccel);
      float gyro_conversion(int16_t rawgyro);
      void printAccelData();
      void printGyroData();

      //getters for specific values
      float getAccelX() { return accel_x_axis; }
      float getAccelY() { return accel_y_axis; }
      float getAccelZ() { return accel_z_axis; }
      float getGyroX()  { return gyro_x_axis; }
      float getGyroY()  { return gyro_y_axis; }
      float getGyroZ()  { return gyro_z_axis; }

   private:
      float accel_x_axis;
      float accel_y_axis;
      float accel_z_axis;
      float gyro_x_axis;
      float gyro_y_axis;
      float gyro_z_axis;
};

#endif
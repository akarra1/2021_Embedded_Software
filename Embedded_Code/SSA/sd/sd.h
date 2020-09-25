#ifndef _SD_
#define _SD_
/*
   Written and maintained by: 
   Andrew Kettle
   September 20th, 2020
*/
#include <stdint.h>
#include <SdFat.h>
#include "imu.h"

class SD {
   public:
      SD()
      {
         isSdOpen = false; //attempting to hardcode append mode so that I can isoltate the problem
         filenum = '0'; //default value (should be overwritten)
      } //defualt constructor

      //Functions
      bool initSD();
      void SdRemove();
      void SdClose();
      int getFname();
      bool SdWrite(IMU imu, float t1, float t2, float t3, float ws);
      bool getSDState();
      void setSDState(bool st);
      void setFileNum(int f) { filenum = char(f); }
      char getFileNum() { return filenum; }

   private:
      bool isSdOpen;
      //SD card object
      SdFatSdio SDCARD; 
      char filenum;
};
#endif
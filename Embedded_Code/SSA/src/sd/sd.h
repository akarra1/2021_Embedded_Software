#ifndef _SD_
#define _SD_
/*
   Written and maintained by: 
   Andrew Kettle
   September 27th, 2020
*/
#include <stdint.h>
#include <SdFat.h>
#include "imu.h"

class SD {

   public:
      SD()
      {
         isSdOpen = false; 
         filenum = '0'; //default value (should be overwritten)
      } //defualt constructor

      //Functions
      bool initSD();
      void SdRemove();
      int getFname();
      bool SdWrite(IMU imu, float t1, float t2, float t3, float ws);
      char getFileNum() { return filenum; }
      void setFileNum(int f) { filenum = char(f); }
      bool getSDState() { return isSdOpen; }
      void setSDState(bool st) { isSdOpen = st; }
      void createFileName();

   private:
      SdFatSdio SDCARD; //SD card object
      File file; //file object
      bool isSdOpen; //state boolean
      char filenum;
      char file_name[13]; //small buffer size
};
#endif
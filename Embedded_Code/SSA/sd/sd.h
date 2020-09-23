#ifndef _SD_
#define _SD_
/*
   Written and maintained by: 
   Andrew Kettle
   September 20th, 2020
*/
#include <stdint.h>
#include "imu.h"

class SD {
   public:
      SD()
      {
         isSdOpen = false;
      } //defualt constructor

      //Functions
      bool initSD();
      void SdRemove();
      void SdClose();
      int getFname();
      bool SdWrite(IMU imu, float t1, float t2, float t3, float ws);
      bool getSDState();
      bool setSDState(bool st);

   private:
      bool isSdOpen;
};
#endif
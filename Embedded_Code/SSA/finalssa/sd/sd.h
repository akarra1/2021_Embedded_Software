#ifndef _SD_
#define _SD_
/*
   Written and maintained by: 
   Andrew Kettle
   September 20th, 2020
*/
#include <stdint.h>
#include "imu.h"

bool initSD();
void SdClose();
void SdWrite(IMU imu);

#endif
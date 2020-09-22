/*
   Written and maintained by: 
   Andrew Kettle
   September 22nd, 2020
*/

#include <SdFat.h>
#include <Arduino.h>
#include "sd.h"

//SD card object
SdFatSdio SD; 
File file;

bool initSD()
{
   if(!SD.begin()) {
      return false;     
   }
   return true;
}

void SdWrite(IMU imu)
{
   //check and remove existing file
   file = SD.open("data.csv", FILE_WRITE);
   file.print("Hello John\n");
   SdClose();
   //write data formatted to csv
}

void SdClose()
{
   //close file
   file.close();
}
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

void SdRemove()
{
   SD.remove("data.csv");
}

bool SdWrite(IMU imu, float t1, float t2, float t3, float ws)
{
   //check and remove existing file
   file = SD.open("data.csv", FILE_WRITE);
   if(!file) { return false; }
   else {
      char fstr[50]; //conservative buffer size
      sprintf(fstr, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", imu.getAccelX(), imu.getAccelY(), imu.getAccelZ(),\
      imu.getGyroX(), imu.getGyroY(), imu.getGyroZ(), t1, t2, t3, ws); //formatting string
      file.seek(EOF); //gets to end of file so append occurs
      file.println(fstr); //printing to string
   }
   SdClose(); //opening and closing file each time to minimize risk of corruption due to unfortunate shutdown
   //TODO: Receive CAN signal from CCM before shutdown so file can be closed / opened only once
   return true;
}

void SdClose()
{
   //close file
   file.close();
}
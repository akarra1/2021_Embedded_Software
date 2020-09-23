/*
   Written and maintained by: 
   Andrew Kettle
   September 22nd, 2020
*/

#include <SdFat.h>
#include <Arduino.h>
#include "sd.h"

//SD card object
SdFatSdio SDCARD; 
File file;

bool SD::initSD()
{
   if(!SDCARD.begin()) {
      return false;     
   }
   return true;
}

int SD::getFname()
{
   //get number from file
   File tempf = SDCARD.open("fname.txt", FILE_READ);
   int n = tempf.read();
   tempf.close();

   //increment for the next log
   tempf = SDCARD.open("fname.txt", FILE_WRITE);
   file.seek(0); //seek to the beginning of the file since its only a character
   if(n+1 > 9) { //only keeping 10 data logs, 0-9 so that character logic doesn't need to change into string as well
      n = -1;
   }
   char to_write = char(n+1);
   file.print(to_write);
   tempf.close();
   return n; //if EOF, -1 is returned so pass it on regardless
}

void SdRemove()
{
   SDCARD.remove("data.csv");
}

bool SD::SdWrite(IMU imu, float t1, float t2, float t3, float ws)
{
   //check for new file name 
   int fnum = getFname();
   if(fnum==-1) {
      fnum = 48; //set to append to default log if there is an error (ascii 0 = 48)
   }

   char num = char(fnum); //cast int to char
   char file_name[13]; 
   snprintf(file_name, sizeof(file_name), "data_%c.csv", num);

   file = SDCARD.open(file_name, FILE_WRITE);
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

void SD::SdClose()
{
   //close file
   file.close();
}

bool SD::getSDState()
{
   return isSdOpen;
}

bool SD::setSDState(bool st)
{
   isSdOpen = st;
}
/*
   Written and maintained by: 
   Andrew Kettle
   September 22nd, 2020
*/

#include <Arduino.h>
#include "sd.h"

bool SD::initSD()
{
   if(!SDCARD.begin()) {
      return false;     
   }
   //get the correct file character
   int fnum = getFname();
   if(fnum == -1) {
      fnum = 48; //set to append to default log if there is an error (ascii 0 = 48)
   }
   setFileNum(fnum);
   createFileName();
   return true;
}

int SD::getFname()
{
   //get number from file
   File tempf = SDCARD.open("fname.txt", FILE_READ);
   tempf.seek(0); //seek to the beginning of the file since its only a character
   int n = tempf.read(); //reads byte = 8 bits, char is 255 (8 bits) tf: next byte is next char returned in integer ascii
   tempf.close();

   //increment for the next log, allows for 10 different data logs to be stored at a given time
   tempf = SDCARD.open("fname.txt", FILE_WRITE);
   tempf.seek(0); //seek to the beginning of the file since its only a character
   //57 is ascii 9
   if(n+1 > 57) { //only keeping 10 data logs, 0-9 so that character logic doesn't need to change into string as well
      n = 48; //0 starting gets incremented into a 1
   }
   int u = n + 1; //incrememnt so the prior log isn't overwritten
   char to_write = char(u);
   tempf.print(to_write);
   tempf.close();

   return u; //if EOF, -1 is returned so pass it on regardless
}

void SD::createFileName() { //create file name
   snprintf(file_name, sizeof(file_name), "data_%c.csv", filenum);
}

//bool SD::SdWrite(IMU imu, float t1, float t2, float t3, float ws)
bool SD::SdWrite(IMU imu, float t1, float t2, float t3, float ws)
{
   //open or append to file depending on state of sd card 
   if(isSdOpen) {
      file = SDCARD.open(file_name, O_APPEND | O_RDWR); //opening to read and write in append mode
   }
   else {
      file = SDCARD.open(file_name, FILE_WRITE); //create file and write the first line to it
   }
   //write updated data to the file
   if(!file) { return false; }
   else {
      char fstr[120]; //conservative buffer size
      sprintf(fstr, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f", imu.getAccelX(), imu.getAccelY(), imu.getAccelZ(),\
      imu.getGyroX(), imu.getGyroY(), imu.getGyroZ(), t1, t2, t3, ws); //formatting string
      file.seek(EOF); //gets to end of file so append occurs
      file.println(fstr); //printing to string
   }
   file.close(); //opening and closing file each time to minimize risk of corruption due to unfortunate shutdown
   //TODO: Receive CAN signal from CCM before shutdown so file can be closed / opened only once
   return true;
}

void SD::SdRemove() {
   SDCARD.remove("data.csv");
}
/*
   Written and maintained by: 
   Andrew Kettle
   September 22nd, 2020
*/

#include <Arduino.h>
#include "sd.h"
#include <fstream>

const char* SD::file_count_name = "file_count.bin";


SD::SD() {     // default constructor
   file_has_data = false; 
}

bool SD::initSD()
{
   if(!SDCARD.begin()) {
      return false;     
   }
   createNewDataFile();
   return true;
}

int SD::getNumFiles() {
   //get number from file
   File tempf = SD::SDCARD.open(SD::file_count_name, FILE_READ);
   tempf.seek(0); //seek to the beginning of the file since its only a character
   
   int n;                        //reads byte = 8 bits, char is 255 (8 bits) tf: next byte is next char returned in integer ascii
   if(!tempf.available()) {      //if the file was empty or nonexistent
      n = 0;
   } else {
      n = tempf.peek();          // get the first byte without advancing
   }
   tempf.close();
   return n;
}

void SD::createNewDataFile() {
   int numFiles = getNumFiles();
   int thisFileNum = numFiles + 1;

   // set this.file_name based on the number of existing files
   snprintf(file_name, sizeof(file_name), "data_%d.csv", thisFileNum);

   // update the isSdOpen to false (since the file is empty)
   file_has_data = false;

   // update file_count file to reflect the new file count
   File tempf = SDCARD.open(SD::file_count_name, FILE_WRITE);
   tempf.seek(0); //seek to the beginning of the file since its only a character
   tempf.write(thisFileNum);
   tempf.close();
}


template<typename T>
bool SD::writeElements(const T* elements, int num_elements) {
   //open or append to file depending on state of sd card
   File file;
   if(file_has_data) {
      file = SD::SDCARD.open(file_name, O_APPEND | O_RDWR); //opening to read and write in append mode
   } else {
      file = SD::SDCARD.open(file_name, FILE_WRITE);        //create file and write the first line to it
   }

   //write updated data to the file
   if(!file) {return false;}
   else {
      // go to the end of the file (to append)
      file.seek(EOF);

      // print the elements to the file as comma-separated values
      for(int i=0; i<num_elements; ++i) {
         file.print(elements[i]);
         if(i < num_elements-1) {
            file.print(", ");
         } else {
            file.print("\n");
         }
      }
   }

   file_has_data = true;     // this flags that the file has data in it

   //opening and closing file each time to minimize risk of corruption due to unfortunate shutdown 
   file.close();
   //TODO: Receive CAN signal from CCM before shutdown so file can be closed / opened only once
   return true;
}

bool SD::SdWrite(IMU imu, float t1, float t2, float t3, float ws)
{
   float args[] = {
      imu.getAccelX(), imu.getAccelY(), imu.getAccelZ(),
      imu.getGyroX(), imu.getGyroY(), imu.getGyroZ(),
      t1, t2, t3, ws
   };
   return writeElements<float>(args, 10);
}

bool SD::SdWriteHeader() {
   String args[] = {
      "Accel-X", "Accel-Y", "Accel-Z",
      "Gyro-X", "Gyro-Y", "Gyro-Z",
      "Temp-1", "Temp-2", "Temp-3", "Wheelspeed"
   };
   return writeElements<String>(args, 10);
}

void SD::SdRemove() {
   SDCARD.remove("data.csv");
}
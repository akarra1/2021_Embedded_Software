/*
   Written and maintained by: 
   Andrew Kettle
   September 22nd, 2020
*/
/* main function for calling all of the various SSA sensors */

#include "imu.h"
#include "sd.h"
#include "analog.h"
#include "wheelspeed.h"
#include "gps.h"
#include <Arduino.h>

void printAllData();

//global containers
IMU lsm9ds1;
SD sdcard;
IRsensors temp;
GPS gpsModule;
float wheelspeed = 0; 

void waitForSerialSetup() {
  // this pauses the code until a serial connection has been setup
  // this is useful if trying to monitor serial prints in the setup phase
  // as the code normally blows through the setup function before a serial
  // monitor can be setup
  while(!Serial);
  Serial.println("serial monitor successfully setup");
}

void scanForI2CDevices() {
    // this piece of code is used for debugging. It checks all possible I2C
    // addresses, and prints it if a response is found when pinged
    // this code is taken from 
    // https://electronics.stackexchange.com/questions/76617/determining-i2c-address-without-datasheet/76622
    // credit for this function goes to that user

    for (int address=1; address <= 126; address++) {
        Wire.beginTransmission(address); // Select address
        if (!Wire.endTransmission()) Serial.printf("Found device at: %02Xh\n", address); 
    }
}

void setup() //initializes different sensors
{
    //beginning serial, default is 12 mbit/s for teensys
	Serial.begin(9600); 

    //setting up sensors
	temp.analogSetup();
    lsm9ds1.IMU_init(); 
    gpsModule.initGPS();
    if(!sdcard.initSD()) { 
        Serial.print("SD initialization failed");  
    } else {
        sdcard.SdWriteHeader();
    }  
}

void loop() //Eventually going to want to multithread this so the other threads can make progress while wheel speed delays
{	
  //get analog data
  temp.analogData(); //reads data

  //get IMU data
  lsm9ds1.getAccelData(); //Read accel data
  lsm9ds1.getGyroData(); //Read gyro data

  //get wheelspeed data (currently untested)
  /*
  wheelspeedSetup();
  while(getwheelspeedData() == 0) { continue; } //waiting for magnet to trigger, magnet has to trigger in order for execution to finish
  wheelspeed = getwheelspeedData();
  */
  
  // get GPS data
  gpsModule.updateGPS();

  if(!sdcard.SdWrite(
      lsm9ds1, temp.getTemps(1), temp.getTemps(2), temp.getTemps(3), 
      0.0, gpsModule.getLatitude(), gpsModule.getLongitude())) { 
    Serial.print("Couldn't open file for writing ");  
  }
  delay(100); //temporary delay for serial line during devlopment
}

void printAllData()
{
  //Temperature data
  Serial.print("Temp1: ");
  Serial.println(temp.getTemps(1));
  Serial.print("Temp2: ");
  Serial.println(temp.getTemps(2));
  Serial.print("Temp3: ");
  Serial.println(temp.getTemps(3));
  Serial.println("\n");

  //Accelerometer data
  Serial.print("X axis accel: ");
  Serial.println(lsm9ds1.getAccelX());
  Serial.print("Y axis accel: ");
  Serial.println(lsm9ds1.getAccelY());
  Serial.print("Z axis accel: ");
  Serial.println(lsm9ds1.getAccelZ());
  Serial.println("\n");

  //Gyrometer data
  Serial.print("X axis gyro: ");
  Serial.println(lsm9ds1.getGyroX());
  Serial.print("Y axis gyro: ");
  Serial.println(lsm9ds1.getGyroY());
  Serial.print("Z axis gyro: ");
  Serial.println(lsm9ds1.getGyroZ());
  Serial.println("\n");
  
  //Wheelspeed data
  Serial.print("WheelSpeed: ");
  Serial.println(wheelspeed);
  Serial.println("\n");

  // GPS data
  Serial.print("lat: ");
  Serial.println(gpsModule.getLatitude());
  Serial.print("long: ");
  Serial.println(gpsModule.getLongitude());
  Serial.println("\n");
}
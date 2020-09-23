#include "imu.h"
#include "sd.h"
#include "analog.h"
#include "wheelspeed.h"
#include <Arduino.h>

void printAllData();
/* main function for calling all of the various SSA functions */

//global containers
IMU lsm9ds1;
SD sdcard;
float temparr[3] = {0}; 
float wheelspeed = 0; 

void setup() //initializes different sensors
{
	Serial.begin(9600);
	analogSetup();
  lsm9ds1.IMU_init(); //need to intialize IMU connection each time
  if(!sdcard.initSD()) { 
    Serial.print("SD initialization failed");  
  } else {
    sdcard.setSDState(true);
  }
}

void loop() //Eventually going to want to multithread this so the other threads can make progress while wheel speed delays
{	
  analogData(&temparr[0]);
  lsm9ds1.getAccelData(); //Read accel data
  lsm9ds1.getGyroData(); //Read gyro data
	wheelspeedSetup();
  while(getwheelspeedData() == 0) { continue; } //waiting for magnet to trigger, magnet has to trigger in order for execution to finish
  wheelspeed = getwheelspeedData();
//  printAllData();
  
  if(!sdcard.SdWrite(lsm9ds1, temparr[0], temparr[1], temparr[2], 0.0)) { 
    Serial.print("Couldn't open file for writing");  
  }
  delay(100);
}

void printAllData()
{
  //Temperature data
  Serial.print("Temp1: ");
  Serial.println(temparr[0]);
  Serial.print("Temp2: ");
  Serial.println(temparr[1]);
  Serial.print("Temp3: ");
  Serial.println(temparr[2]);
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
  Serial.print("X axis accel: ");
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
  
}
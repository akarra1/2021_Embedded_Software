#include "imu.h"
#include "analog.h"
#include "wheelspeed.h"
#include "Energia.h"

void printAllData();
/* main function for calling all of the various SSA functions */

//include all header files
float temparr[3] = {0}; 
float accelarr[3] = {0}; //x, y, z
float gyroarr[3] = {0}; //x, y, z
float wheelspeed = 0; 

void setup() //initializes different sensors
{
	analogSetup();
  IMU_init(); //need to intialize IMU connection each time
	Serial.begin(9600);
}

void loop() //Eventually going to want to multithread this so the other threads can make progress while wheel speed delays
{	
  analogData(&temparr[0]);
  getAccelData(&accelarr[0]); //Read accel data
  getGyroData(&gyroarr[0]); //Read gyro data
	wheelspeedSetup();
  while(getwheelspeedData() == 0) { continue; } //waiting for magnet to trigger
  wheelspeed = getwheelspeedData();
  printAllData();
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

  Serial.print("X axis accel: ");
  Serial.println(accelarr[0]);
  Serial.print("Y axis accel: ");
  Serial.println(accelarr[1]);
  Serial.print("Z axis accel: ");
  Serial.println(accelarr[2]);
  Serial.println("\n");

  Serial.print("X axis gyro: ");
  Serial.println(gyroarr[0]);
  Serial.print("Y axis gyro: ");
  Serial.println(gyroarr[1]);
  Serial.print("Z axis gyro: ");
  Serial.println(gyroarr[2]);
  Serial.println("\n");

  Serial.print("WheelSpeed: ");
  Serial.println(wheelspeed);
  Serial.println("\n");
}
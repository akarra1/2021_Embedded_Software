/*
   Written and maintained by: 
   Andrew Kettle
   September 19th, 2020
*/

//headers:
#include "imu.h"
#include <Wire.h>
#include <Energia.h>

//pin declerations
int sda = P2_2; //I2C pins
int scl = P2_1;

//Initializes I2C transmission and registers
void IMU::IMU_init()
{ 
  pinMode(scl, 0x2); //Both clock and data line start high for I2C protocol
  pinMode(sda, 0x2);

  //I2C intitial transmission begins 
  Wire.begin(); 
  Wire.beginTransmission(lsm9ds1_ag);
  
  //Initialize registers
  Wire.write(gyro_control1); //Control register for gyro
  Wire.write(193);           //Powers and set to 952 HZ, stock settings elsewhere (sampling faster helped the gyro but not the accel) 
  Wire.write(accel_control4);
  Wire.write(56);            //Makes sure gyro output is turned on
  Wire.write(accel_control5); 
  Wire.write(56);            //Makes sure accel output is turned on, also controls decimation of accel output reg + fifo
  Wire.write(accel_control6); 
  Wire.write(64);            //first three digits select Output data rate, current = 10 Hz
  Wire.write(accel_control7);
  Wire.write(196);           //currently using high resolution mode 
	
  Wire.endTransmission();
  //I2C initial tranmission ends
}

void IMU::getAccelData()
{
  int16_t rawaccelx, rawaccely, rawaccelz;
  float convaccelx, convaccely, convaccelz;
  byte read_arr[6] = {0};

  Wire.beginTransmission(lsm9ds1_ag);
  Wire.write(accelX0 | 0x80);      //getting data from accelerometer registers, 0x80 is burst read starting w/ x axis 
  Wire.endTransmission(); 

  Wire.requestFrom(lsm9ds1_ag, 6); //requesting 6 bytes, 6 8 bit #'s
  for(uint8_t i = 0; i < 6; i++)
  { 
    read_arr[i] = Wire.read();
  }

  Wire.endTransmission();

  rawaccelx = convert_16bit(int8_t(read_arr[1]), uint8_t(read_arr[0])); //returns bytes in descending order
  rawaccely = convert_16bit(int8_t(read_arr[3]), uint8_t(read_arr[2]));
  rawaccelz = convert_16bit(int8_t(read_arr[5]), uint8_t(read_arr[4]));

  convaccelx = accel_conversion(rawaccely); //y and x axes are returning their bytes swapped, not sure what happened in config for that to occur
  convaccely = accel_conversion(rawaccelx);
  convaccelz = accel_conversion(rawaccelz);

  accel_x_axis = convaccelx; //setting private variables
  accel_y_axis = convaccely;
  accel_z_axis = convaccelz;
}

void IMU::getGyroData()
{
  int16_t rawgyrox, rawgyroy, rawgyroz;
  float convgyrox, convgyroy, convgyroz;
  byte read_arr[6] = {0};

  Wire.beginTransmission(lsm9ds1_ag);
  Wire.write(gyroX0 | 0x80);       //getting data from gyro registers, 0x80 is burst read starting w/ x axis 
  Wire.endTransmission(); 

  Wire.requestFrom(lsm9ds1_ag, 6); //requesting 6 bytes, 6 8 bit #'s
  for(uint8_t i = 0; i < 6; i++)
  { 
    read_arr[i] = Wire.read();
  }
  Wire.endTransmission();

  rawgyrox = convert_16bit(int8_t(read_arr[1]), uint8_t(read_arr[0])); //returns bytes in descending order
  rawgyroy = convert_16bit(int8_t(read_arr[3]), uint8_t(read_arr[2]));
  rawgyroz = convert_16bit(int8_t(read_arr[5]), uint8_t(read_arr[4]));

  convgyrox = gyro_conversion(rawgyrox); //y and x axes are returning their bytes swapped, not sure what happened in config for that to occur
  convgyroy = gyro_conversion(rawgyroy);
  convgyroz = gyro_conversion(rawgyroz);

  gyro_x_axis = convgyrox; //populating values of supplied array
  gyro_y_axis = convgyroy;
  gyro_z_axis = convgyroz;
}

//converts two separate 8 bit numbers to a 16 bit number
int16_t IMU::convert_16bit(int8_t high, uint8_t low)
{
	int16_t sixteenbit = ((high << 8) | low); 
	return sixteenbit;
}

//converts the 16 bit int into human understandable data
//0 for x axis, 1 for y axis, 2 for z axis
//different axes are used to discern what the offset factor should be
float IMU::accel_conversion(int16_t rawaccel) {
	//raw unit is millig's/LSB (mg/LSB)
	//default sampling is +-2g, list of conversion factors on page 12 of datasheet

	float conv_factor = .061; //conversion factor for +-2g
	return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
}

float IMU::gyro_conversion(int16_t rawgyro)
{
	//raw unit is millidps/LSB (mdps/LSB)
	//default sampling is +-245dps, list of conversion factors on page 12 of datasheet
	float conv_factor = 8.75; //conversion factor for +-245dps
	return (rawgyro * conv_factor) / 1000; //ouputs in standard dps
}

/*
void IMU::printAccelData()
{
 	  Serial.print("Accel X = ");
  	Serial.println(accel_x_axis, 2); //prints 3 decimal places
  	Serial.print("Accel Y = ");
  	Serial.println(accel_y_axis, 2); //prints 3 decimal places
  	Serial.print("Accel Z = ");
  	Serial.println(accel_z_axis, 2); //prints 3 decimal places
  	Serial.println("\n");
}

void IMU::printGyroData()
{
 	  Serial.print("gyro X = ");
  	Serial.println(gyro_x_axis, 2); //prints 3 decimal places
  	Serial.print("gyro Y = ");
  	Serial.println(gyro_y_axis, 2); //prints 3 decimal places
  	Serial.print("gyro Z = ");
  	Serial.println(gyro_z_axis, 2); //prints 3 decimal places
  	Serial.println("\n");
}
*/
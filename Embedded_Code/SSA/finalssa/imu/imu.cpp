/*
   Written and maintained by: 
   Andrew Kettle
   August 1st, 2020
*/
#include "imu.h"


//Initializes I2C transmission and registers
void IMU_init()
{ 
  //pin declerations
  int sda = P2_2; //I2C pins
  int scl = P2_1;
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
  Wire.write(64);            //first three digits select Output data rate, current = 10 Hz, which is later averaged over 10
  Wire.write(accel_control7);
  Wire.write(196);           //currently using high resolution mode 
	
  Wire.endTransmission();
  //I2C initial tranmission ends
}

void getAccelData(float *accelarr)
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

  *(accelarr + 0) = convaccelx; //populating values of supplied array
  *(accelarr + 1) = convaccely;
  *(accelarr + 2) = convaccelz;
}

void getGyroData(float *gyroarr)
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

  *(gyroarr + 0) = convgyrox; //populating values of supplied array
  *(gyroarr + 1) = convgyroy;
  *(gyroarr + 2) = convgyroz;
}

//converts two separate 8 bit numbers to a 16 bit number
int16_t convert_16bit(int8_t high, uint8_t low)
{
	int16_t sixteenbit = ((high << 8) | low); 
	return sixteenbit;
}

//converts the 16 bit int into human understandable data
//0 for x axis, 1 for y axis, 2 for z axis
//different axes are used to discern what the offset factor should be
float accel_conversion(int16_t rawaccel) {
	//raw unit is millig's/LSB (mg/LSB)
	//default sampling is +-2g, list of conversion factors on page 12 of datasheet

	float conv_factor = .061; //conversion factor for +-2g
	return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
}

float gyro_conversion(int16_t rawgyro)
{
	//raw unit is millidps/LSB (mdps/LSB)
	//default sampling is +-245dps, list of conversion factors on page 12 of datasheet
	float conv_factor = 8.75; //conversion factor for +-245dps
	return (rawgyro * conv_factor) / 1000; //ouputs in standard dps
}

void printAccelData(float accelx, float accely, float accelz)
{
 	  Serial.print("Accel X = ");
  	Serial.println(accelx, 2); //prints 3 decimal places
  	Serial.print("Accel Y = ");
  	Serial.println(accely, 2); //prints 3 decimal places
  	Serial.print("Accel Z = ");
  	Serial.println(accelz, 2); //prints 3 decimal places
  	Serial.println("\n");
}

void printGyroData(float gyrox, float gyroy, float gyroz)
{
 	  Serial.print("gyro X = ");
  	Serial.println(gyrox, 2); //prints 3 decimal places
  	Serial.print("gyro Y = ");
  	Serial.println(gyroy, 2); //prints 3 decimal places
  	Serial.print("gyro Z = ");
  	Serial.println(gyroz, 2); //prints 3 decimal places
  	Serial.println("\n");
}
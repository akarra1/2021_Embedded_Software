/*
   Written and maintained by: 
   Andrew Kettle
   July 4th, 2020
*/
#include "imu.h"

void setup() {
  I2C_init(); //initialize I2C transmission
  Serial.begin(9600);
}

void loop() { //According to millis(), main loop takes about 1 second, no point in sampling fast because of that delay
              //I am slightly surprised by that delay though, so I'm not taking it as fact yet.
  getI2CData(); //Read IMU in a loop
  Serial.flush();
}

void I2C_init()
{ 
  pinMode(scl, 0x2); //Both clock and data line start high in I2C protocol
  pinMode(sda, 0x2);

  //I2C intitial transmission begins 
  Wire.begin(); 
  Wire.beginTransmission(lsm9ds1_ag);
  
  //Initialize registers
  Wire.write(gyro_control1); //Control register for gyro
  Wire.write(193); //Powers and set to 952 HZ, stock settings elsewhere, trial and error with filtering currently
  Wire.write(accel_control4);
  Wire.write(56); //Makes sure gyro output is turned on
  Wire.write(accel_control5); //was 56
  Wire.write(56); //Makes sure accel output is turned on, also controls decimation of accel output reg + fifo
  Wire.write(accel_control6); //was 192, 00100000
  Wire.write(64); //first three digits select Output data rate, current = 10 Hz, which is later averaged over 10
  Wire.write(accel_control7);
  Wire.write(196); //currently using high resolution mode 
  Wire.write(accel_control8);
  Wire.write(2); //Converts to Big endian bit structure 
  Wire.write(fifo_ctrl);
  Wire.write(192); 
	
  Wire.endTransmission();
  //I2C initial tranmission ends
}

void getI2CData() 
{
  int16_t rawaccelx, rawaccely, rawaccelz, rawgyrox, rawgyroy, rawgyroz;
  float convaccelx, convaccely, convaccelz, convgyrox, convgyroy, convgyroz;
  int8_t gX0, gY0, gZ0, aX0, aY0, aZ0; //high byte contains the sign
  uint8_t gX1, gY1, gZ1, aX1, aY1, aZ1;//low byte is unsigned

  Wire.beginTransmission(lsm9ds1_ag);
  
  Wire.write(gyroX0); //getting data from gyro registers
  Wire.write(gyroX1);
  Wire.write(gyroY0);
  Wire.write(gyroY1);
  Wire.write(gyroZ0);
  Wire.write(gyroZ1);
  
  Wire.write(accelX0); //getting data from accelerometer registers
  Wire.write(accelX1);
  Wire.write(accelY0);
  Wire.write(accelY1);
  Wire.write(accelZ0);
  Wire.write(accelZ1);

  Wire.endTransmission(); 
  Wire.requestFrom(lsm9ds1_ag, 12); //requesting 12 bytes, 12 8 bit #'s, or 6 16 bit #'s
  
  if(Wire.available()>=12)          //Waits until all of the axes have available data
  //if((status_reg & 0x01) == 0x01)
  { 
    gX0 = Wire.read();
    gX1 = Wire.read();
    gY0 = Wire.read();
    gY1 = Wire.read();
    gZ0 = Wire.read();
    gZ1 = Wire.read();

    aX0 = Wire.read();
    aX1 = Wire.read();
    aY0 = Wire.read();
    aY1 = Wire.read();
    aZ0 = Wire.read();
    aZ1 = Wire.read();
  }

  Wire.endTransmission();

  rawgyrox = convert_16bit(gX0, gX1);
  rawgyroy = convert_16bit(gY0, gY1);
  rawgyroz = convert_16bit(gZ0, gZ1);
 
  rawaccelx = convert_16bit(aX0, aX1);
  rawaccely = convert_16bit(aY0, aY1);
  rawaccelz = convert_16bit(aZ0, aZ1);
  
  convgyrox = gyro_conversion(rawgyrox);
  convgyroy = gyro_conversion(rawgyroy);
  convgyroz = gyro_conversion(rawgyroz);

  convaccelx = accel_conversion(rawaccelx, 0);
  convaccely = accel_conversion(rawaccely, 1);
  convaccelz = accel_conversion(rawaccelz, 2);
  
  printData(convaccelx, convaccely, convaccelz, convgyrox, convgyroy, convgyroz);	
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

float accel_conversion(int16_t rawaccel, int axis) {
	//raw unit is millig's/LSB (mg/LSB)
	//default sampling is +-2g, list of conversion factors on page 12 of datasheet

	float conv_factor = .061; //conversion factor for +-2g

  switch(axis) //preparing for slight offset factors (how to calculate this over time?)
  {
    case 0: 
	    return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
    case 1: 
	    return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
    case 2: 
	    return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
  }
}

float gyro_conversion(int16_t rawgyro)
{
	//raw unit is millidps/LSB (mdps/LSB)
	//default sampling is +-245dps, list of conversion factors on page 12 of datasheet

	float conv_factor = 8.75; //conversion factor for +-2g
	return (rawgyro * conv_factor) / 1000; //ouputs in standard dps
}

void printData(float accelx, float accely, float accelz, float gyrox, float gyroy, float gyroz)
{
 	  Serial.print("Accel X = ");
  	Serial.println(accelx, 2); //prints 3 decimal places
  	Serial.print("Accel Y = ");
  	Serial.println(accely, 2); //prints 3 decimal places
  	Serial.print("Accel Z = ");
  	Serial.println(accelz, 2); //prints 3 decimal places
  	Serial.println("\n");
}
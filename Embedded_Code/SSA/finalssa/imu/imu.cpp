/*
   Written and maintained by: 
   Andrew Kettle
   June 24th, 2020
*/

#include "imu.h"

void setup() {

  I2C_init(); //initialize I2C transmission
  Serial.begin(9600);
}

void loop() {

  //Reading IMU
  getI2CData();
  Serial.flush();
}

void I2C_init()
{ 
  pinMode(scl, 0x2); //Hex for pull up resistor
  pinMode(sda, 0x2);
  overall_accelx = overall_accely = overall_accelz = overall_gyrox = overall_gyroy = overall_gyroz = 0.0;
  Wire.begin(); //initialize i2c transmission
  Wire.beginTransmission(lsm9ds1_ag);
  //init power modes
  Wire.write(gyro_control1); //Control register for gyro
  Wire.write(193); //Powers and set to 952 HZ, stock settings elsewhere, trial and error with filtering currently
  Wire.write(accel_control4);
  Wire.write(56); //Makes sure gyro output is turned on
  Wire.write(accel_control5); //was 56
  Wire.write(56); //Makes sure accel output is turned on, also controls decimation of accel output reg + fifo
  Wire.write(accel_control6);
  Wire.write(192); // 11000000, first three digits select Output data rate, current = 952 Hz, which is later averaged over 10
  Wire.write(accel_control7);
  Wire.write(196); //currently using high resolution mode 
  Wire.write(fifo_ctrl);
  Wire.write(192);
	
  Wire.endTransmission();
}

void getI2CData() 
{
  int16_t rawaccelx, rawaccely, rawaccelz, rawgyrox, rawgyroy, rawgyroz;
  float convaccelx, convaccely, convaccelz, convgyrox, convgyroy, convgyroz;
  int8_t gX0, gX1, gY0, gY1, gZ0, gZ1, aX0, aX1, aY0, aY1, aZ0, aZ1;

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

  Wire.endTransmission(true); //continue transmission until reading is done

  Wire.requestFrom(lsm9ds1_ag, 12); //requesting 12 bytes, 12 8 bit #'s, or 6 16 bit #'s
  
  if(Wire.available()<=12) 
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

  convaccelx = accel_conversion(rawaccelx);
  convaccely = accel_conversion(rawaccely);
  convaccelz = accel_conversion(rawaccelz);
  
  printData(convaccelx, convaccely, convaccelz, convgyrox, convgyroy, convgyroz);	
}

//converts two separate 8 bit numbers to a 16 bit number
int16_t convert_16bit(int8_t high, int8_t low)
{
	int16_t sixteenbit = (high << 8) | low; 
	return sixteenbit;
}

//converts the 16 bit int into human understandable data
float accel_conversion(int16_t rawaccel)
{
	//raw unit is millig's/LSB (mg/LSB)
	//default sampling is +-2g, list of conversion factors on page 12 of datasheet

	float conv_factor = .061; //conversion factor for +-2g
	return (rawaccel * conv_factor) / 1000; //ouputs in standard g's
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
  //calculates running avg and prints out every 10 times
  if(count >= 5)
  {
    //accel averages
    overall_accelx /= 5.0;
    overall_accely /= 5.0;
    overall_accelz /= 5.0;

    //gyro averages
    overall_gyrox /= 5.0;
    overall_gyroy /= 5.0;
    overall_gyroz /= 5.0;

    //printing for accel
 	  Serial.print("Accel X = ");
  	Serial.println(overall_accelx, 2); //prints 3 decimal places
  	Serial.print("Accel Y = ");
  	Serial.println(overall_accely, 2); //prints 3 decimal places
  	Serial.print("Accel Z = ");
  	Serial.println(overall_accelz, 2); //prints 3 decimal places
  	Serial.println("\n\n");

    //resetting the count back to 0 
    count = 0;
    overall_accelx = 0.0;
    overall_accely = 0.0;
    overall_accelz = 0.0;

    //gyro averages
    overall_gyrox = 0.0;
    overall_gyroy = 0.0;
    overall_gyroz = 0.0;
  }

  else
  {
    //create accel total
    overall_accelx += accelx;
    overall_accely += accely;
    overall_accelz += accelz;

    //create gyro total
    overall_gyrox += gyrox;
    overall_gyroy += gyroy;
    overall_gyroz += gyroz;

    count++;
  }
}

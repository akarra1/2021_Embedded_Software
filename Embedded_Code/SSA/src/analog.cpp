/*Written by Andrew Kettle and Jada Berneguer
  Last modified: August 2nd, 2020
  Driver for acquiring temperature data from ztp-115m temp sensor 
*/
//headers
#include "analog.h"
#include <Arduino.h>

//pin declerations  
int irpin1 = A16;
int irpin2 = A15;
int irpin3 = A14;

void IRsensors::analogSetup() {
  pinMode(irpin1, INPUT);
  pinMode(irpin2, INPUT);
  pinMode(irpin3, INPUT);
}

void IRsensors::analogData() { //Reading IR sensors
  int irread1, irread2, irread3;

  //read raw values
  irread1 = analogRead(irpin1);
  irread2 = analogRead(irpin2);
  irread3 = analogRead(irpin3);

  //caclculating actual temperatures (in F)
  temps[0] = calcTemp(analogConvert(irread1));
  temps[1] = calcTemp(analogConvert(irread2));
  temps[2] = calcTemp(analogConvert(irread3));
}

float IRsensors::analogConvert(int analog_read) //convert analog to Volts
{
  float analog_percentage = analog_read * 3.3; //relative to 3.3v
	return (analog_percentage / 1023);
}

float IRsensors::calcTemp(float volts)
{
  float temperature = 0;
  temperature = volts * (48.24) - 51.8; //Line is derived from taking a linear regression of the datasheets temp vs voltage curve 
  temperature = (temperature * 9/5) + 32; //converting to fahrenheit
  return temperature; 
}

void IRsensors::printAnalogData(float temp1, float temp2, float temp3)
{
  Serial.print("Temp1: ");
  Serial.println(temp1);
  Serial.print("Temp2: ");
  Serial.println(temp2);
  Serial.print("Temp3: ");
  Serial.println(temp3);
  Serial.println("\n");
}

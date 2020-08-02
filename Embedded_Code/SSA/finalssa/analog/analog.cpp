#include "analog.h"

/*Written by Andrew Kettle and Jada Berneguer
  Last modified: August 2nd, 2020
  Driver for acquiring temperature data from ztp-115m temp sensor 
*/
//pin declerations  
int irpin1 = A0;
int irpin2 = A3;
int irpin3 = A4;

void analogSetup() {
  pinMode(irpin1, INPUT);
  pinMode(irpin2, INPUT);
  pinMode(irpin3, INPUT);
}

void analogData(float *analogarray) { //Reading IR sensors
  int irread1, irread2, irread3;
  float temp1, temp2, temp3;

  //sensor1
  irread1 = analogRead(irpin1);
  temp1 = calcTemp(analogConvert(irread1));
  //sensor2
  irread2 = analogRead(irpin2);
  temp2 = calcTemp(analogConvert(irread2));
  //sensor3
  irread3 = analogRead(irpin3);
  temp3 = calcTemp(analogConvert(irread3));

  *(analogarray + 0) = temp1; //populating values of supplied array
  *(analogarray + 1) = temp2;
  *(analogarray + 2) = temp3;
}

float analogConvert(int analog_read) //convert analog to Volts
{
  float analog_percentage = analog_read * 5;
	return (analog_percentage / 1024);
}

float calcTemp(float volts)
{
  float temperature = 0;
  temperature = volts * (48.24) - 51.8; //Line is derived from taking a linear regression of the datasheets temp vs voltage curve (reads a bit higher than true value)
  temperature = (temperature * 9/5) + 32; //converting to fahrenheit
  return temperature; 
}

void printAnalogData(float temp1, float temp2, float temp3)
{
  Serial.print("Temp1: ");
  Serial.println(temp1);
  Serial.print("Temp2: ");
  Serial.println(temp2);
  Serial.print("Temp3: ");
  Serial.println(temp3);
  Serial.println("\n");
}

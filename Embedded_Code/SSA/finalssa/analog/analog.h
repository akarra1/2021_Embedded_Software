#ifndef _ANALOG_
#define _ANALOG_

//headers
#include "Energia.h"

//Functions:
void analogSetup();
void analogData(float *analogarr);
float analogConvert(int analog_read);
float calcTemp(float Volts);
void printAnalogData(float temp1, float temp2, float temp3); 

#endif

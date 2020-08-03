/*Component: Suspension System Array
 * Portion: Wheel Speed
 * This code is used to calculate wheel speed.
 * Written by Leslie Uribe and maintained by Andrew Kettle
 */

#include "wheelspeed.h"

int hall_pin = 13;
float initial_time = 0;
float end_time = 0;
float time_difference = 0;
int w_speed = 0;
int KV = 0;

void wheelspeedSetup() //this function needs to be called everytime wheel speed is called
{
  /* Hall Sensor Setup*/
  pinMode(hall_pin, INPUT);
  initial_time = 0;
  end_time = 0;
  time_difference = 0;
  w_speed = 0;
  KV = 0;
}

/* function calcs wheel speed */
float wheelspeedCalc(float time_difference_micro) 
{
  /* the tire radius is 18 inches */
  float wheel_diameter = 18;
  float circumf = 0;
  float half_rotation = 0;
  float wheel_speed_f = 0;
  float hours = 0;
  float miles = 0;
  /* The circumf is the distance for a full rotation */
  circumf = (3.14159 * wheel_diameter);
  /* half_rotation is distance for a half rotation*/
  half_rotation = circumf/2;

  /*microseconds to hours conversion*/
  float power = pow(10,9);
  power = power * 3.6;
  hours = time_difference_micro / power;
  
  /*half rotations to miles*/
  miles = circumf / 63360;
  
  /*Wheel speed for this half of a rotation*/
  /*convert from rotations per microsecond to miles per hour*/
  wheel_speed_f = miles / hours;
  return wheel_speed_f;
}

/* Start the timer in the loop */
float getwheelspeedData()
{
    float wheel_speed_final = 0;
    if(KV == 0)
    {
        initial_time = micros();
       // delay(10);
        KV = 1;// variable set to one to not take the initial time again
    }
    if(digitalRead(hall_pin) == LOW)
    {
      delay(200); //try to prevent another false read from the sensor
      end_time = micros();
      //delay(10);
      time_difference = end_time - initial_time;
      wheel_speed_final= wheelspeedCalc(time_difference);
      KV = 0;// resetting variable to 0 to take the initial time 
    }
    return wheel_speed_final;
}

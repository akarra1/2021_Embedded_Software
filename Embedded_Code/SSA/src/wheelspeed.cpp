/*Component: Suspension System Array
 * Portion: Wheel Speed
 * This code is used to calculate wheel speed.
 * Written by Leslie Uribe and maintained by Andrew Kettle, and William Sun
 */

#include "wheelspeed.h"


WheelSpeed::WheelSpeed() {
	prev_time = 0;
	last_time = micros();
	last_measured_value = LOW;
	pinMode(WheelSpeed::hall_pin, INPUT);
}

void WheelSpeed::checkWheelspeedSensor() {
	// this function should be called as often as possible to avoid missing
	// the passing by of the halls effect sensor


	/* 
	pseudocode/logic:
	
	if the halls sensor is activated
		if the halls sensor was previously activated, then that means
		that the magnet hasnt moved far away yet. Therefore we do not 
		update the prev_time 
		if the halls sensor was not previously activated, then that means 
		that the magnet has moved into the range of the halls sensor.
		Therefore, we update the prev_time and the last_time
	Update the last_measured_value
	*/

	if(digitalRead(WheelSpeed::hall_pin) == HIGH) {
		uint32_t curr = micros();

		if(last_measured_value == LOW &&
			((curr - last_time) > 10000)	// this condition is for input debouncing
			) {
			prev_time = last_time;
		}
		last_time = curr;
	}
	last_measured_value = digitalRead(WheelSpeed::hall_pin);
}

float WheelSpeed::getWheelspeedMph() const {
	if(prev_time == 0) {	// if it hasnt had two data points yet
		return 0.0f;
	} else {
		return wheelspeedCalc(last_time - prev_time);
	}
}

float WheelSpeed::wheelspeedCalc(uint32_t time_difference_micro) const {
	// microseconds to hours conversion 
	float conv = pow(10,9) * 3.6;
	float hours = time_difference_micro / conv;

	// Wheel speed for this half of a rotation
	// convert from rotations per microsecond to miles per hour
	return half_rotation_in_miles / hours;
}
#ifndef _WHEELSPEED_
#define _WHEELSPEED_

#include <i2c_t3.h>

class WheelSpeed {
	private:
		// static constants needed within this class
		// these two can be configured
		static const int hall_pin = 13;
		static constexpr float wheel_diameter = 18.0f;

		// these constants are dependent on the constants above
		static constexpr float wheel_circumf = 3.1415927 * wheel_diameter;
		static constexpr float half_rotation = wheel_circumf / 2;
		static constexpr float half_rotation_in_miles = half_rotation / 63360;

	public:
		// public interface
		WheelSpeed();
		void checkWheelspeedSensor(); 
		float getWheelspeedMph() const;

	private:
		// private helper functions
		float wheelspeedCalc(uint32_t time_difference_micro) const;

		// private member variables
		uint32_t prev_time;
		uint32_t last_time;
		char last_measured_value;
};

#endif

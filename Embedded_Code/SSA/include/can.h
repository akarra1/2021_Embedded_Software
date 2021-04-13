#ifndef CAN_H
#define CAN_H

#include "FlexCAN.h"			// library used to interface with CAN
#include "imu.h"


// By using a union here, it allows us to unpack data types like doubles into
// An array with 8 8-bit long values
union CanMessageData {
	uint8_t byte_arr[8];	// 8 bytes long (since each uint8_t is 8 bits)
	double doub;			// 8 bytes (double is 8 bytes)
	uint64_t num;			// 8 bytes (64 bit integer)
};

class CanInterface {

private:
	// ensure this id doesn't collide with other CAN nodes (including other SSAs)
	// the data that comes out of the CAN bus will have ID in the range [CAN_ID, CAN_ID+11] inclusive
	const static int CAN_IDENTIFIER = 1;			// arbitrary number (should be diff for each SSA)

	// CAN ID's for the temperature sensors
	const static int TEMP_1_CAN_ID 		= CAN_IDENTIFIER;
	const static int TEMP_2_CAN_ID 		= CAN_IDENTIFIER + 1;
	const static int TEMP_3_CAN_ID 		= CAN_IDENTIFIER + 2;

	const static int ACCEL_X_CAN_ID 	= CAN_IDENTIFIER + 3;
	const static int ACCEL_Y_CAN_ID 	= CAN_IDENTIFIER + 4;
	const static int ACCEL_Z_CAN_ID 	= CAN_IDENTIFIER + 5;
	
	const static int GYRO_X_CAN_ID 		= CAN_IDENTIFIER + 6;
	const static int GYRO_Y_CAN_ID 		= CAN_IDENTIFIER + 7;
	const static int GYRO_Z_CAN_ID 		= CAN_IDENTIFIER + 8;

	const static int WHEELSPEED_CAN_ID 	= CAN_IDENTIFIER + 9;

	const static int GPS_LONG_CAN_ID 	= CAN_IDENTIFIER + 10;
	const static int GPS_LAT_CAN_ID		= CAN_IDENTIFIER + 11;

	// constants for constructing FlexCAN instance
	// see https://github.com/pawelsky/FlexCAN_Library for choice of constants
	const static int CAN_BAUD_RATE = 125000;
	const static int CAN_INTERFACE_ID = 0;
	const static int CAN_TX_ALT = 0;		// use pin 3 for CAN TX
	const static int CAN_RX_ALT = 0;		// use pin 4 for CAN RX


public:
	CanInterface();
	void setupCan();

	void sendAllData(const IMU& imu, float t1, float t2, float t3, float ws, float lat, float lon);

private:
	void sendTemp1(float t);
	void sendTemp2(float t);
	void sendTemp3(float t);

	void sendAccelX(float a);
	void sendAccelY(float a);
	void sendAccelZ(float a);

	void sendGyroX(float p);
	void sendGyroY(float p);
	void sendGyroZ(float p);

	void sendWheelspeed(float mph);

	void sendLongitude(float lon);
	void sendLatitude(float lat);


	void sendDouble(uint32_t can_id, double data);
	void sendMessage(uint32_t can_id, const CanMessageData& msg);

	bool setupSuccessful;
	FlexCAN flexCanInstance;
};


#endif

#include "can.h"// header file for this module

// Public interface for the CAN module

CanInterface::CanInterface() {
	flexCanInstance = FlexCAN(CAN_BAUD_RATE, CAN_INTERFACE_ID, CAN_TX_ALT, CAN_RX_ALT);
	setupSuccessful = false;
}

void CanInterface::setupCan() {
	flexCanInstance.begin();
}

void CanInterface::sendAllData(const IMU& imu, float t1, float t2, float t3, float ws, float lat, float lon) {
	sendTemp1(t1);
	sendTemp2(t2);
	sendTemp3(t3);
	
	sendAccelX(imu.getAccelX());
	sendAccelY(imu.getAccelY());
	sendAccelZ(imu.getAccelZ());

	sendGyroX(imu.getGyroX());
	sendGyroY(imu.getGyroY());
	sendGyroZ(imu.getGyroZ());

	sendWheelspeed(ws);
	sendLongitude(lon);
	sendLatitude(lat);
}





// Helper functions for CAN module

/*
CAN_message typedef from FlexCAN.h

typedef struct CAN_message_t {
  uint32_t id; // can identifier
  uint8_t ext; // identifier is extended
  uint8_t len; // length of data
  uint16_t timeout; // milliseconds, zero will disable waiting
  uint8_t buf[8];
} CAN_message_t;
*/

void CanInterface::sendDouble(uint32_t can_id, double data) {
	CanMessageData msg;
	msg.doub = data;
	sendMessage(can_id, msg);
}

void CanInterface::sendMessage(uint32_t can_id, const CanMessageData& msg) {
	CAN_message_t can_message = CAN_message_t {
		can_id,					// uint32_t id - can identifier
		0,						// uint8_t ext - identifier is extended
		8,						// uint8_t len - length of data
		0,						// uint16_t timeout - milliseconds, zero will disable waiting
		*(msg.byte_arr)			// uint8_t buf[8]
	};
	flexCanInstance.write(can_message);
}


// functions for sending data with the appropriate CAN ID

void CanInterface::sendTemp1(float t) {
	sendDouble(TEMP_1_CAN_ID, t);
} 

void CanInterface::sendTemp2(float t) {
	sendDouble(TEMP_2_CAN_ID, t);
} 

void CanInterface::sendTemp3(float t) {
	sendDouble(TEMP_3_CAN_ID, t);
} 

void CanInterface::sendAccelX(float a) {
	sendDouble(ACCEL_X_CAN_ID, a);
} 

void CanInterface::sendAccelY(float a) {
	sendDouble(ACCEL_Y_CAN_ID, a);
} 

void CanInterface::sendAccelZ(float a) {
	sendDouble(GYRO_Z_CAN_ID, a);
} 

void CanInterface::sendGyroX(float p) {
	sendDouble(GYRO_X_CAN_ID, p);
} 

void CanInterface::sendGyroY(float p) {
	sendDouble(GYRO_Y_CAN_ID, p);
} 

void CanInterface::sendGyroZ(float p) {
	sendDouble(GYRO_Z_CAN_ID, p);
} 

void CanInterface::sendWheelspeed(float mph) {
	sendDouble(WHEELSPEED_CAN_ID, mph);
} 

void CanInterface::sendLongitude(float lon) {
	sendDouble(GPS_LONG_CAN_ID, lon);
} 

void CanInterface::sendLatitude(float lat) {
	sendDouble(GPS_LAT_CAN_ID, lat);
} 
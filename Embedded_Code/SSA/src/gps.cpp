/**********************************************************\

         /+-+\ 
 ,_______|/ \|_______,    Anteater Electric Racing 2021
 |"|"|"|"|   |"|"|"|"|    Ampeater GPS Module : gps.cpp
 ^"""""""|/-\|"""""""^
         / | \         This module controls the acquisition,
           .           parsing, and sending of GPS data from
           .           the onboard GPS sensor. This GPS data 
         (   )         is logged to a SD card on the SSA for
       (       )       use in data analysis.  

        _. |\          Author(s) : Jordan Whiting
     <o""=="'o...~`    Modified  : Oct. 13 2020

\**********************************************************/

#include "gps.h"

GPS::GPS() {
	this->lastUpdated = 0;
	this->lat = 0.0F;
	this->lon = 0.0F;
}

void GPS::initGPS() {
	pinMode(GPS_SCL, INPUT_PULLUP);
	pinMode(GPS_SDA, INPUT_PULLUP);

	Wire.begin();

	Wire.beginTransmission(GPS_ID);

	Wire.write(GPS_SET_MODE);
	Wire.write(GPS_SET_CTL);

	Wire.endTransmission();
}

// C-style strings because Arduino Strings are the spawn of Satan
void GPS::collectData(char* nmeaData) {

	Wire.beginTransmission(GPS_ID);

	int index = 1;
	nmeaData[0] = RMC_PREFIX[0];

	Wire.requestFrom(GPS_ID, PACKET_SIZE - index);

	while (Wire.read() != RMC_PREFIX[0]) {
		if (!Wire.available()) Wire.requestFrom(GPS_ID, PACKET_SIZE - index);
	}

	while (index < PACKET_SIZE && nmeaData[index] != '\n') {
		if (Wire.available()) {
			nmeaData[index] = Wire.read();
			++index;
		} else {
			Wire.requestFrom(GPS_ID, PACKET_SIZE - index);
		}
	}

	Wire.endTransmission();
}

int GPS::parseData(char* nmea) {
	tm t;

	// Check if NMEA sentence has correct prefix
	char* prefix = strtok(nmea, DELIM);
	if (strcmp(prefix,RMC_PREFIX))
		return 1; // WRONG PREFIX

	// Get time ( HHMMSS )
	int tdata = atoi(strtok(NULL, DELIM));

	// Check if NMEA data is Acceptable or Void
	if (strtok(NULL, DELIM)[0] != 'A')
		return 2; // DATA VOID

	// Latitude ( ####.## )
	float lat = atof(strtok(NULL, DELIM)) *
		(strtok(NULL, DELIM)[0] == 'N' ? 1.0F : -1.0F);

	// Longitude ( ####.## )
	float lon = atof(strtok(NULL, DELIM)) *
		(strtok(NULL, DELIM)[0] == 'E' ? 1.0F : -1.0F);

	strtok(NULL, DELIM); // Ground speed, ignored
	strtok(NULL, DELIM); // Track angle, ignored

	// Date ( ddmmyy )
	int date = atoi(strtok(NULL, DELIM));

	// There is technically more data left in the NMEA setence: 
	// the magnetic variation and the checksum. 
	// This data is ignored, as we don't need it.

	t.tm_sec  = tdata % 100;
	t.tm_min  = (tdata % 10000) / 100;
	t.tm_hour = tdata / 10000;
	t.tm_year = (date % 100) + 100; // Assume after 2000
	t.tm_mon  = (date % 10000) / 100 - 1; // Months 0-11
	t.tm_mday = date / 10000;

	this->lastUpdated = mktime(&t);
	this->lat = lat;
	this->lon = lon;

	return 0; 
}

void GPS::updateGPS() {
	char nmea[PACKET_SIZE];
	this->collectData(nmea);
	this->parseData(nmea);
}

uint32_t GPS::getLastUpdate() {
	return this->lastUpdated;
}

float GPS::getLatitude() {
	return this->lat;
}

float GPS::getLongitude() {
	return this->lon;
}

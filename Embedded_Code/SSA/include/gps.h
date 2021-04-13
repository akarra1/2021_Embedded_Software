	/**********************************************************\

         /+-+\ 
 ,_______|/ \|_______,    Anteater Electric Racing 2021
 |"|"|"|"|   |"|"|"|"|     Ampeater GPS Module : gps.h
 ^"""""""|/-\|"""""""^
         / | \         This module controls the acquisition,
           .           parsing, and sending of GPS data from
           .           the onboard GPS sensor. This GPS data 
         (   )         is logged to a SD card on the SSA for
       (       )       use in data analysis.  

        _. |\          Author(s) : Jordan Whiting
     <o""=="'o...~`    Modified  : Oct. 13 2020

\**********************************************************/

#ifndef _GPS_H_
#define _GPS_H_

#include <TimeLib.h>

#include <i2c_t3.h>			// Wire.h for Teensy3.6, as it has 4 sda/scl ports
#include <Arduino.h>

#define GPS_ID  0x10

#define PACKET_SIZE 256
#define RMC_PREFIX "$GNGGA"
#define DELIM ","

/// The following are predefined NMEA MTK Packets for use with the GPS.
/// For more info on how these commands are used, look up "PMTK Commands".

// GPS Reset
#define GPS_FULL_RESTART "$PMTK104*37"

// Set the GPS position fix interval (1 / 1000ms == 1 Hz). Max 10 Hz
#define GPS_SET_CTL "$PMTK300,1000,0,0,0,0*1C\r\n"

/// Enable NMEA RMC (Recommended Minimum)
/// This can be changed later if it's decide more data is needed.
#define GPS_SET_MODE "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"

// This is the response that the GPS module will send over I2C when it
// is done performing setup and is ready for commands
#define GPS_STARTUP_RESPONSE "$PMTK011,MTKGPS*08\r\n$PMTK010,001*2E\r\n"

/**
 * Implementation for the GPS module
 */
class GPS {
public:

	/**
	* GPS default constructor
	*/
	GPS();

	/**
	* Initialize GPS I2C.
	*/
	void initGPS();

	/**
	* Update the GPS with the latest data
	*/
	void updateGPS();

	/**
	 * Get the unix timestamp of the latest fix
	 * @return timestamp
	 */
	uint32_t getLastUpdate();

	/**
	 * Get latitude at latest fix
	 * @return latitude
	 */
	float getLatitude();

	/**
	 * Get longitude at latest fix
	 * @return longitude
	 */
	float getLongitude();

private:
	time_t lastUpdated; ///< Unix timestamp of when the last fix was taken
	float lat, lon;       ///< Lattitude and longitude returned from the fix, float values

	/**
	* Collect data from the GPS sensor over I2C.
	* @param[out] nmeaData the NMEA GPS data string
	*/
	void collectData(char* nmeaData);

	/**
	* Parse NMEA sentence into usable data.
	*
	* @param[in] nmea NMEA sentence from GPS, should be in RMC format.
	* @returns `0` if no errors, `>0` if errors are found.
	*/
	int parseData(char* nmea);
};

#endif // _GPS_H_
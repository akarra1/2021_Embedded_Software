#include "multiplexer.hpp"
#include <Arduino.h>

Multiplexer::Multiplexer() {
	pinMode(mux1_A0, OUTPUT);
	pinMode(mux1_A1, OUTPUT);
	pinMode(mux1_A2, OUTPUT);
	pinMode(mux1_A3, OUTPUT);
	pinMode(mux1_En, OUTPUT);
	
	pinMode(mux2_A0, OUTPUT);
	pinMode(mux2_A1, OUTPUT);
	pinMode(mux2_A2, OUTPUT);
	pinMode(mux2_A3, OUTPUT);
	pinMode(mux2_En, OUTPUT);

	pinMode(mux1_Out, INPUT);
	pinMode(mux2_Out, INPUT);
}

// while these are currently the binary representations of 0-25
// this approach allows us to easily change the address of wires
const unsigned char Multiplexer::lineAddresses[26] = {	
	0b00000,	// mux 1 input  0
	0b00001,	// mux 1 input  1
	0b00010,	// mux 1 input  2
	0b00011,	// mux 1 input  3
	0b00100,	// mux 1 input  4
	0b00101,	// mux 1 input  5
	0b00110,	// mux 1 input  6
	0b00111,	// mux 1 input  7
	0b01000,	// mux 1 input  8
	0b01001,	// mux 1 input  9
	0b01010,	// mux 1 input 10
	0b01011,	// mux 1 input 11
	0b01100,	// mux 1 input 12
	0b01101,	// mux 1 input 13
	0b01110,	// mux 1 input 14
	0b01111,	// mux 1 input 15
	0b10000,	// mux 2 input  0
	0b10001,	// mux 2 input  1
	0b10010,	// mux 2 input  2
	0b10011,	// mux 2 input  3
	0b10100,	// mux 2 input  4
	0b10101,	// mux 2 input  5
	0b10110,	// mux 2 input  6
	0b10111,	// mux 2 input  7
	0b11000,	// mux 2 input  8
	0b11001,	// mux 2 input  9
};

unsigned int Multiplexer::readLine(unsigned char lineNum) {
	bool mux = get_bit(lineNum, 4);
	bool A3 = get_bit(lineNum, 3);
	bool A2 = get_bit(lineNum, 2);
	bool A1 = get_bit(lineNum, 1);
	bool A0 = get_bit(lineNum, 0);

	if(mux) {
		return readFromMux1(A3, A2, A1, A0);
	} else {
		return readFromMux2(A3, A2, A1, A0);
	}
}

unsigned int Multiplexer::readFromMux1(bool A3, bool A2, bool A1, bool A0) {
	digitalWrite(mux1_En, HIGH);
	digitalWrite(mux2_En, LOW);

	digitalWrite(mux1_A3, A3? HIGH: LOW);
	digitalWrite(mux1_A2, A2? HIGH: LOW);
	digitalWrite(mux1_A1, A1? HIGH: LOW);
	digitalWrite(mux1_A0, A0? HIGH: LOW);

	return analogRead(mux1_Out);
}

unsigned int Multiplexer::readFromMux2(bool A3, bool A2, bool A1, bool A0) {
	digitalWrite(mux2_En, HIGH);
	digitalWrite(mux1_En, LOW);

	digitalWrite(mux2_A3, A3? HIGH: LOW);
	digitalWrite(mux2_A2, A2? HIGH: LOW);
	digitalWrite(mux2_A1, A1? HIGH: LOW);
	digitalWrite(mux2_A0, A0? HIGH: LOW);

	return analogRead(mux2_Out);
}

bool Multiplexer::get_bit(unsigned char lineNum, unsigned char position) {
	unsigned char byte = lineAddresses[lineNum];
	return ((byte & (1 << position)) != 0);
}
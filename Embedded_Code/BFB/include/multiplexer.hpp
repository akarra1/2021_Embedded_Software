class Multiplexer {
	private:
	// these integers represent which pins on the teensy correponds to
	// which pins on each mux

	// mux1 input pins
	static const int mux1_A0 = 5;
	static const int mux1_A1 = 6; 
	static const int mux1_A2 = 7;
	static const int mux1_A3 = 8; 
	static const int mux1_En = 9; 

	// mux2 input pins
	static const int mux2_A0 = 35;
	static const int mux2_A1 = 36; 
	static const int mux2_A2 = 37;
	static const int mux2_A3 = 38; 
	static const int mux2_En = 30; 

	// mux output pins
	static const int mux1_Out = 14;
	static const int mux2_Out = 15;


	public:
	// constructs the object to abstract away the two multiplexers
	Multiplexer();

	// this static array contains the addresses of each individual line
	// Each address can be represented bya 5 bit integer
	// [A, B, C, D, E]
	//		A decides which MUX (0 for mux1 and 1 for mux2)
	//		B,C,D and E go to lines A3-A0 on each mux accordingly
	static const unsigned char lineAddresses[26];

	// this function, given a lineNum [0-25] will give an int
	// representing the analog value of that line from 0-1023
	unsigned int readLine(unsigned char lineNum);

	private:
	// this function, given a lineNum [0-25] will give an int
	// representing the analog value of that line from 0-1023
	unsigned int readFromMux1(bool A3, bool A2, bool A1, bool A0);
	unsigned int readFromMux2(bool A3, bool A2, bool A1, bool A0);

	// lineNum ranges from 0 to 25 inclusive (see readLine)
	// given a position that ranges from 0 to 4 inclusive	 (since each address has 5 bits)
	// returns a boolean representing that control bit
	bool get_bit(unsigned char lineNum, unsigned char position);
};
#ifndef _CAN_
#define _CAN_

#include <SPI.h>

//registers
# define CNF_1 0x2A           // control registers for initial setup
# define CNF_2 0x29           // control registers for initial setup
# define CNF_3 0x28           // control registers for initial setup
# define CFG_1 0x00           // control register values for 8MHz
# define CFG_2 0x90           // control register values for 8MHz
# define CFG_3 0x82           // control register values for 8MHz
# define WRITE_INSTRUCTION 0x02
# define MCP2515_CS_LOW 0
# define MCP2515_CS_HIGH 1
# define CAN_MAX_DLEN 8       // number of bytes to send

/* Transmit buffer addresses **********************************************/
// buffer 0
# define MCP_TXB0CTRL 0x30    // transmit buff 0 control reg
# define MCP_TXB0SIDH 0x31    // transmit buff 0 ID reg
# define MCP_TXB0DLC  0x35    // transmit buff 0 Data length reg
# define MCP_TXB0DATA 0x36    // transmit buff 0 Data Byte
// buffer 1
# define MCP_TXB1CTRL 0x40    // transmit buff 1 control reg
# define MCP_TXB1SIDH 0x41    // transmit buff 1 ID reg
# define MCP_TXB1DLC  0x45    // transmit buff 1 Data length reg
# define MCP_TXB1DATA 0x46    // transmit buff 1 Data Byte
// buffer 2
# define MCP_TXB2CTRL 0x50    // transmit buff 2 control reg
# define MCP_TXB2SIDH 0x51    // transmit buff 2 ID reg
# define MCP_TXB2DLC  0x55    // transmit buff 2 Data length reg
# define MCP_TXB2DATA 0x56    // transmit buff 2 Data Byte
/***********************************************************************************/

/*


NOTE

this code was taken from when tEnergia was still being used. 
The following declaration for MCP_csPin is deprecated

Can is currently not working on the SSA



*/
// pin
//const int MCP_csPin = P1_4; // May need to active this depending on SSA data output

const int MCP_csPin = -1;   // see comment above

// CAN data frame class
class CAN_FRAME 
{
    public:
         CAN_FRAME()
         {
            //Jank initialization to make sure no bits are undefined
            for(int i = 0; i < CAN_MAX_DLEN; i++)
            {
                data[i] = 0;
            } 
         }
         uint16_t  can_id;
         unsigned char can_dlen;
         unsigned char data[CAN_MAX_DLEN];   // this is the can data to be sent
};


class CAN 
{
    public:
        CAN();

        // Function Declarations
        void CAN_INIT();
        void CAN_Message(CAN_FRAME cf, int type, int data);
        void INIT_SEND(CAN_FRAME cf, int type);
        enum buffer_type {analog, imu, wheelspeed}; //buffer type for the CAN data
};

#endif
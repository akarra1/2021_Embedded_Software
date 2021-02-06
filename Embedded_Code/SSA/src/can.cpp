/* Author: Michael Barbosa, Andrew Kettle
 * Last modification: August 31st, 2020
 * CAN-BUS SPI communication code
 */
// header files
#include "can.h"

void CAN::CAN_INIT()
{
  pinMode(MCP_csPin, OUTPUT);           // set the mcp chipselect as output
  digitalWrite(MCP_csPin, HIGH);        // set it to high so there is no communication. cs is brought low com start
  SPI.setBitOrder(MSBFIRST);            // sets bit order to shift out MSB first
  SPI.setClockDivider(SPI_CLOCK_DIV2);  // cuts msp clock frequency by 2 from 16Mhz to 8Mhz
  SPI.setDataMode(SPI_MODE0);           // sets the data mode as CPOL=0
  SPI.begin();                          // initialize SPI 

  digitalWrite(MCP_csPin, LOW);
  SPI.transfer(WRITE_INSTRUCTION);  // write a byte to a reg
  SPI.transfer(CNF_1);              // set the reg that we are writing to
  SPI.transfer(CFG_1);              // the date we are writing to the reg above

  SPI.transfer(WRITE_INSTRUCTION);  // write a byte to a reg
  SPI.transfer(CNF_2);              // set the reg that we are writing to
  SPI.transfer(CFG_2);              // the date we are writing to the reg above

  SPI.transfer(WRITE_INSTRUCTION);  // write a byte to a reg
  SPI.transfer(CNF_3);              // set the reg that we are writing to
  SPI.transfer(CFG_3);              // the date we are writing to the reg above
  digitalWrite(MCP_csPin, HIGH);
}

void CAN::INIT_SEND(CAN_FRAME cf, int buffer_num)
{
  digitalWrite(MCP_csPin, LOW);
  SPI.transfer(WRITE_INSTRUCTION);  // set registers to write instruction
  SPI.transfer(MCP_TXB0SIDH);       // select transmit buffer 0 CAN ID reg
  SPI.transfer(cf.can_id);          // send CAN ID
  SPI.transfer(MCP_TXB0DLC);        // select tranmist buffer 0 DATA LENGTH reg
  SPI.transfer(cf.can_dlen); // send CAN DATA LEN
}

void CAN::CAN_Message(CAN_FRAME cf, int type, int data)
{
  SPI.begin();
  INIT_SEND(cf, type);

  SPI.end();
}

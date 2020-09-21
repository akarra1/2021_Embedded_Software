#ifndef _SD_
#define _SD_
/*
   Written and maintained by: 
   Andrew Kettle
   September 20th, 2020
*/
#include <stdint.h>
#include <Energia.h>

//msp430 registers

//Interrupt Vector Addresses -> Datasheet page 11
#define USCI_B0RXIFG 0xFFEE //priority 23 (higher)
#define USCI_B0TXIFG 0xFFEC //priority 22 (lower)

//SFRS -> Datasheet page 12
#define USCI_ENABLE  0x1    //Enables interrupt for USCI data -> Datasheet page 12 
#define USCI_FLAGS   0x3    //Flags for USCI data -> Datsheet page 12

//Universal Serial Communications Interface (USCI) -> description on page 17

//USCI B0 registers
#define UCB0TXBUF    0x6F   //TX buffer
#define UCB0RXBUF    0x6E   //RX buffer
#define UCB0STAT     0x6D   //Stastus Reg
#define UCB0CIE      0x6C   //I2C interrupt enable (not needed)
#define UCB0BR1      0x6B   //bit rate control 1
#define UCB0BR0      0x6A   //bit rate control 0
#define UCB0CTL1     0x69   //bit rate control 0
#define UCB0CTL0     0x68   //bit rate control 0
#define UCB0SA       0x11A  //I2C Slave address (not needed)
#define UCB0OA       0x118  //I2C Own address (not needed)

#endif
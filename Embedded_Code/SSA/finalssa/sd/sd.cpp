/*
   Written and maintained by: 
   Andrew Kettle
   September 20th, 2020
*/

#include "sd.h"

//pins for USCI_B0 -> Datasheet page 6
uint8_t mosi = P1_7; 
uint8_t miso = P1_6;
uint8_t cs = P1_4; 
uint8_t sck = P1_5; 

void spi_init()
{
    //using USCI_B0

}
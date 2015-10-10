/* $Id: SPI.cpp 1163 2011-06-08 03:40:56Z bhagman $
||
|| @author         Hernando Barragan <b@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Brett Hagman <bhagman@wiring.org.co>
|| @contribution   Alexander Brevig <abrevig@wiring.org.co>
|| @contribution   Ralph Doncaster ralphdoncaster AT gmail
||
|| @description
|| | SPI Library.
|| |
|| | Wiring Core Library
|| | stripped down for PicoWiring
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/


#include "SPI.h"


void WSPI::begin()
{
//    pinMode(SS, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);

    // Enable SPI, Master
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
}


void WSPI::end() {
    SPCR &= ~_BV(SPE);
    pinMode(SCK, INPUT);
    pinMode(MOSI, INPUT);
}


// send and receive
uint8_t WSPI::transfer(uint8_t data)
{
  SPDR =  data;
  while(!(SPSR & _BV(SPIF)));
  return SPDR;
}


void WSPI::setClockDivider(uint8_t rate) {
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >>2) & SPI_2XCLOCK_MASK);
}


WSPI SPI;

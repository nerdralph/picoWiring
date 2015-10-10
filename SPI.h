/* $Id: SPI.h 1163 2011-06-08 03:40:56Z bhagman $
||
|| @author         Hernando Barragan <b@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Brett Hagman <bhagman@wiring.org.co>
|| @contribution   Alexander Brevig <abrevig@wiring.org.co>
||
|| @description
|| | SPI Library.
|| |
|| | Wiring Core Library
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/


#ifndef SPI_h
#define SPI_h

#include <Wiring.h>

#define SPI_MASTER 0x01
#define SPI_SLAVE 0x00 

// clock rate bits: SPI2X, SPR1, SPR0 
// SPI2X is on register SPSR
// SPR1 and SPR0 are on register SPCR

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C
 
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR


class WSPI
{
  public:
    void begin();
    static void end();
    uint8_t transfer(uint8_t);
    static void setBitOrder(uint8_t);
    static inline void setDataMode(uint8_t) {};
    static void setClockDivider(uint8_t);
};


static inline void WSPI::setBitOrder(uint8_t bitOrder) {
  if(bitOrder == LSBFIRST) {
    SPCR |= _BV(DORD);
  } else {
    SPCR &= ~(_BV(DORD));
  }
}


extern WSPI SPI;

#endif

/* $Id: WHardwareSerial.cpp 1154 2011-06-07 01:25:23Z bhagman $
||
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Alexander Brevig <abrevig@wiring.org.co>
|| @contribution   gabebear
|| @contribution   Hernando Barragan <b@wiring.org.co>
|| @contribution   Nicholas Zambetti
|| @contribution   Ralph Doncaster
||
|| @description
|| | Hardware Serial class for
|| | Atmel AVR 8 bit microcontroller series.
|| |
|| | Wiring Core API
|| #
||
|| @notes
|| | Utilizes modified FIFO class by Alexander Brevig (2010).
|| | U2X and frame format code by gabebear (2010).
|| | Interface by Hernando Barragan and Nicholas Zambetti (2006).
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/

#include <avr/io.h>
#include <stdlib.h>
#include "WHardwareSerial.h"

// Now, provide the class only if the hardware has at least one serial port.
#if SERIALPORTS > 0

#if !defined(RXCIE)
// UCSRnA bits
#define RXC    7
#define UDRE   5
#define U2X    1
// UCSRnB bits
#define RXCIE  7
#define UDRIE  5
#define RXEN   4
#define TXEN   3
// UCSRnC bits
#define UPM1   5
#define UPM0   4

#endif

// Constructor

HardwareSerial::HardwareSerial(uint8_t serialPortNumber)
{
  switch (serialPortNumber)
  {
    // We do not take into consideration older AVRs with a single UART,
    // such as the m163, m103, etc.  They only have an 8 bit UBRR, and
    // do not have a UCRSC.
#if !defined(SINGLEUSART1)
    case 0:
      #if defined(UBRRL)
      _ubrrh = &UBRRH;
      _ubrrl = &UBRRL;
      _ucsra = &UCSRA;
      _ucsrb = &UCSRB;
      _ucsrc = &UCSRC;
      _udr = &UDR;
      #else
      _ubrrh = &UBRR0H;
      _ubrrl = &UBRR0L;
      _ucsra = &UCSR0A;
      _ucsrb = &UCSR0B;
      _ucsrc = &UCSR0C;
      _udr = &UDR0;
      #endif
      break;
#endif
#if (SERIALPORTS > 1) || defined(SINGLEUSART1)
    case 1:
      _ubrrh = &UBRR1H;
      _ubrrl = &UBRR1L;
      _ucsra = &UCSR1A;
      _ucsrb = &UCSR1B;
      _ucsrc = &UCSR1C;
      _udr = &UDR1;
      break;
#endif
  }
}


// Public Methods

void HardwareSerial::begin(const uint32_t baud,
                           const uint8_t data_bits,
                           const uint8_t stop_bits,
                           const uint8_t parity)
{
  uint16_t ubrrValue;
  uint8_t frame_format;
  uint8_t use_u2x;

  uint16_t calcUBRRU2X;
  uint16_t calcUBRR;
  uint32_t calcBaudU2X;
  uint32_t calcBaud;

  // Calculate for U2X (with rounding)
  calcUBRR = (F_CPU/2/baud + 4) / 8;
  calcUBRRU2X = (F_CPU/baud + 4) / 8;

  calcBaud = F_CPU/16/calcUBRR;
  calcBaudU2X = F_CPU/8/calcUBRRU2X;

  if (abs(calcBaudU2X - baud) < abs(calcBaud - baud))
  {
    *_ucsra = 1 << U2X;
    ubrrValue = calcUBRRU2X - 1;
  }
  else
  {
    *_ucsra = 0;
    ubrrValue = calcUBRR - 1;
  }

  switch (data_bits)
  {
    // UMSZ0, UMSZ1 are in UCSRnC at bits 1 and 2 respectively on
    // all Atmel AVR 8 bit microcontrollers.
    // UMSZ2 is bit 2 found in UCSRnB.
    case 5:
      frame_format = 0b000;
      break;
    case 6:
      frame_format = 0b010;
      break;
    case 7:
      frame_format = 0b100;
      break;
    case 9:
      frame_format = 0b110;
      *_ucsrb |= 0b100;
      break;
    case 8:
    default:
      frame_format = 0b110;
      break;
  }

  // USBSn is found in UCSRnC at bit 3
  if (stop_bits == 2) frame_format |= 0b1000;

  // UPM0 and UPM1 are found in UCSRnC at bits 4 and 5, respectively.
  switch (parity)
  {
    case 1:  // odd parity
      frame_format |= 0b110000;
      break;
    case 2:  // even parity
      frame_format |= 0b100000;
      break;
    default:  // no parity
      break;
  }

  *_ucsrc = frame_format;

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  *_ubrrh = ubrrValue >> 8;
  *_ubrrl = ubrrValue;
  *_ucsrb = (1 << RXEN) | (1 << TXEN);
}


void HardwareSerial::end()
{
  *_ucsrb &= ~(1 << RXEN) | (1 << TXEN);
}


int HardwareSerial::available(void)
{
  return (*_ucsra & (1<<RXC)); 
}


int HardwareSerial::read(void)
{
  if ( available() )
    return (int)*_udr;
  else
    return -1;
}


size_t HardwareSerial::write(uint8_t c)
{
  // We will block here until we have some space free in the FIFO
  while ( !(*_ucsra & (1<<UDRE)) ); 
  *_udr = c;

  return 1;
}


// Preinstantiate Objects


#if !defined(SINGLEUSART1)
HardwareSerial Serial(0);
#endif
#if SERIALPORTS > 1 || defined(SINGLEUSART1)
HardwareSerial Serial1(1);
#endif

#endif // SERIALPORTS > 0

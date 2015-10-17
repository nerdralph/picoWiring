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

#if !defined(SINGLEUSART1)
      #if defined(UBRRL)
      #define _UBRRH UBRRH
      #define _UBRRL UBRRL
      #define _UCSRA UCSRA
      #define _UCSRB UCSRB
      #define _UCSRC UCSRC
      #define _UDR UDR
      #else
      #define _UBRRH UBRR0H
      #define _UBRRL UBRR0L
      #define _UCSRA UCSR0A
      #define _UCSRB UCSR0B
      #define _UCSRC UCSR0C
      #define _UDR UDR0
      #endif
#endif
#if (SERIALPORTS > 1) || defined(SINGLEUSART1)
      #define _UBRRH UBRR1H
      #define _UBRRL UBRR1L
      #define _UCSRA UCSR1A
      #define _UCSRB UCSR1B
      #define _UCSRC UCSR1C
      #define _UDR UDR1
#endif


// Public Methods

void HardwareSerial::begin(const uint32_t baud)
{
  // USART defaults on all supported AVRs to 81N

  uint16_t ubrrValue;

  uint16_t calcUBRRU2X;
  uint32_t calcBaudU2X;

  // Calculate for U2X (with rounding)
  calcUBRRU2X = (F_CPU/baud + 4) / 8;

  calcBaudU2X = F_CPU/8/calcUBRRU2X;

  _UCSRA = 1 << U2X;
  ubrrValue = calcUBRRU2X - 1;

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  _UBRRH = ubrrValue >> 8;
  _UBRRL = ubrrValue;
  _UCSRB = (1 << RXEN) | (1 << TXEN);
}


void HardwareSerial::end()
{
  _UCSRB &= ~(1 << RXEN) | (1 << TXEN);
}


int HardwareSerial::available(void)
{
  return (_UCSRA & (1<<RXC)); 
}


int HardwareSerial::read(void)
{
  if ( available() )
    return _UDR;
  else
    return -1;
}


size_t HardwareSerial::write(uint8_t c)
{
  // We will block here until we have some space free in the FIFO
  while ( !(_UCSRA & (1<<UDRE)) ); 
  _UDR = c;

  return 1;
}


// Preinstantiate
HardwareSerial Serial;

#endif // SERIALPORTS > 0

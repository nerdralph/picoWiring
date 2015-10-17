/* $Id: WHardwareSerial.h 1154 2011-06-07 01:25:23Z bhagman $
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
|| @license Please see cores/Common/License.txt.
||
*/

#ifndef WHARDWARESERIAL_H
#define WHARDWARESERIAL_H

#include <inttypes.h>
#include <avr/interrupt.h>
#include <Stream.h>

#define SERIALPORTS 0

#if defined(USART0_RX_vect)
#undef SERIALPORTS
#define SERIALPORTS 1
#elif defined(USART_RX_vect)
#undef SERIALPORTS
#define SERIALPORTS 1
#elif defined(USART_RXC_vect)
#undef SERIALPORTS
#define SERIALPORTS 1
#endif

#if defined(USART1_RX_vect)
#undef SERIALPORTS
// Some AVRs have 1 USB and a single USART - USART1
#if !defined(USART0_RX_vect)
#define SERIALPORTS 1
#define SINGLEUSART1
#else
#define SERIALPORTS 2
#endif
#endif

// Now, provide the class only if the hardware has at least one serial port.
#if SERIALPORTS > 0

class HardwareSerial : public Stream
{
  private:
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_ucsrc;
    volatile uint8_t *_udr;
    uint8_t peekchar;
  public:
    HardwareSerial(uint8_t SerialPortNumber);
    void begin(const uint32_t baud = 9600,
               const uint8_t data_bits = 8,
               const uint8_t stop_bits = 1,
               const uint8_t parity = 0);
    void end();
    int available(void);
    int read(void);
    int peek(void);
    inline void flush(void){};
    size_t write(uint8_t);
};

#if !defined(SINGLEUSART1)
extern HardwareSerial Serial;
#endif
#if SERIALPORTS > 1 || defined(SINGLEUSART1)
extern HardwareSerial Serial1;
#endif

#endif // SERIALPORTS > 0

#endif
// WHARDWARESERIAL_H

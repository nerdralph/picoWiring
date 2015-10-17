/* 
||
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Hernando Barragan <b@wiring.org.co>
||
|| @description
|| | Digital pin/port control for
|| | Atmel AVR 8 bit microcontroller series core.
|| |
|| | Wiring Core API
|| #
|| 
|| @license Please see cores/Common/License.txt.
||
|| modified 2015 Ralph Doncaster ralphdoncaster at gmail
*/

#ifndef WDIGITAL_H
#define WDIGITAL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define STR1(x) #x
#define STR(x) STR1(x)

#define NOT_A_REG  0
#define NOT_A_PORT 0xFF

#if defined(PORTC)
// m88/168/328 and t48/88
#include "Wiring-xx8.h"
#elif defined(PORTB) && !defined(PORTA)
// 8-pin t13 and tinyx5
#endif

#include <WHardwareSerial.h>
#include <WMath.h>

#define digitalWrite(PIN, VALUE) pinWrite(PIN, VALUE)
#define digitalRead(PIN) pinRead(PIN)

// Pin functions
void _pinMode(uint8_t, uint8_t);
uint8_t _pinRead(uint8_t);
void _pinWrite(uint8_t, uint8_t);

// Port functions
void _portMode(uint8_t, uint8_t);
uint8_t _portRead(uint8_t);
void _portWrite(uint8_t, uint8_t);

static inline void pinMode(uint8_t, uint8_t) __attribute__((always_inline, unused));
static inline void pinMode(uint8_t PIN, uint8_t MODE)
{
    if (MODE)
      *(portModeRegister(digitalPinToPort(PIN))) |= digitalPinToBitMask(PIN);
    else
      *(portModeRegister(digitalPinToPort(PIN))) &= ~digitalPinToBitMask(PIN);  
}


static inline uint8_t pinRead(uint8_t) __attribute__((always_inline, unused));
static inline uint8_t pinRead(uint8_t PIN)
{
    return (*(portInputRegister(digitalPinToPort(PIN))) & digitalPinToBitMask(PIN)) ? HIGH : LOW;
}


// BH: We don't need to turn off interrupts for the
// single instruction expansion macro. (i.e. this static inline
// will be optimized to a single instruction if constants are used)
static inline void pinWrite(uint8_t, uint8_t) __attribute__((always_inline, unused));
static inline void pinWrite(uint8_t PIN, uint8_t VALUE)
{
    if (VALUE)
    {
      *(digitalPinToPortReg(PIN)) |= digitalPinToBitMask(PIN);
    }
    else
    {
      *(digitalPinToPortReg(PIN)) &= ~digitalPinToBitMask(PIN);  
    }
}

void delay(uint16_t millisecs);

// main program prototypes
void setup(void);
void loop(void);


#endif
// WDIGITAL_H

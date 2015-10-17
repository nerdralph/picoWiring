/* $Id: Print.h 1156 2011-06-07 04:01:16Z bhagman $
||
|| @author         Hernando Barragan <b@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Nicholas Zambetti
|| @contribution   Brett Hagman <bhagman@wiring.org.co>
|| @contribution   Alexander Brevig <abrevig@wiring.org.co>
||
|| @description
|| | Print library.
|| |
|| | Wiring Common API
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/

#ifndef PRINT_H
#define PRINT_H

#ifdef __cplusplus

#include <stdint.h>
#include <stdio.h>

#include "WConstants.h"
#include "WString.h"
#include "WConstantTypes.h"
#include "Printable.h"

class Print
{
  public:
    Print() {}
  
    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }

    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }
  
    // print
    size_t print(char);
    size_t print(const char[]);

    size_t print(unsigned long, int = DEC);
    size_t print(long, int = DEC);

    size_t print(unsigned int, int = DEC);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(double, int = 2);

    size_t print(const Printable &p);
    size_t print(const __ConstantStringHelper *cs);
    size_t print(const String &);
  
    // println
    size_t println(void);

    size_t println(const char[]);
    size_t println(char);

    size_t println(unsigned char, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(double, int = 2);

    size_t println(const Printable &p);
    size_t println(const __ConstantStringHelper *cs);
    size_t println(const String &s);

  private:
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
};

#endif  // __cplusplus
#endif
// PRINT_H

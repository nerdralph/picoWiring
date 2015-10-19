/* $Id: Stream.h 1151 2011-06-06 21:13:05Z bhagman $
||
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @url            http://wiring.org.co/
|| @contribution   Alexander Brevig <abrevig@wiring.org.co>
|| @contribution   David A. Mellis
||
|| @description
|| | Base class for streams.
|| |
|| | Wiring Common API
|| #
||
|| @notes
|| | Originally discussed here:
|| |
|| | http://code.google.com/p/arduino/issues/detail?id=60
|| #
||
|| @license Please see cores/Common/License.txt.
||
*/

#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <Print.h>

class Stream : public Print
{
  public:
    //virtual ~Stream() {}
    int available();

    // no peek to keep things simple
    //virtual int peek() = 0;
    int read();
    void flush() {};
  
    Stream() {}
  
  
    size_t readBytes( char *buffer, size_t length); // read chars from stream into buffer
    // terminates if length characters have been read or timeout (see setTimeout)
    // returns the number of characters placed in the buffer (0 means no valid data found)
  
    size_t readBytesUntil( char terminator, char *buffer, size_t length); // as readBytes with terminator character
    // terminates if length characters have been read, timeout, or if the terminator character  detected
    // returns the number of characters placed in the buffer (0 means no valid data found)
  
    // Wiring String functions to be added here
    String readString();
    String readStringUntil(char terminator);
};

#endif
// STREAM_H

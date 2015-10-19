/* $Id: Stream.cpp 1151 2014-08-01 21:13:05Z  $
||
|| @author         David A. Mellis
|| @url            http://wiring.org.co/
|| @contribution   parsing functions based on TextFinder library by Michael Margolis
|| @contribution   Ralph Doncaster
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


#include "Wiring.h"
#include "Stream.h"

#define NO_SKIP_CHAR  1  // a magic char not found in a valid ASCII numeric field

// Public Methods
//////////////////////////////////////////////////////////////

// read characters from stream into buffer
// terminates if length characters have been read, or no more available
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
//
size_t Stream::readBytes(char *buffer, size_t length)
{
  size_t count = 0;
  while (count < length) {
    int c = read();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}


// as readBytes with terminator character
// terminates if length characters have been read, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length)
{
  if (length < 1) return 0;
  size_t index = 0;
  while (index < length) {
    int c = read();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

String Stream::readString()
{
  String ret;
  int c = read();
  while (c >= 0)
  {
    ret += (char)c;
    c = read();
  }
  return ret;
}

String Stream::readStringUntil(char terminator)
{
  String ret;
  int c = read();
  while (c >= 0 && c != terminator)
  {
    ret += (char)c;
    c = read();
  }
  return ret;
}

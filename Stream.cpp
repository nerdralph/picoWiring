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

// find returns true if the target string is found
bool  Stream::find(char *target)
{
  return findUntil(target, "");
}

// reads data from the stream until the target string of given length is found
// returns true if target string is found
bool Stream::find(char *target, size_t length)
{
  return findUntil(target, length, NULL, 0);
}

// as find but search ends if the terminator string is found
bool  Stream::findUntil(char *target, char *terminator)
{
  return findUntil(target, strlen(target), terminator, strlen(terminator));
}

// reads data from the stream until the target string of the given length is found
// search terminated if the terminator string is found
// returns true if target string is found, false if terminated or no data
bool Stream::findUntil(char *target, size_t targetLen, char *terminator, size_t termLen)
{
  size_t index = 0;  // maximum target string length is 64k bytes!
  size_t termIndex = 0;
  int c;
  
  if( *target == 0)
  return true;   // return true if target is a null string
  while( available() && (c = read()) ){
    
    if(c != target[index])
    index = 0; // reset index if any char does not match
    
    if( c == target[index]){
      //////Serial.print("found "); Serial.write(c); Serial.print("index now"); Serial.println(index+1);
      if(++index >= targetLen){ // return true if all chars in the target match
        return true;
      }
    }
    
    if(termLen > 0 && c == terminator[termIndex]){
      if(++termIndex >= termLen)
      return false;       // return false if terminate string found before target string
    }
    else
    termIndex = 0;
  }
  return false;
}


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

/*
  MemStream.h - memory stream for Arduino
 
  (C) 2012-09 Oleg Kobchenko
  http://github.com/tingtronics
  BSD license, see license.txt
 */

#ifndef MemStream_h
#define MemStream_h

#include <avr/pgmspace.h>

class MemStream // : public Stream // has Stream conforming interface
{
private:
  prog_char *_ptr;            // memory start

protected:
  int _p;                     // memory index

public:
  MemStream() { 
  }

  MemStream* begin(char *ptr) {
    _ptr = ptr;
    flush();
    return this;
  }

  int available() {
    return peek() != 0;
  }

  int read() {
    int c = peek();
    if (c != 0) skip();
    return c;
  }

  int peek() {
    return pgm_read_byte(_ptr + _p);
  }

  void flush()   {            // rewind
    _p = 0; // prog array index
  }

  void skip() {
    _p++;
  }

  int parseInt() {
    int num = 0;
    int c = peek();

    while (c >= '0' && c <= '9') {
      num = (num * 10) + (c - '0');
      skip();
      c = peek();
    }
    return num;
  }
};


#endif












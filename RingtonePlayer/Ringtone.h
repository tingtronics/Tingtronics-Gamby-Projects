/*
  Ringtone.h - ringtone player library for Arduino
 
  2012-09 Oleg Kobchenko
 */

#ifndef Ringtone_h
#define Ringtone_h

#include "MemStream.h"

class Ringtone
{
private:
  MemStream *_s;
  char _name[16];

  unsigned char default_dur;
  unsigned char default_oct;
  int bpm;
  long wholenote;

  long _duration;
  unsigned char _note;
  unsigned char _octave;

  int readHeaderItem(char item);
  void readHeader();

public:
  Ringtone() : _s(0) {}
  void begin(MemStream *s);  
  void begin();  
  char* name();  

  int read();
  unsigned char note();       // 0=pause, 1=c, 3=d, 5=e, 6=f, 8=g, 10=a, 12=b
  unsigned char octave();
  long duration();  
};

#endif


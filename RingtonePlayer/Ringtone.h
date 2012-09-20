/*
  Ringtone.h - ringtone player library for Arduino
 
  Partly based on a fun sketch to demonstrate the use of the Tone library.
  by Brett Hagman bhagman@roguerobotics.com, http://www.roguerobotics.com
  http://arduino.cc/forum/index.php/topic,991.msg7782.html#msg7782  

  To mix the output of the signals to output to a small speaker (i.e. 8 Ohms or higher),
  simply use 1K Ohm resistors from each output pin and tie them together at the speaker.
  Don't forget to connect the other side of the speaker to ground!

  You can get more RTTTL (RingTone Text Transfer Language) songs from
  http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation

  (C) 2012-09 Oleg Kobchenko
  http://github.com/tingtronics
  BSD license, see license.txt
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


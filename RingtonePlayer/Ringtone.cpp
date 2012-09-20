/*
  Ringtone.cpp - ringtone player library for Arduino
 
  Partly based on a fun sketch to demonstrate the use of the Tone library.
  by Brett Hagman bhagman@roguerobotics.com, http://www.roguerobotics.com
  http://arduino.cc/forum/index.php/topic,991.msg7782.html#msg7782  

  To mix the output of the signals to output to a small speaker (i.e. 8 Ohms or higher),
  simply use 1K Ohm resistors from each output pin and tie them together at the speaker.
  Don't forget to connect the other side of the speaker to ground!

  You can get more RTTTL (RingTone Text Transfer Language) songs from
  http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation

 
  2012-09 Oleg Kobchenko
 */

#include "Ringtone.h"

#define OCTAVE_OFFSET (0)

void Ringtone::begin(MemStream *s)
{
  _s = s;
  begin();
}

void Ringtone::begin()
{
  default_dur = 4;
  default_oct = 6;
  bpm = 63;
  _name[0] = 0;

  readHeader();
}

char* Ringtone::name()
{
  return _name;
}

long Ringtone::duration()
{
  return _duration;
}

unsigned char Ringtone::note()
{
  return _note;
}

unsigned char Ringtone::octave()
{
  return _octave;
}

int Ringtone::readHeaderItem(char item)
{
  int num = 0;
  if(_s->peek() == item)
  {
    _s->skip(); 
    _s->skip();                         // skip <item> '='
    num = _s->parseInt();
    if(num >= 3 && num <=7) default_oct = num;
    _s->skip();                         // skip ',' or ':'
  }
  return num;
}


void Ringtone::readHeader()
{
  int num = 0;

  while(_s->peek() != ':') { 
    if (num < (sizeof _name) - 1)
      _name[num++] = _s->peek();          // store name and skip up to ':'
    _s->skip();
  }    
  _s->skip();                                  // ':'
  _name[num] = 0;
  
  if ((num = readHeaderItem('d')) > 0)    // get default duration
    default_dur = num;

  num = readHeaderItem('o');              // get default octave
  if (num >= 3 && num <=7)
    default_dur = num;

  if ((num = readHeaderItem('b')) > 0)     // get BPM
    bpm = num;
 
  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)
}

int Ringtone::read()
{
  if (_s == 0) return -1;  // EOF

  int c = _s->peek();

  if (c == 0) return -1;   // EOF

  // first, get note duration, if available
  int num = _s->parseInt();   
  if (num > 0) 
    _duration = wholenote / num;
  else 
    _duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
  _note = 0;

  switch (_s->read())
  {
  case 'c': 
    _note =  1; 
    break;
  case 'd': 
    _note =  3; 
    break;
  case 'e': 
    _note =  5; 
    break;
  case 'f': 
    _note =  6; 
    break;
  case 'g': 
    _note =  8; 
    break;
  case 'a': 
    _note = 10; 
    break;
  case 'b': 
    _note = 12; 
    break;
  case 'p':
  default : 
    _note = 0;
  }

  // now, get optional '#' sharp
  if(_s->peek() == '#')
  {
    _note++;
    _s->skip();
  }

  // now, get optional '.' dotted note
  if (_s->peek() == '.')
  {
    _duration += _duration/2;
    _s->skip();
  }

  // now, get octave
  num = _s->parseInt();   
  if (num > 0) 
    _octave = num;
  else
    _octave = default_oct;

  _octave += OCTAVE_OFFSET;

  if (_s->peek() == ',')
    _s->skip();       // skip comma for next note (or we may be at the end)

  // now play the note
  return _note;
}



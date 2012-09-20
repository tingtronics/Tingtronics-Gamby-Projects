/*
  OkeyDoke.h - smart autorepeating key driver

  OkeyDoke provides original design supporitng non-interfering
  simultanous repeatable and non-repeatable keys with configurable 
  repeat delay and dynamic acceleration.
 
  (C) 2012-09 Oleg Kobchenko
  http://github.com/tingtronics
  BSD license, see license.txt
 */

#ifndef OkeyDoke_h
#define OkeyDoke_h

#include <Arduino.h>

typedef byte (* ReadInputsPtr)();

class OkeyDoke
{
public:
  static const int DEBOUNCE_DELAY = 10;  // ms, 100 Hz
  
  static const int DEFAULT_DELAY  = 500; // ms, 0.8 Hz autorepeat speed
  static const int MIN_DELAY      = DEBOUNCE_DELAY;
  static const int MAX_DELAY      = DEFAULT_DELAY*2;
  
  static const int DEFAULT_ACCEL  = 4;   // new delay = delay * (1 - 1/repeatAccel)
  static const int MIN_ACCEL      = 2;   // 100/2 = 50%
  static const int MAX_ACCEL      = 10;  // 100/10 = 10%

private:
  ReadInputsPtr readInputs;
  int normalDelay;
  int repeatAccel;
  int repeatDelay;
  
public:
  OkeyDoke(ReadInputsPtr readInputsPtr);

  byte keysPressed(byte repeatMask);

  int getRepeatDelay() { return repeatDelay; }
  void setNormalDelay(int newDelay);
  void setRepeatAccel(int newAccel);
};

#endif


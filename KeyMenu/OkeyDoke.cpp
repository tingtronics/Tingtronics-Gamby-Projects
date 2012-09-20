/*
  OkeyDoke.cpp - smart autorepeating key driver
 
  OkeyDoke provides original design supporitng non-interfering
  simultanous repeatable and non-repeatable keys with configurable
  repeat delay and dynamic acceleration.
 
  (C) 2012-09 Oleg Kobchenko
  http://github.com/tingtronics
  BSD license, see license.txt
 */

#include "OkeyDoke.h"

OkeyDoke::OkeyDoke(ReadInputsPtr readInputsPtr) : 
  normalDelay(DEFAULT_DELAY), 
  repeatAccel(DEFAULT_ACCEL)
{ 
  repeatDelay = normalDelay;
  readInputs = readInputsPtr;
}

void OkeyDoke::setNormalDelay(int newDelay)
{
  normalDelay = max(MIN_DELAY, min(newDelay, MAX_DELAY));
}

void OkeyDoke::setRepeatAccel(int newAccel)
{
  repeatAccel = max(MIN_ACCEL, min(newAccel, MAX_ACCEL));
}

/**
 * Returns a debounced subset of readInputs(), which is
 * either newly pressed or repeated from repeatMask after
 * a dynamic delay
 */
byte OkeyDoke::keysPressed(byte repeatMask) {
  static unsigned long debounceTime = 0;
  static unsigned long repeatTime = 0;
  static byte debounceInputs = 0;
  static byte lastInputs = 0;
  static byte repeatInputs = 0;

  unsigned long time = millis();

  if (time < debounceTime) return 0;
  debounceTime = time + DEBOUNCE_DELAY;

  byte inputs = readInputs();
  if (debounceInputs != inputs) {               // input is unstable
    debounceInputs = inputs;
    return 0;
  }

  byte filteredInputs = inputs &~ lastInputs;   // start with new keys (both repeat and non-repeat)

  if (inputs & repeatInputs) {                  // any repeated keys?
    if (time > repeatTime) {
      filteredInputs |= inputs & repeatInputs;  // add repeated keys
      repeatInputs = inputs & repeatMask;
      repeatDelay = max(MIN_DELAY, repeatDelay - repeatDelay/repeatAccel);
      repeatTime = time + repeatDelay;
    }
    // else repeatTime stays the same
  } 
  else {
    repeatInputs = inputs & repeatMask;
    repeatDelay = normalDelay;
    repeatTime = time + repeatDelay;
  }
  lastInputs = inputs;

  return filteredInputs;
}



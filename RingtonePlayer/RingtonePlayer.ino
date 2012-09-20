/*
  Ringtone Player - ringtone player for Gamby
 
  (C) 2012-09 Oleg Kobchenko
  http://github.com/tingtronics
  BSD license, see license.txt
 */

#include <Arduino.h>
#include <Gamby.h>

#include "OkeyDoke.h"
#include "Ringtone.h"
#include "pitches.h"
#include "Songs.h"

extern prog_int32_t font[];
GambyTextMode gamby;

MemStream titleStream;

MemStream memStream;
Ringtone ringtone;

OkeyDoke okey(gambyInputs);

const int lineCount = sizeof songs / sizeof songs[0];
int topLine = 0;
int curLine = 0;
int selLine = -1;  // nothing selected

int tonePin = 9;

PROGMEM prog_uint16_t notes[] = { 
  0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

byte gambyInputs() {
  gamby.readInputs();
  return gamby.inputs;
}

void drawLine(int line, byte row) {
  gamby.setPos(0, row);
  gamby.clearLine();
  gamby.setColumn(5);

  titleStream.begin( (char*)pgm_read_word(songs + line) );
  while(titleStream.peek() != ':') {
    gamby.drawChar(titleStream.peek());
    titleStream.skip();
  }      
}

void printLine(int line) {
  byte row = line - topLine;
  if (row < 0 || 7 < row) return;
  gamby.drawMode = line == curLine ? TEXT_INVERSE : DRAW_NORMAL;

  drawLine(line, row);

  if (line == selLine) {   // draw selection
    gamby.setColumn(0);
    gamby.drawChar('>');
    gamby.setColumn(NUM_COLUMNS - 5);
    gamby.drawChar('<');
  }
}

void keyTone(int freq) {
  if (selLine == -1 && okey.getRepeatDelay() > OkeyDoke::MIN_DELAY) 
    tone(9, freq, 20);
}

/**
 * Play ringtone
 * return: 0 playing previous note, -1 finished all notes
 */
int ringtonePlay()
{
  static unsigned long noteEnd = 0;
  unsigned long time = millis();

  if (time < noteEnd) return 0;
  if (ringtone.read() == -1) return -1;

  noteEnd = millis() + ringtone.duration();
  if (ringtone.note()) {
    int freq = pgm_read_word(notes + (ringtone.octave() - 4) * 12 + ringtone.note());
    tone(tonePin, freq, ringtone.duration());
  } 
  return 1;
}

int ringtoneBegin()
{
  ringtone.begin(memStream.begin( (char*)pgm_read_word(songs + selLine) ));
}

void setup(void)
{
  gamby.font = font;

  for (int i = 0; i < 8; i++) {
    printLine(i);
  }
}

void loop(void)
{
  int prevLine = -2;

  if (selLine != -1) {                                // play only if there is selected line
    if (selLine != -1 && ringtonePlay() == -1) {      // song ended, select new song
      delay(2000);

      prevLine = selLine;
      selLine = -1;
      printLine(prevLine);                            // clear selection

      selLine = (prevLine + 1) % (sizeof songs / sizeof songs[0]);
      ringtoneBegin();
      printLine(selLine);                             // show new selection
    }
  }

  byte keys = okey.keysPressed(DPAD_UP | DPAD_DOWN);

  if (keys & DPAD_UP && curLine > 0) {
    prevLine = curLine--;
    printLine(prevLine);                              // clear selection
    if (curLine < topLine) {
      topLine--;
      gamby.scroll(1);
    }
  }

  if (keys & DPAD_DOWN && curLine + 1 < lineCount) {
    prevLine = curLine++;
    printLine(prevLine);                              // clear selection
    if (curLine > topLine + 7) {
      topLine++;
      gamby.scroll(-1);
    }
  }

  if (keys & DPAD_RIGHT && curLine != selLine) {
    prevLine = selLine;
    selLine = curLine;
    printLine(prevLine);                              // clear selection
    ringtoneBegin();
  }

  if (keys & DPAD_LEFT && selLine != -1) {
    prevLine = selLine;
    selLine = -1;
    printLine(prevLine);                              // clear selection
    noTone(tonePin);
  }

  if (prevLine != -2) {                               // draw new line if position / selection changed
    keyTone(440);
    printLine(curLine);                               // draw line and selection
  }

}











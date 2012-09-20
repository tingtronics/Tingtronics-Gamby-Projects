/*
  KeyMenu - scrolling list with highlighted and selected item
 
  (CC BY-SA) 2012-09 Oleg Kobchenko
 */

#include <Gamby.h>
#include "OkeyDoke.h"

// Bring in the font from the 'font' tab (font.cpp)
extern prog_int32_t font[];

GambyTextMode gamby;
OkeyDoke okey(gambyInputs);

const int lineCount = 25;

int topLine = 0;
int curLine = 0;
int selLine = -1;  // nothing selected

char buf[10];   // shared buffer for itoa

byte gambyInputs() {
  gamby.readInputs();
  return gamby.inputs;
}

void drawChars(char ch, int count) {
  for (int i = count; i-- ;) {
    gamby.drawChar(ch);
  }
}

void drawLine(int line, byte row) {
  int viewLine = line + 1;
  itoa(viewLine, buf, 10);
  byte w = gamby.getTextWidth(buf);

  gamby.setPos(0, row);
  gamby.clearLine();
  gamby.setColumn(2*(10 - viewLine % 10) + 20 - w/2);
  drawChars('.', viewLine % 10);

  gamby.print(" <");
  gamby.print(buf);
  gamby.print("> ");
  
  drawChars('.', viewLine % 10);
}

void printLine(int line, byte row) {
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

void setup () {
  gamby.font = font;

  for (int i = 0; i < 8; i++) {
    printLine(i, i);
  }
}

void keyTone(int freq) {
  if (okey.getRepeatDelay() > OkeyDoke::MIN_DELAY) 
    tone(9, freq, 20);
}

void loop() {
  byte keys = okey.keysPressed(DPAD_UP | DPAD_DOWN);
  int prevLine = -2;
  
  if (keys & DPAD_UP && curLine > 0) {
    prevLine = curLine--;
    printLine(prevLine, prevLine - topLine); // clear selection
    if (curLine < topLine) {
      topLine--;
      gamby.scroll(1);
    }
  }
  
  if (keys & DPAD_DOWN && curLine + 1 < lineCount) {
    prevLine = curLine++;
    printLine(prevLine, prevLine - topLine); // clear selection
    if (curLine > topLine + 7) {
      topLine++;
      gamby.scroll(-1);
    }
  }

  if (keys & DPAD_RIGHT && curLine != selLine) {
    prevLine = selLine;
    selLine = curLine;
    printLine(prevLine, prevLine - topLine); // clear selection
    // run new selection
  }

  if (keys & DPAD_LEFT && selLine != -1) {
    prevLine = selLine;
    selLine = -1;
    printLine(prevLine, prevLine - topLine); // clear selection
    // stop running
  }

  if (prevLine != -2) {  // draw new line if position / selection changed
    keyTone(440);
    printLine(curLine, curLine - topLine);   // draw line and selection
  }

}



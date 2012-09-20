/*
  KeyScroll - dynamically accelerated scrolling lines of text 
 
  (CC BY-SA) 2012-09 Oleg Kobchenko
 */

#include <Gamby.h>
#include "OkeyDoke.h"

// Bring in the font from the 'font' tab (font.cpp)
extern prog_int32_t font[];

GambyTextMode gamby;
OkeyDoke okeyDoke(gambyInputs);

int line = 0;
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
void printLine(int line) {
  itoa(line, buf, 10);
  byte w = gamby.getTextWidth(buf);
  gamby.clearLine();
  gamby.setColumn(2*(10 - line % 10) + 20 - w/2);
  drawChars('.', line % 10);

  gamby.print(" <");
  gamby.print(buf);
  gamby.print("> ");
  
  drawChars('.', line % 10);
}

void setup () {
  gamby.font = font;

  for (int i = 0; i < 8; i++) {
    printLine(i + 1);
    if (i < 7) 
      gamby.newline();
  }
}

void loop() {
  byte keys = okeyDoke.keysPressed(DPAD_ANY);
  
  if (keys & DPAD_UP && line > 0) {
    if (okeyDoke.getRepeatDelay() > OkeyDoke::MIN_DELAY) 
      tone(9, 220, 20);
    line--;
    gamby.scroll(1);
    gamby.setPos(0, 0);
    printLine(line + 1);
  }
  
  if (keys & DPAD_DOWN) {
    if (okeyDoke.getRepeatDelay() > OkeyDoke::MIN_DELAY) 
      tone(9, 440, 20);
    line++;
    gamby.scroll(-1);
    gamby.setPos(0, 7);
    printLine(line + 8);
  }

}



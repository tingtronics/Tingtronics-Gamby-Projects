/*
  TextModeSize - show screen dimentions in Text Mode

  2012-09 Oleg Kobchenko
*/

#include <Gamby.h>

// Bring in the font from the 'font' tab (font.ino)
extern prog_int32_t font[];

GambyTextMode gamby;

char buf[8]; // itoa buffer

void setup () {
  gamby.font = font;

  for (int row = 0; row < 8; row++) {
    gamby.newline();
    gamby.print(itoa(row + 1, buf, 10));
    if (row % 5 == 0) {
      for (int col = 2; col < 20; col++) {
        if (col % 5 == 0) {
          gamby.print("-");
        } else {
          gamby.print(itoa(col % 10, buf, 10));
        }
      }
    }
  }

}

void loop () {
  // Your code here!
}



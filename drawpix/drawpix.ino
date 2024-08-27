#include "src/M5Atom/M5Atom.h"

void setup()
{
  M5.begin(true, false, true);
}

void loop()
{
  int x, y;

  for (x = 0; x < 5; x++) {
    for (y = 0; y < 5; y++) {
      M5.dis.drawpix(x, y, 0xffffff);
    }
  }
  delay(100);
}

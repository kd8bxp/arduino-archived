#include "src/M5Atom/M5Atom.h"
#include "numbers.h"

int UWB_B_NUMBER = 1;

void setup() {
   M5.begin(true, false, true);
    M5.dis.displaybuff(numberImageData);
    M5.dis.displaybuff(numberImageData, (10 - UWB_B_NUMBER ) * 5, 0);
}

void loop() {
  

M5.update();
    delay(50);
}

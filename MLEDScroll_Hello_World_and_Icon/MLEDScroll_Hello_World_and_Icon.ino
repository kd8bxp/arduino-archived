#include <MLEDScroll.h>

MLEDScroll matrix;
String scroll = "Hello World! ";

void setup() {
  matrix.begin();
  matrix.flip=false;
  matrix.message(scroll);
}

void loop() {
  matrix.clear();
  matrix.message(scroll);
  matrix.scroll(SCROLL_RIGHT);
  while (matrix.scroll()!=SCROLL_ENDED) {
    yield();
  }
  matrix.clear();
    matrix.icon(0x101); //WiFi
  delay(1000);
  
  
  matrix.clear();
  matrix.scroll(SCROLL_LEFT);
  for (int i = 0; i < 11; i++) {
  matrix.icon(0x102); //Download
  matrix.message(" ");
  matrix.scroll(SCROLL_DOWN,0);
  while (matrix.scroll()!=SCROLL_ENDED) {
    yield();
  }
  }
  delay(1000);
  matrix.clear();
  matrix.icon(0x103); //Power
  delay(1000);
}

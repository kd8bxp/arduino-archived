#include <Adafruit_NeoPixel.h>

#define N_LEDS 12
#define PIN     9

/*
 * COLOR1 is least bright
 * COLOR2 is medium bright
 * COLOR3 is brightest
 */

//GREEN 
#define COLOR1 0x0F1F00 
#define COLOR2 0x0F3F00
#define COLOR3 0x0FFF00

/*
//PURPLE
#define COLOR1 0x1F001F
#define COLOR2 0x3F003F
#define COLOR3 0xFF00FF
*/

/*
 * mode 0 - knight rider 80s style slow scan
 * mode 1 - knight rider 00s style fast scan
 * mode 2 - strobe color
 * mode 3 - knight rider 80s style medium scan
 * mode 4 - knight rider 80s style fast scan
 * mode 5 - knight rider 00s style medium scan
 * mode 6 - knight rider 00s style slow scan
 */

int mode = 0;
int pos = 0, dir = 1; // Position, direction of "eye"

const int buttonPin = 2;
int buttonState;
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(0, modeChange, CHANGE);
  strip.begin();
}

void modeChange() {
  delay(500);
  mode++;
}

void loop() {
  /*
int reading = digitalRead(buttonPin);
if (reading != lastButtonState) {
  lastDebounceTime = millis();
}
if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        mode++;
      }
    }
}
*/
if (mode > 6) {mode = 0;}
  if (mode == 0){mode0();}
if (mode == 1)  {mode1();}
if (mode == 2) {mode2();}
if (mode == 3) {mode3();}
if (mode == 4) {mode4();}
if (mode == 5) {mode5();}
if (mode == 6) {mode6();}
}


 void mode0() { 
  int j;

  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  delay(100);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode1() {
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(10);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(10);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(10);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(10);
    }
  
  }

void mode2() {
  
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j,COLOR3);
  }
  strip.show();
  delay(100);
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j,COLOR2);
  }
  strip.show();
  delay(100);
  for (int j=0;j<=strip.numPixels();j++){
  strip.setPixelColor(j, COLOR1);
  }
  strip.show();
  delay(100);
}

void mode3() { 
  int j;

  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  delay(75);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode4() { 
  int j;

  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); 
  strip.setPixelColor(pos - 1, COLOR2); 
  strip.setPixelColor(pos    , COLOR3); 
    }
  
  strip.show();
  delay(50);

  // Rather than being sneaky and erasing just the tail pixel,
  // it's easier to erase it all and draw a new one next time.
  for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

  // Bounce off ends of strip
  pos += dir;
  if(pos < 0) {
    pos = 1;
    dir = -dir;
  } else if(pos >= strip.numPixels()) {
    pos = strip.numPixels() - 2;
    dir = -dir;
  }
  
  if (dir < 0) {
  strip.setPixelColor(pos    , COLOR3); 
  strip.setPixelColor(pos + 1, COLOR2); 
  strip.setPixelColor(pos + 2, COLOR1); 
  }
}

void mode5() {
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(20);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(20);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(20);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(20);
    }
  
  }

  void mode6() {
  int half = strip.numPixels()/2;
  for(int j=0; j<=half; j++) {
    strip.setPixelColor(j, COLOR3);
    strip.setPixelColor(j+1,COLOR2);
    strip.setPixelColor(strip.numPixels()-j, COLOR3);
    strip.setPixelColor(strip.numPixels()-(j+1),COLOR2);
    strip.show();
    delay(30);
    strip.setPixelColor(j, 0);
    strip.setPixelColor(strip.numPixels()-j, 0);
    strip.show();
    delay(30);
  }
    for (int j=half; j>=0; j--) {
      strip.setPixelColor(j, COLOR3);
      strip.setPixelColor(strip.numPixels()-j,COLOR3);
      strip.show();
      delay(30);
      strip.setPixelColor(j,0);
      strip.setPixelColor(strip.numPixels()-j,0);
      strip.show();
      delay(30);
    }
  
  }

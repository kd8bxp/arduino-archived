#include <Adafruit_NeoPixel.h>

#define N_LEDS 12
#define PIN    6

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

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  strip.begin();
}

int pos = 0, dir = 1; // Position, direction of "eye"

void loop() {
  int j;

  if (dir == 1 ) {
  strip.setPixelColor(pos - 2, COLOR1); // Dark red
  strip.setPixelColor(pos - 1, COLOR2); // Medium red
  strip.setPixelColor(pos    , COLOR3); // Center pixel is brightest
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

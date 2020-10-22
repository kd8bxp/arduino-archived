#include <Adafruit_NeoPixel.h>

int PIN = 3;
int totalLEDs = 30;
int ledFadeTime = 5;

int isOn = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLEDs, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
	if (isOn == 0) {
		saberon(0, 255, 0, ledFadeTime);
		isOn = 1;
	}
  rgbFadeInAndOut(0, 255, 0, ledFadeTime); // Green
  
  if (isOn == 1) {
  	saberoff();
  	isOn = 0;
  }
  discoleds(ledFadeTime);
}

void rgbFadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
 strip.setPixelColor(0, 255, 0,0);
  for(uint8_t b = 75; b <255; b++) {
     for(uint8_t i=8; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
     }

     strip.show();
     delay(wait);
  };

  for(uint8_t b=255; b > 75; b--) {
     for(uint8_t i = 8; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
     }
     strip.show();
     delay(wait);
  };
};

void saberon(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
	for (uint8_t i = 8; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, red, green, blue);
		strip.show();
		delay(25);
	}
}

void saberoff() {
	for (uint8_t i = strip.numPixels(); i > 7; i--) {
		strip.setPixelColor(i, 0, 0, 0);
		strip.show();
		delay(25);
	}
}

void discoleds(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=8; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
#include <Adafruit_NeoPixel.h>

#define PIN 3
int wait = 100;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, PIN, NEO_RGB);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, PIN, NEO_KHZ400);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, PIN, NEO_RGB + NEO_KHZ400);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 100); // Red
  colorWipe(strip.Color(0, 255, 0), 100); // Green
  colorWipe(strip.Color(0, 0, 255), 100); // Blue
  rainbow(20);
  colorChase(strip.Color(255, 0, 0), 100); // Red
  colorChase(strip.Color(0, 255, 0), 100); // Green
  colorChase(strip.Color(0, 0, 255), 100); // Blu
  rainbowCycle(20);
  colorCyclon(strip.Color(255, 0, 0), 100); // Red
  colorCyclon(strip.Color(0, 255, 0), 100); // Green
  colorCyclon(strip.Color(0, 0, 255), 100); // Blu

rain();
blue();

  almostrain(strip.Color(255, 0, 0), 100); // Red
  almostrain(strip.Color(0, 255, 0), 100); // Green
  almostrain(strip.Color(0, 0, 255), 100); // Blu
  
sortarandom();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
	allOff();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rain() {
	allOff();
	
		//0,17,18
		//25 not working
		
		for(uint16_t i=strip.numPixels(); i>strip.numPixels()-10; i--){
		//uint16_t i=25;
			for (int count = 0; count<5; count++){
		strip.setPixelColor(i, 0, 0, 255);
		strip.show();
		delay(50);
		strip.setPixelColor(i, 0, 0, 0);
		//strip.setPixelColor(((i/3)+3), 0, 0, 255);
		//strip.setPixelColor(((strip.numPixels()-8)/2)-((i/3)+3), 0,0,255);
		strip.setPixelColor(((strip.numPixels()-i)+8), 0,0,255);
		//strip.setPixelColor(i-17, 0, 0, 255);
		strip.show();
		delay(50);
		//strip.setPixelColor(((i/3)+3), 0,0,0);
		strip.setPixelColor(((strip.numPixels()-i)+8), 0,0,0);
		//strip.setPixelColor(((strip.numPixels()-8)/2)-((i/3)+3), 0,0,0);
		//strip.setPixelColor(i-17, 0, 0, 0);
		strip.setPixelColor(i-18, 0, 0, 255);
		strip.show();
		delay(50);
		strip.setPixelColor(i-18, 0, 0, 0);
	}
	}
}

void almostrain(uint32_t c, uint8_t wait) {
	allOff();
	
		//0,17,18
		for(uint16_t i=strip.numPixels(); i>strip.numPixels()-18; i--){
			for (int count = 0; count<5; count++){
		//strip.setPixelColor(18, 0,0,255);
		strip.setPixelColor(i, c);
		strip.show();
		delay(150);
		//strip.setPixelColor(18, 0, 0, 0);
		//strip.setPixelColor(17, 0, 0, 255);
		strip.setPixelColor(i, 0, 0, 0);
		strip.setPixelColor(i-9, c);
		strip.show();
		delay(150);
		//strip.setPixelColor(17, 0,0,0);
		//strip.setPixelColor(0, 0,0, 255);
		strip.setPixelColor(i-9, 0,0,0);
		strip.setPixelColor(i-18, c);
		strip.show();
		delay(150);
		//strip.setPixelColor(0, 0, 0, 0);
		strip.setPixelColor(i-18, 0, 0, 0);
	}
	}
}

void sortarandom() {
	allOff();
	for (int count = 0; count<5; count++){
			for(uint16_t i=strip.numPixels(); i>strip.numPixels()-18; i--){
			//
		strip.setPixelColor(i, random(255),random(255),random(255));
		strip.show();
		delay(100);
		strip.setPixelColor(i, 0, 0, 0);
		strip.setPixelColor(i-9, random(255),random(255),random(255));
		strip.show();
		delay(100);
		strip.setPixelColor(i-9, 0,0,0);
		strip.setPixelColor(i-18, random(255),random(255),random(255));
		strip.show();
		delay(100);
		strip.setPixelColor(i-18, 0, 0, 0);
	}
	}
}

void blue() {
	allOff();
	for (int count = 0; count<3; count++){
		//0,17,18
		for(uint16_t i=strip.numPixels(); i>0; i--){ 
		//strip.setPixelColor(18, 0,0,255);
		strip.setPixelColor(i, 0, 0, 255);
		strip.show();
		delay(150);
		//strip.setPixelColor(18, 0, 0, 0);
		//strip.setPixelColor(17, 0, 0, 255);
		strip.setPixelColor(i, 0, 0, 0);
		strip.setPixelColor(i-9, 0, 0, 255);
		strip.show();
		delay(150);
		//strip.setPixelColor(17, 0,0,0);
		//strip.setPixelColor(0, 0,0, 255);
		strip.setPixelColor(i-9, 0,0,0);
		strip.setPixelColor(i-18, 0, 0, 255);
		strip.show();
		delay(150);
		//strip.setPixelColor(0, 0, 0, 0);
		strip.setPixelColor(i-18, 0, 0, 0);
	}
	}
}


void colorCyclon(uint32_t c, uint8_t wait) {
	allOff();
	for (int count = 0; count<5; count++) {
  for(uint16_t i=0; i<(strip.numPixels()/2); i++) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(strip.numPixels()-i, c);
      strip.show();
      strip.setPixelColor(i, 0,0,0);
      strip.setPixelColor(strip.numPixels()-i, 0,0,0);
      delay(wait);
  }
   for(uint16_t i=(strip.numPixels()/2); i>0; i--) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(strip.numPixels()-i, c);
      strip.show();
      strip.setPixelColor(i, 0,0,0);
      strip.setPixelColor(strip.numPixels()-i, 0,0,0);
      delay(wait);
  }
	}
}


void allOff() {
	  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0,0,0);
      strip.show();
      //delay(wait);
  }
}

void colorChase(uint32_t c, uint8_t wait) {
	allOff();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(100);
      strip.setPixelColor(i, 0,0,0);
      delay(wait);
  }
}




void rainbow(uint8_t wait) {
  uint16_t i, j;
allOff();
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
allOff();
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


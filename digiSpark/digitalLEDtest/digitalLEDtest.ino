// Based on NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license 
#include <Adafruit_NeoPixel.h>
 
// Which pin on the Digispark is connected to the DigiLED?
#define PIN            1
 
// How many DigiLEDs are attached to the Digispark?
#define NUMPIXELS      1
 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// For the WS2812B type through hole LED used by the DigiLED,  NEO_RGB + NEO_KHZ800 is the correct data format
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
 
int delayval = 500; // delay for half a second
 
void setup() 
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
}
 
void loop()
{
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  // in Red,Green,Blue order
 
  pixels.setPixelColor(0, pixels.Color(255, 255, 255)); //white
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
 
  pixels.setPixelColor(0, pixels.Color(0, 255, 255)); //cyan
  pixels.show();
  delay(delayval);
 
  pixels.setPixelColor(0, pixels.Color(255, 0, 255)); //violet
  pixels.show();
  delay(delayval);
 
  pixels.setPixelColor(0, pixels.Color(255, 255, 0)); //yellow
  pixels.show();
  delay(delayval);
 
  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); //red
  pixels.show();
  delay(delayval);
 
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //green
  pixels.show();
  delay(delayval);
 
  pixels.setPixelColor(0, pixels.Color(0, 0, 255)); //blue
  pixels.show();
  delay(delayval);
}

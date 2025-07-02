#include "src/FastLED/FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    33 //26
//#define CLK_PIN   4
#define LED_TYPE    WS2812B //1
#define COLOR_ORDER GRB
#define NUM_LEDS    16 //126
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          85
#define FRAMES_PER_SECOND  120
#define wait 10 //delay

void setup() {
  delay(1000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}



void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
   leds[i] = CRGB::Green; //Yellow;
    //leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
   delay(wait); 
    // now that we've shown the leds, reset the i'th led to black
     leds[i] = CRGB::Black;
    FastLED.show();
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(wait);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CRGB::Green; //Yellow; 
    //leds[i] = CHSV(hue++, 255, 255);
    //leds[i] = CHSV(0,0,0);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
   delay(wait);
   leds[i] = CRGB::Black;
   //leds[i] = CHSV(hue++, 255, 255);
   FastLED.show();
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(wait);
  }

 
}

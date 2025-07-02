#include <stdint.h>
#include <stdio.h>

#include <Arduino.h>
#include "src/FastLED/FastLED.h"
#include "ArduinoTapTempo.h"

#define NUM_LEDS 25
#define MAX_BRIGHTNESS 20
#define DATA_PIN_LED 27
#define BPM_BUTTON 39

static CRGB leds[NUM_LEDS];
ArduinoTapTempo tapTempo;

void setup(void)
{
    Serial.begin(115200);
    FastLED.addLeds < WS2812B, DATA_PIN_LED, GRB > (leds, NUM_LEDS);
    FastLED.setBrightness(MAX_BRIGHTNESS);

    pinMode(BPM_BUTTON, INPUT_PULLUP);
}

static void setled(int x, int y, CRGB c)
{
    leds[y * 5 + x] = c;
}

static void drawHeart()
{
    memset(leds, 0, sizeof(leds));
    CRGB c = CRGB::Red;

    setled(1,0,c);
    setled(3,0,c);

    setled(0,1,c);
    setled(1,1,c);
    setled(2,1,c);
    setled(3,1,c);
    setled(4,1,c);

    setled(0,2,c);
    setled(1,2,c);
    setled(2,2,c);
    setled(3,2,c);
    setled(4,2,c);

    setled(1,3,c);
    setled(2,3,c);
    setled(3,3,c);

    setled(2,4,c);
}


void heartbeat() {
  const uint8_t hbTable[] = {
    25,
    61,
    105,
    153,
    197,
    233,
    253,
    255,
    252,
    243,
    230,
    213,
    194,
    149,
    101,
    105,
    153,
    197,
    216,
    233,
    244,
    253,
    255,
    255,
    252,
    249,
    243,
    237,
    230,
    223,
    213,
    206,
    194,
    184,
    174,
    162,
    149,
    138,
    126,
    112,
    101,
    91,
    78,
    69,
    62,
    58,
    51,
    47,
    43,
    39,
    37,
    35,
    29,
    25,
    22,
    20,
    19,
    15,
    12,
    9,
    8,
    6,
    5,
    3
  };

  #define NUM_STEPS (sizeof(hbTable)/sizeof(uint8_t)) //array size

  drawHeart();
  // beat8 generates index 0-255 (fract8) as per getBPM(). lerp8by8 interpolates that to array index:
  uint8_t hbIndex = lerp8by8( 0, NUM_STEPS, beat8( tapTempo.getBPM() )) ;
  uint8_t brightness = lerp8by8( 0, MAX_BRIGHTNESS, hbTable[hbIndex] ) ; // interpolate to max MAX_BRIGHTNESS

  FastLED.setBrightness( brightness );
  FastLED.show();
}

void updateTempo() {
  static float currentBpm  = tapTempo.getBPM();

  // TapTempo debounces internally already
  if (digitalRead(BPM_BUTTON) == LOW) {
    tapTempo.update(true);
  } else {
    tapTempo.update(false);
  }

  // Print current BPM if it changes
  if(tapTempo.getBPM() != currentBpm){
    currentBpm  = tapTempo.getBPM();
    Serial.println(currentBpm);
  }
}


void loop(void)
{
    heartbeat();
    updateTempo();
    delay(10);
}



 /* 
  -LeRoy Miller, KD8BXp Aug, 18, 2022
  
  */
   

#include "src/M5Atom/M5Atom.h"

CRGB leds[25];
void (*anims[1])(CRGB* leds) {voice};

void setup()
{
    M5.begin(false, false, true);
    FastLED.addLeds<WS2812B, 27>(leds, 25);
    
    FastLED.clear();
    //for (int i = 0; i < 25; ++i)
    //{
    //    leds[i] = 0x000000;
    //}
    FastLED.show();
}

uint8_t animIndex = 0;
int iterations = 1;

void loop()
{
   
    (*anims[animIndex])(leds);
    if (iterations % 10 == 0 ) {
       ++animIndex;
       animIndex%=4; 
    }
    ++iterations;
    
}

void voice(CRGB* leds)
{
   
    leds[12] = 0x00FF00;
    
    FastLED.show();
    delay(500);
    leds[11] = 0x00FF00;
    leds[12] = 0x00FF00;
    leds[13] = 0x00FF00;
    FastLED.show();
    delay(500);
    leds[10] = 0x00FF00;
    leds[11] = 0x00FF00;
    leds[12] = 0x00FF00;
    leds[13] = 0x00FF00;
    leds[14] = 0x00FF00;
    leds[7] = 0x00FF00;
    leds[17] = 0x00FF00;
    FastLED.show();
    delay(500);
    leds[2] = 0x00FF00;
    leds[10] = 0x00FF00;
    leds[11] = 0x00FF00;
    leds[12] = 0x00FF00;
    leds[13] = 0x00FF00;
    leds[14] = 0x00FF00;
    leds[6] = 0x00FF00;
    leds[7] = 0x00FF00;
    leds[8] = 0x00FF00;
    leds[16] = 0x00FF00;
    leds[17] = 0x00FF00;
    leds[18] = 0x00FF00;
    leds[22] = 0x00FF00;
    FastLED.show();
    delay(500);

    leds[2] = 0x000000;
    leds[10] = 0x000000;
    leds[11] = 0x000000;
    leds[12] = 0x00FF00;
    leds[13] = 0x000000;
    leds[14] = 0x000000;
    leds[6] = 0x000000;
    leds[7] = 0x00FF00;
    leds[8] = 0x000000;
    leds[16] = 0x000000;
    leds[17] = 0x00FF00;
    leds[18] = 0x000000;
    leds[22] = 0x000000;
    FastLED.show();
    delay(500);
    leds[10] = 0x000000;
    leds[11] = 0x00FF00;
    leds[12] = 0x00FF00;
    leds[13] = 0x00FF00;
    leds[14] = 0x000000;
    leds[7] = 0x000000;
    leds[17] = 0x000000;
    FastLED.show();
    delay(500);
    leds[11] = 0x000000;
    leds[12] = 0x00FF00;
    leds[13] = 0x000000;
    FastLED.show();
    delay(500);
    leds[12] = 0x000000;
    
    FastLED.show();
    delay(500);
}

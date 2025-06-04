/*
Copyright (c) 2019 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

#include "src/FastLED/FastLED.h"
#include "src/FastLED-GFX/FastLED_GFX.h" //https://github.com/Jorgen-VikingGod/FastLED-GFX

#define LED_PIN            4
#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B
#define BRIGHTNESS      15

#define CANVAS_WIDTH    16
#define CANVAS_HEIGHT   8
#define NUM_LEDS        (CANVAS_WIDTH * CANVAS_HEIGHT)
#define MATRIX_TYPE         HORIZONTAL_ZIGZAG_MATRIX

CRGB leds[NUM_LEDS];

GFXcanvas matrix(CANVAS_WIDTH, CANVAS_HEIGHT);

int x    = matrix.width();
bool flag = false;

void setup() {
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix.getBuffer(), NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);
  matrix.setTextWrap(false);
  matrix.setTextColor(CRGB(0,255,0));
  randomSeed(analogRead(A0));
}

void loop() {
  
  matrix.fillScreen(0);
displayText("Hello", 100); //What to say, and speed of display
matrix.setTextColor(CRGB(random(0,256),random(0,256),random(0,256)));
displayText("World", 50);
matrix.setTextColor(CRGB(random(0,256),random(0,256),random(0,256)));
displayText("12345 6789 12", 100);
displayText("12",50);
displayText("pneumonoultramicroscopicsilicovolcanoconiosis", 75); // this is one of the longest words in English https://en.wikipedia.org/wiki/Longest_word_in_English
}

int displayText(String temp, int speed) {
  flag = false;
  int len = temp.length();
  if (len < 11) { len = ((len*2)/4); } else { len = len/4; }
   do {
    matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(temp);
  
  if (--x < (-36*len)) {
    x = matrix.width();
    flag = true;
      }
      
  FastLED.show();
  delay(speed);
   } while(flag == false);
   
}

/*
Copyright (c) 2018 LeRoy Miller

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

#include <Adafruit_NeoPixel.h>

#define NEOPIXELPIN 13
#define RED strip.Color(155,0,0)
#define GREEN strip.Color(0,155,0)
#define YELLOW strip.Color(140,130,0)
#define OFF strip.Color(0,0,0)

//uint32_t RED, GREEN, YELLOW, OFF;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

byte pixelState = B00000000;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(50);
//RED = strip.Color(155,0,0);
//GREEN = strip.Color(0,155,0);
//YELLOW = strip.Color(140,130,0);
//OFF = strip.Color(0,0,0);

//strip.setPixelColor(7, RED);
//strip.setPixelColor(6, RED);
//strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, YELLOW);
//strip.setPixelColor(2, YELLOW);
//strip.setPixelColor(1, GREEN);
//strip.setPixelColor(0, GREEN);
//strip.show();
}

void loop() {
  
for (int i=0; i<50; i++) {
setPixels(i);
}

for (int i=50; i>0; i--) {
  setPixels(i);
}

}

void setPixels(int i) {  
 // strip.setPixelColor(7,RED);
  if (i >= 25) {
    pixelState = B11111111;
//    strip.setPixelColor(6, RED);
//    strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, YELLOW);
//strip.setPixelColor(2, YELLOW);
//strip.setPixelColor(1, GREEN);
//strip.setPixelColor(0, GREEN);  
  }
  if ( i <= 24 && i > 20 ) {
    pixelState = B11111110;
//    strip.setPixelColor(6, RED);
//   strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, YELLOW);
//strip.setPixelColor(2, YELLOW);
//strip.setPixelColor(1, GREEN);
//strip.setPixelColor(0, OFF);

  }
  if ( i <= 20 &&  i > 17 ) {
pixelState = B11111100;
//       strip.setPixelColor(6, RED);
//   strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, YELLOW);
//strip.setPixelColor(2, YELLOW);
//strip.setPixelColor(1, OFF);
//strip.setPixelColor(0, OFF);
  }
  if ( i<=17 && i> 14) {
pixelState = B11111000;
//       strip.setPixelColor(6, RED);
//   strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, YELLOW);
//strip.setPixelColor(2, OFF);
//strip.setPixelColor(1, OFF);
//strip.setPixelColor(0, OFF);

  }
  if ( i<=14 && i> 10) {
pixelState = B11110000;
//       strip.setPixelColor(6, RED);
//   strip.setPixelColor(5, RED);
//strip.setPixelColor(4, YELLOW);
//strip.setPixelColor(3, OFF);
//strip.setPixelColor(2, OFF);
//strip.setPixelColor(1, OFF);
//strip.setPixelColor(0, OFF);
  }
  if (i<=10 && i>7) {
pixelState = B11100000;
//         strip.setPixelColor(6, RED);
//   strip.setPixelColor(5, RED);
//   strip.setPixelColor(4, OFF);
//   strip.setPixelColor(3, OFF);
//   strip.setPixelColor(2, OFF);
//   strip.setPixelColor(1, OFF);
//   strip.setPixelColor(0, OFF);
  }
  if (i<=7 && i>4) {
pixelState = B11000000;
//         strip.setPixelColor(6, RED);
//         strip.setPixelColor(5, OFF);
//         strip.setPixelColor(4, OFF);
//         strip.setPixelColor(3, OFF);
//         strip.setPixelColor(2, OFF);
//         strip.setPixelColor(1, OFF);
//         strip.setPixelColor(0, OFF);
     }
  if (i<=4 && i>1) {
pixelState = B10000000;
//       strip.setPixelColor(6, OFF);
//         strip.setPixelColor(5, OFF);
//         strip.setPixelColor(4, OFF);
//         strip.setPixelColor(3, OFF);
//         strip.setPixelColor(2, OFF);
//         strip.setPixelColor(1, OFF);
//         strip.setPixelColor(0, OFF);
  }
  if (i == 0) {
    pixelState = B00000000;
//    strip.setPixelColor(7, OFF);
//    strip.setPixelColor(6, OFF);
//         strip.setPixelColor(5, OFF);
//         strip.setPixelColor(4, OFF);
//         strip.setPixelColor(3, OFF);
//         strip.setPixelColor(2, OFF);
//         strip.setPixelColor(1, OFF);
//         strip.setPixelColor(0, OFF);
  }
  
 /*  strip.setPixelColor(7, bitRead(pixelState,0) ? strip.Color(OFF) : strip.Color(RED));
   strip.setPixelColor(6, bitRead(pixelState,1) ? strip.Color(OFF) : strip.Color(RED));
   strip.setPixelColor(5, bitRead(pixelState,2) ? strip.Color(OFF) : strip.Color(RED));
   strip.setPixelColor(4, bitRead(pixelState,3) ? strip.Color(OFF) : strip.Color(YELLOW));
   strip.setPixelColor(3, bitRead(pixelState,4) ? strip.Color(OFF) : strip.Color(YELLOW));
   strip.setPixelColor(2, bitRead(pixelState,5) ? strip.Color(OFF) : strip.Color(YELLOW));
   strip.setPixelColor(1, bitRead(pixelState,6) ? strip.Color(OFF) : strip.Color(GREEN));
   strip.setPixelColor(0, bitRead(pixelState,7) ? strip.Color(OFF) : strip.Color(GREEN));

   strip.setPixelColor(7, bitRead(pixelState,7) ? strip.Color(RED) : strip.Color(OFF));
   strip.setPixelColor(6, bitRead(pixelState,6) ? strip.Color(RED) : strip.Color(OFF));
   strip.setPixelColor(5, bitRead(pixelState,5) ? strip.Color(RED) : strip.Color(OFF));
   strip.setPixelColor(4, bitRead(pixelState,4) ? strip.Color(YELLOW) : strip.Color(OFF));
   strip.setPixelColor(3, bitRead(pixelState,3) ? strip.Color(YELLOW) : strip.Color(OFF));
   strip.setPixelColor(2, bitRead(pixelState,2) ? strip.Color(YELLOW) : strip.Color(OFF));
   strip.setPixelColor(1, bitRead(pixelState,1) ? strip.Color(GREEN) : strip.Color(OFF));
   strip.setPixelColor(0, bitRead(pixelState,0) ? strip.Color(GREEN) : strip.Color(OFF));
*/

   strip.setPixelColor(7, bitRead(pixelState,7) ? RED : OFF);
   strip.setPixelColor(6, bitRead(pixelState,6) ? RED : OFF);
   strip.setPixelColor(5, bitRead(pixelState,5) ? RED : OFF);
   strip.setPixelColor(4, bitRead(pixelState,4) ? YELLOW : OFF);
   strip.setPixelColor(3, bitRead(pixelState,3) ? YELLOW : OFF);
   strip.setPixelColor(2, bitRead(pixelState,2) ? YELLOW : OFF);
   strip.setPixelColor(1, bitRead(pixelState,1) ? GREEN : OFF);
   strip.setPixelColor(0, bitRead(pixelState,0) ? GREEN : OFF);
   
   
     strip.show();
     delay(10);
}



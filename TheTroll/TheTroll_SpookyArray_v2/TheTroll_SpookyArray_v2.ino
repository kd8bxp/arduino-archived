/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.7 July 4, 2019 by LeRoy Miller

Portions of this code lifted from the Adafruit Strandtest example. 
 
If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

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
*/

/* This is still a work in progress, stay tune for updates - Timings still seem off to me.*/

/* Sensors active with HIGH output:
 *      mini PIR output HIGH - use DIP Switch 4 to change if needed connected to External Trigger Pin (Pin 9)
 *      Neopixel ring of 12 pixels - connected to pin A5
 *      Adafruit neopixel library required.
    Use a PIR sensor, 3D printed pumpkin, and neopixels.
    3D printed pumpkin can be found here:
      https://www.thingiverse.com/thing:134066
 */

#include "troll.h"
#include <Adafruit_NeoPixel.h>

troll t(true); // Using the Internal Speaker will rampdown after each use this saves power

#define LED_PIN    A5
#define LED_COUNT 12

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int randomSound; //will pick a random sound from the whole sound library
int trigger; //This will be set to either high or low from DIP switch 4.  So if your external trigger normal output is always a HIGH, then we need to look for a LOW, and if it's a normal output is LOW then we look for a HIGH

bool triggerActive = false; 

int spookyArray[14][14] = {{EVIL_LAUGH1},{EVIL_LAUGH2},{EVIL_LAUGH3},{DYING},{SCREAM1},{SCREAM2},{SCREAM_FEMALE},{ZOMBIE1},{ZOMBIE2},{ZOMBIE3},{SCREAM_GIRL},{BREATHING},{NO_MERCY},{MOMMY}};
unsigned long previousMillis;


void setup() {
t.begin();
strip.begin();
strip.show();
strip.setBrightness(255);
randomSeed(LDR); //We are going to use the LDR on analog zero to set a random seed value.

delay(500);
}

void loop() {
  
   fadeTo(strip.Color(255,25,0),1); //orange
 readSwitches(); //change the dip switches on the fly
 if (t.externalTrigger() == trigger) {
  triggerActive = true;
   } 
   
  while (triggerActive == true) {
 switch (random(0,7)) {

    case 0:
    
   colorWipe(strip.Color(255,   0,   0), 50); // Red
   
   break;
   case 1:
   
   colorWipe(strip.Color(  0, 255,   0), 50); // Green
   
   break;
   case 2:
   
   colorWipe(strip.Color(  0,   0, 255), 50); // Blue
   
   break;
   case 3:
   
   theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  
   break;
   case 4:
   
   theaterChase(strip.Color(0,127,0), 50); // Green, half brightness
   
   break;
   case 5:
   
   theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
   
   break;
      }
    //playSound();
     }
  
 
 
} 

void readSwitches() {
    //we are going to read the switches directly because we need to split the values up
    //for this example we only need to use one of the DIP switches - I choose SW4

if (digitalRead(SW4) == LOW) { trigger = HIGH; } else { trigger = LOW; }

//The switches are defined in the library and can be used in your programs 

}

void playSound() {
  
  //pick a semi-random sound from our soundArray 
randomSound = random(0, 14);
     digitalWrite(LED, HIGH);
     t.sound(spookyArray[randomSound][0],0); //It looks a little weird, but the sound function needs both the address location of the sound, and the duration of the sound.
  digitalWrite(LED,LOW);
  triggerActive = false;
 
}

void fadeTo(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
   }
}

//Lifted from Adafruit Neopixel strandtest example
void colorWipe(uint32_t color, int wait) {
  playSound();
  previousMillis = millis;
  //unsigned long holdingMillis = millis() - previousMillis;
  do {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
   }
    //holdingMillis = millis() - previousMillis;
  } while (millis() - previousMillis <= (long)spookyArray[randomSound][randomSound]);
  delay(1000); //can't trigger while waiting. Hopefully preventing a lot of triggers
}

//Lifted from Adafruit Neopixel strandtest example
void theaterChase(uint32_t color, int wait) {
  playSound();
  previousMillis = millis;
  //unsigned long holdingMillis = millis() - previousMillis;
  do  {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
  //holdingMillis = millis() - previousMillis;
} while (millis() - previousMillis <= (long)spookyArray[randomSound][randomSound]);
delay(1000); //can't trigger while waiting. Hopefully preventing a lot of triggers
}

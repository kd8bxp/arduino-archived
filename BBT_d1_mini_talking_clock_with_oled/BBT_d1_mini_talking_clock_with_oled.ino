/*
Talking Time Clock Example Code.

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

Copyright (c) 2018 LeRoy Miller
Portions of code based on saynumber example
by Matt Ganis (matt.ganis@gmail.com) or @mattganis on Twitter
Copyright (c) 2018 Matt Ganis

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

/* D1 Mini, with a Button connected to pin D3.
 * Connect the BBT (Little Buddy Talker)   
 * D1 Mini Pin      BBT PIN
 *     D7             DI
 *     D5             SC
 *     TX             CS1
 *     RX             CS2
 *     D4             CS3
 *     D0             CS4
 *  
 *  Use a button connected to D3 (Button Shield).
 *  Set your time zone off set in seconds, below.
 *  Set if you want Military Time/24 Hour time  or want to hear AM/PM
 *  
 *  The sketch shows time in the serial console (You can verify the offset is correct this way.
 *  hold the button until it speaks, it will announce the hours and minutes.
 *  The sketch connects to a NTP (Time Server) to grab the time. The NTPClient library is required, see below.
 *  The WiFiManager library is also required, see below.
 *  Possiable Improvement - Display Time on the 0.66" OLED. 
 *  
 *  May 28, 2018 - Updated and simplified sketch to work with BBT Library (better)
 *  
 *  Portions of this code based on SSD1306CLockDemo.ino
 *  Copyright (c) 2016 Daniel Eichhorn 
 *  Copyright (c) 2016 Fabrice Weinberg 
 *  under the MIT License (MIT)
 */
 
#include <NTPClient.h>  //https://github.com/arduino-libraries/NTPClient
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "SSD1306.h" //https://github.com/ThingPulse/esp8266-oled-ssd1306
#include "OLEDDisplayUi.h"
#include "images.h"

#include "Word100BBT.h";

#define BUTTON_PIN D3
#define arr_len( x ) ( sizeof ( x ) / sizeof (*x) )

#define TIMEOFFSET -14400 //Find your Time Zone off set Here https://www.epochconverter.com/timezones OFF Set in Seconds
#define AMPM 1 //1 = AM PM time, 0 = MILITARY/24 HR Time

//The Big Buddy Talker uses 4 CS select pins.
#define CS1_PIN TX 
#define CS2_PIN RX
#define CS3_PIN D4
#define CS4_PIN D0

//Use Word100ham for the secondary "ham" chip of the 100+ Word Shield
Word100bbt Word100(CS1_PIN, CS2_PIN, CS3_PIN, CS4_PIN); 
SSD1306  display(0x3c, D2, D1);
OLEDDisplayUi ui ( &display );

int sentence[4][2] = {{_THE}, {_TIME}, {_IS} };
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org");


int screenW = 128;
int screenH = 64;
int clockCenterX = screenW/2;
int clockCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height
int clockRadius = 23;

 // utility function for digital clock display: prints leading 0
String twoDigits(int digits){
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
//  ui.disableIndicator();

  // Draw the clock face
//  display->drawCircle(clockCenterX + x, clockCenterY + y, clockRadius);
  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
  //
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display->drawLine( x2 + x , y2 + y , x3 + x , y3 + y);
  }

  // display second hand
  float angle = timeClient.getSeconds() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display minute hand
  angle = timeClient.getMinutes() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display hour hand
  angle = timeClient.getHours() * 30 + int( ( timeClient.getMinutes() / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
}

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  if (AMPM) {
    if (timeClient.getHours() > 12) {
      String timenow = String(timeClient.getHours() - 12) + ":" + twoDigits(timeClient.getMinutes());
      display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x , clockCenterY + y, timenow );
} 
  } else {  String timenow = String(timeClient.getHours())+":"+twoDigits(timeClient.getMinutes()); 
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x , clockCenterY + y, timenow );
  }
}


// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { analogClockFrame, digitalClockFrame };

// how many frames are there?
int frameCount = 2;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { clockOverlay };
int overlaysCount = 1;



void setup(){
  Serial.begin(9600);
  ui.setTargetFPS(60);

  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();
  Word100.begin();
  delay(600); //wait for BBT to come online
   WiFiManager wifiManager;
   wifiManager.autoConnect("TalkingClock");
  Serial.println("connected...yeey :)");
  Word100.setDelay(500); //changed default word timings
  Word100.say(_ON);
  Word100.say(_LINE);
  timeClient.begin();
  timeClient.setTimeOffset(TIMEOFFSET);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Word100.setAMPM(AMPM);
}

void loop() {
  
  timeClient.update();
   int remainingTimeBudget = ui.update();
if (remainingTimeBudget >0) {
if (digitalRead(BUTTON_PIN) == LOW) {sayTime();}
  Serial.println(timeClient.getFormattedTime());
  delay(remainingTimeBudget);
      }
      yield();
}

void sayTime() {
  for (int i = 0; i < arr_len(sentence); i++) {
  Word100.say(sentence[i][0],sentence[i][1]);
  
  }
  Word100.sayHours(timeClient.getHours());
  delay(50);
  Word100.sayMinutes(timeClient.getMinutes()); 
  //May 28, 2018 - new issue found - minutes ending in zero (10,20,30,40,50) will not say AM or PM (?)
  
}

/* The code in sayMinute and sayHours is based on saynumber code
 *  example by Matt Ganis (matt.ganis@gmail.com) or @mattganis on Twitter
 *  Copyright (c) 2018 Matt Ganis
 */



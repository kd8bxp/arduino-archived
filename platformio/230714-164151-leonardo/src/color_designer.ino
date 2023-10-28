/*****************************************************************************
 * color_designer.ino
 *
 * This sketch will allow you to use the potentiometers on the Total Control
 * Lighting Developer Shield to adjust the colors of the LEDs and select an
 * appropriate color. Push the first button to send the command which
 * generates this color back over the serial port. The colors will cascade
 * the lighting strand as you adjust the potentiometers. Be sure to open the
 * serial monitor to see the color information on your computer.
 *
 * Copyright 2011-2012 Christopher De Vries
 * This program is distributed under the Artistic License 2.0, a copy of which
 * is included in the file LICENSE.txt along with this library.
 ****************************************************************************/

// July 14 2023 LeRoy Miller - This is using The Total Control Lighting Developers Shield to display cyclon/knight rider style leds
// the shield is only being used as it was a convenant way to hook the Total Control Lighting LEDs strips up.
// FastLED is being used

#include <Arduino.h>
#include <SPI.h>
//#include "TCL.h"
#include "FastLED.h"

#define NUM_LEDS 100 //25
#define DATA_PIN 11
#define CLOCK_PIN 13
#define red 255,0,0
#define red_med 38,0,0
#define red_light 8,0,0
#define off 0,0,0

//int cur_sensorValue = 0;
//int scanDelay = 5; //scanDelay * cur_sensorValue / 10 
int ledDelay = 65;

CRGB leds[NUM_LEDS];

void allOff() {
  for (int i=0; i< NUM_LEDS; i++) {
    leds[i].setRGB(off);
  }
FastLED.show();
}

void allLight() {
for (int i=0; i < 10; i++) {
  leds[i].setRGB(red_light);
}
FastLED.show();
delay(125);
}

void allMed() {
for (int i=0; i < 10; i++) {
  leds[i].setRGB(red_med);
  }
FastLED.show();
delay(125);
}

void allRed() {
for (int i=0; i<10; i++) {
  leds[i].setRGB(red);
}
FastLED.show();
delay(125);
}

void One() {
leds[0].setRGB(red_light);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Two() {
leds[0].setRGB(red_med);
leds[1].setRGB(red_light);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Three() {
leds[0].setRGB(red);
leds[1].setRGB(red_med);
leds[2].setRGB(red_light);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Four() {
leds[0].setRGB(off);
leds[1].setRGB(red);
leds[2].setRGB(red_med);
leds[3].setRGB(red_light);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Five() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(red);
leds[3].setRGB(red_med);
leds[4].setRGB(red_light);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Six() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(red);
leds[4].setRGB(red_med);
leds[5].setRGB(red_light);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Seven() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(red);
leds[5].setRGB(red_med);
leds[6].setRGB(red_light);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Eight() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(red);
leds[6].setRGB(red_med);
leds[7].setRGB(red_light);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Nine() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(red);
leds[7].setRGB(red_med);
leds[8].setRGB(red_light);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Ten() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(red);
leds[8].setRGB(red_med);
leds[9].setRGB(red_light);
FastLED.show();
delay(ledDelay);
}

void Eveleven() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(red);
leds[9].setRGB(red_med);
FastLED.show();
delay(ledDelay);
}

void Twelve() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(red);
FastLED.show();
delay(ledDelay);
}

void Thirteen() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}

void Fourteen() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(red_light);
FastLED.show();
delay(ledDelay);
}

void Fifthteen() {
leds[0].setRGB(off);
leds[1].setRGB(off);
leds[2].setRGB(off);
leds[3].setRGB(off);
leds[4].setRGB(off);
leds[5].setRGB(off);
leds[6].setRGB(off);
leds[7].setRGB(off);
leds[8].setRGB(off);
leds[9].setRGB(off);
FastLED.show();
delay(ledDelay);
}


void initLightBar() {
allOff();
allLight();
allMed();
allRed();
allMed();
allLight();
allOff();
}

void setup() {
  
  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  //pinMode(sensorPin, INPUT);
  initLightBar();
}

void loop() {
  //cur_sensorValue = analogRead( sensorPin);
  
  //  cur_sensorValue = map(cur_sensorValue, 4000, 0, 0, 256);
  //  ledDelay = scanDelay * cur_sensorValue / 10;
  //  if (ledDelay < 20) {ledDelay = 20;}
  //  Serial.println(ledDelay);
  
  //delay(50);
  Fifthteen();
  One();
  Two();
  Three();
  Four();
  Five();
  Six();
  Seven();
  Eight();
  Nine();
  Ten();
  Eveleven();
  Twelve();
  Thirteen();
  //Fourteen();
  //Fifthteen();
  //Fourteen();
  //Thirteen();
  Twelve();
  Eveleven();
  Ten();
  Nine();
  Eight();
  Seven();
  Six();
  Five();
  Four();
  Three();
  Two();
  One();
}


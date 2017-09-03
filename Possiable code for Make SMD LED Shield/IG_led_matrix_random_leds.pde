/*
 LED Matrix - Random LEDs lighting up
 Version: 1.0
 Author: electricteardown (http://insidegadgets.wordpress.com)
 Created: 6/11/2010
 Last Modified: 6/11/2010

*/

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
//Pin connected to DS of 74HC595
int dataPin = 11;

int counter = 0;

int LEDs[8][8] = {   
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}      
};

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  
  LEDs[random(8)][random(8)] = 1;
  if (counter > 200) {
    blankLED();
    counter = 0;
  }
  
  lightLED();
  counter++;
}

void blankLED() {
  int x = 0;
  int y = 0;
  for (x = 0; x < 8; x++) {
    for (y = 0; y < 8; y ++) {
      LEDs[x][y] = 0;
    }
  }
}

void lightLED() {
  byte columnbitsToSend = 0;
  byte rowbitsToSend = 0;
  
  int x = 0;
  int y = 0;
  for (x = 0; x < 8; x++) {
    columnbitsToSend = 0;
    rowbitsToSend = 0;
      
    for (y = 0; y < 8; y ++) {
      if (LEDs[x][y] == 1) {
        bitWrite(rowbitsToSend, y, HIGH);
      } 
    }
    
    digitalWrite(latchPin, LOW);
    bitWrite(columnbitsToSend, x, HIGH);
    shiftOut(dataPin, clockPin, MSBFIRST, columnbitsToSend);
    shiftOut(dataPin, clockPin, MSBFIRST, rowbitsToSend);
    digitalWrite(latchPin, HIGH);   
    delay(2);
  }
}


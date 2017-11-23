/*
 LED Matrix - Display blinking text
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

int letterH[8][8] = {   
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0}      
};

int letterI[8][8] = {   
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},     
};


void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  
  if (counter < 50) {
    lightLED(letterH);
  }
  if (counter > 50) {
    lightLED(letterI);
  }
  if (counter > 100) {
    counter = 0;
  }
  counter++;
}

void lightLED(int tempLED[8][8]) {
  byte columnbitsToSend = 0;
  byte rowbitsToSend = 0;
  
  int x = 0;
  int y = 0;
  for (x = 0; x < 8; x++) {
    columnbitsToSend = 0;
    rowbitsToSend = 0;
      
    for (y = 0; y < 8; y ++) {
      if (tempLED[x][y] == 1) {
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


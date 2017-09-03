/*
 LED Matrix - Display scrolling text
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
int movement = 0;

int LEDs[8][17] = {   
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}      
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
  
  if (movement == 0) {
    int x;
    int y;
    for (x = 0; x < 8; x++) {
      for (y = 0; y < 8; y++) {
        LEDs[x][y+8] = letterH[x][y];
      }
    }
  } 
  if (movement == 8) {
    int x;
    int y;
    for (x = 0; x < 8; x++) {
      for (y = 0; y < 8; y++) {
        LEDs[x][y+8] = letterI[x][y];
      }
    }
  } 
  
  if (counter % 25 == 0) {
    movement++;
    int x;
    int y;
    for (x = 0; x < 8; x++) {
      for (y = 0; y < 16; y++) {
        LEDs[x][y] = LEDs[x][y+1];
      }
    }  
  }
  if (movement >= 16) {
    counter = 0;
    movement = 0;
  }
  
  lightLED();
  counter++;
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


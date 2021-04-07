#include <avr/pgmspace.h>

#define SCROLLSPEED 40

static const char PROGMEM  font5x8[] = {

0x00, 0x00, 0x00, 0x00, 0x00,// (spacja)

0x00, 0x00, 0x5F, 0x00, 0x00,// !

0x00, 0x07, 0x00, 0x07, 0x00,// "

0x14, 0x7F, 0x14, 0x7F, 0x14,// #

0x24, 0x2A, 0x7F, 0x2A, 0x12,// $

0x23, 0x13, 0x08, 0x64, 0x62,// %

0x36, 0x49, 0x55, 0x22, 0x50,// &

0x00, 0x05, 0x03, 0x00, 0x00,// '

0x00, 0x1C, 0x22, 0x41, 0x00,// (

0x00, 0x41, 0x22, 0x1C, 0x00,// )

0x08, 0x2A, 0x1C, 0x2A, 0x08,// *

0x08, 0x08, 0x3E, 0x08, 0x08,// +

0x00, 0x50, 0x30, 0x00, 0x00,// ,

0x08, 0x08, 0x08, 0x08, 0x08,// -

0x00, 0x30, 0x30, 0x00, 0x00,// .

0x20, 0x10, 0x08, 0x04, 0x02,// /

0x3E, 0x51, 0x49, 0x45, 0x3E,// 0

0x00, 0x42, 0x7F, 0x40, 0x00,// 1

0x42, 0x61, 0x51, 0x49, 0x46,// 2

0x21, 0x41, 0x45, 0x4B, 0x31,// 3

0x18, 0x14, 0x12, 0x7F, 0x10,// 4

0x27, 0x45, 0x45, 0x45, 0x39,// 5

0x3C, 0x4A, 0x49, 0x49, 0x30,// 6

0x01, 0x71, 0x09, 0x05, 0x03,// 7

0x36, 0x49, 0x49, 0x49, 0x36,// 8

0x06, 0x49, 0x49, 0x29, 0x1E,// 9

0x00, 0x36, 0x36, 0x00, 0x00,// :

0x00, 0x56, 0x36, 0x00, 0x00,// ;

0x00, 0x08, 0x14, 0x22, 0x41,// <

0x14, 0x14, 0x14, 0x14, 0x14,// =

0x41, 0x22, 0x14, 0x08, 0x00,// >

0x02, 0x01, 0x51, 0x09, 0x06,// ?

0x32, 0x49, 0x79, 0x41, 0x3E,// @

0x7E, 0x11, 0x11, 0x11, 0x7E,// A

0x7F, 0x49, 0x49, 0x49, 0x36,// B

0x3E, 0x41, 0x41, 0x41, 0x22,// C

0x7F, 0x41, 0x41, 0x22, 0x1C,// D

0x7F, 0x49, 0x49, 0x49, 0x41,// E

0x7F, 0x09, 0x09, 0x01, 0x01,// F

0x3E, 0x41, 0x41, 0x51, 0x32,// G

0x7F, 0x08, 0x08, 0x08, 0x7F,// H

0x00, 0x41, 0x7F, 0x41, 0x00,// I

0x20, 0x40, 0x41, 0x3F, 0x01,// J

0x7F, 0x08, 0x14, 0x22, 0x41,// K

0x7F, 0x40, 0x40, 0x40, 0x40,// L

0x7F, 0x02, 0x04, 0x02, 0x7F,// M

0x7F, 0x04, 0x08, 0x10, 0x7F,// N

0x3E, 0x41, 0x41, 0x41, 0x3E,// O

0x7F, 0x09, 0x09, 0x09, 0x06,// P

0x3E, 0x41, 0x51, 0x21, 0x5E,// Q

0x7F, 0x09, 0x19, 0x29, 0x46,// R

0x46, 0x49, 0x49, 0x49, 0x31,// S

0x01, 0x01, 0x7F, 0x01, 0x01,// T

0x3F, 0x40, 0x40, 0x40, 0x3F,// U

0x1F, 0x20, 0x40, 0x20, 0x1F,// V

0x7F, 0x20, 0x18, 0x20, 0x7F,// W

0x63, 0x14, 0x08, 0x14, 0x63,// X

0x03, 0x04, 0x78, 0x04, 0x03,// Y

0x61, 0x51, 0x49, 0x45, 0x43,// Z

0x00, 0x00, 0x7F, 0x41, 0x41,// [

0x02, 0x04, 0x08, 0x10, 0x20,// "\"

0x41, 0x41, 0x7F, 0x00, 0x00,// ]

0x04, 0x02, 0x01, 0x02, 0x04,// ^

0x40, 0x40, 0x40, 0x40, 0x40,// _

0x00, 0x01, 0x02, 0x04, 0x00,// `

0x20, 0x54, 0x54, 0x54, 0x78,// a

0x7F, 0x48, 0x44, 0x44, 0x38,// b

0x38, 0x44, 0x44, 0x44, 0x20,// c

0x38, 0x44, 0x44, 0x48, 0x7F,// d

0x38, 0x54, 0x54, 0x54, 0x18,// e

0x08, 0x7E, 0x09, 0x01, 0x02,// f

0x08, 0x14, 0x54, 0x54, 0x3C,// g

0x7F, 0x08, 0x04, 0x04, 0x78,// h

0x00, 0x44, 0x7D, 0x40, 0x00,// i

0x20, 0x40, 0x44, 0x3D, 0x00,// j

0x00, 0x7F, 0x10, 0x28, 0x44,// k

0x00, 0x41, 0x7F, 0x40, 0x00,// l

0x7C, 0x04, 0x18, 0x04, 0x78,// m

0x7C, 0x08, 0x04, 0x04, 0x78,// n

0x38, 0x44, 0x44, 0x44, 0x38,// o

0x7C, 0x14, 0x14, 0x14, 0x08,// p

0x08, 0x14, 0x14, 0x18, 0x7C,// q

0x7C, 0x08, 0x04, 0x04, 0x08,// r

0x48, 0x54, 0x54, 0x54, 0x20,// s

0x04, 0x3F, 0x44, 0x40, 0x20,// t

0x3C, 0x40, 0x40, 0x20, 0x7C,// u

0x1C, 0x20, 0x40, 0x20, 0x1C,// v

0x3C, 0x40, 0x30, 0x40, 0x3C,// w

0x44, 0x28, 0x10, 0x28, 0x44,// x

0x0C, 0x50, 0x50, 0x50, 0x3C,// y

0x44, 0x64, 0x54, 0x4C, 0x44,// z

0x00, 0x08, 0x36, 0x41, 0x00,// {

0x00, 0x00, 0x7F, 0x00, 0x00,// |

0x00, 0x41, 0x36, 0x08, 0x00,// }

0x08, 0x08, 0x2A, 0x1C, 0x08,// ->

0x08, 0x1C, 0x2A, 0x08, 0x08 // <-

};

int buffer[8];
int graphic[8];

//this is fixed!
int row[8] = {
  0b1000000010000000,
  0b0100000001000000,
  0b0010000000100000,
  0b0001000000010000,
  0b0000100000001000,
  0b0000010000000100,
  0b0000001000000010,
  0b0000000100000001,
};

int smiley[8] = {
  0b0000001111000000,
  0b0000010000100000,
  0b0000101001010000,
  0b0000100000010000,
  0b0000100000010000,
  0b0000100000010000,
  0b0000010000100000,
  0b0000001111000000,
};

int smiley2[8] = {
  0b0000001111000000,
  0b0000010000100000,
  0b0000101001010000,
  0b0000100000010000,
  0b0000101001010000,
  0b0000100110010000,
  0b0000010000100000,
  0b0000001111000000,
};


int love1[8] = {
  0b0000011111000000,
  0b0000000100000000,
  0b0000000100000000,
  0b0000000100000000,
  0b0000000100000000,
  0b0000000100000000,
  0b0000000100000000,
  0b0000011111000000,
};


int love2[8] = {
  0b00000110001100000,
  0b00001111011110000,
  0b00001111111110000,
  0b00001111111110000,
  0b00000111111100000,
  0b00000011111000000,
  0b00000001110000000,
  0b00000000100000000,
};

int love3[8] = {
  0b0000000000000000,
  0b0010000101000100,
  0b0011000100101000,
  0b0010100100010000,
  0b0010010100010000,
  0b0010001100010000,
  0b0010000100010000,
  0b0000000000000000,
};

int flash1[8] = {
  0b1110000000000000,
  0b1110000000000000,
  0b1110000000000000,
  0b0000000000000000,
  0b0000000011100000,
  0b0000000011100000,
  0b0000000011100000,
  0b0000000000000000,
};

int flash2[8] = {
  0b0000000000000000,
  0b0000000011100000,
  0b0000000011100000,
  0b0000000011100000,
  0b1110000000000000,
  0b1110000000000111,
  0b1110000000000111,
  0b0000000000000111,
};

int flash3[8] = {
  0b0000000000000111,
  0b0000000000000111,
  0b0000011100000111,
  0b0000011100000000,
  0b0000011100000000,
  0b0111000000000000,
  0b0111000000000000,
  0b0111000000000000,
};

int flash4[8] = {
  0b0011100000000000,
  0b0011100000011100,
  0b0011100000011100,
  0b0000000000011100,
  0b0000000000000000,
  0b0000000111000000,
  0b0000000111000000,
  0b0000000111000000,
};

int flash5[8] = {
  0b0000000000000000,
  0b0000001110000000,
  0b0000001110000000,
  0b0000001110000000,
  0b0011100000000000,
  0b0011100001110000,
  0b0011100001110000,
  0b0000000001110000,
};

int flash6[8] = {
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
};

char mode;

char dispString[32];

int latchPin = 4;
int dataPin = 3;
int clockPin = 2;//2


int latchPin2 = A2;
int dataPin2 = A1;
int clockPin2 = A0;

int enablePWM1 = 5;
int enablePWM2 = 6;

char rowNum;
char selection;

void initTimer(){
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 30; 
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);  
  
  sei();
}


void setup() {
  
 
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);

  pinMode(enablePWM1, OUTPUT);
  pinMode(enablePWM2, OUTPUT);


  //pinMode(testPin, OUTPUT);

  digitalWrite(enablePWM1, LOW);
  digitalWrite(enablePWM2, LOW);

  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin2, LOW);
  //updateShiftRegister(i); //col
  //updateShiftRegister2(i);//row
  shiftOut(dataPin, clockPin, LSBFIRST, 0b01010000);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000110);
  shiftOut(dataPin2, clockPin2, LSBFIRST, 0b01010000);//row
  shiftOut(dataPin2, clockPin2, LSBFIRST, 0b00000100);//row
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin2, HIGH);

  initTimer();
  selection = 1;
  rowNum = 0;
  
  sprintf(dispString, "Andee"); 
  
  mode = 2;
}



void shiftBuffer(uint8_t data){
  uint8_t j;
  //shift data for each row of buffer
  
  for(uint8_t k=0; k<8; k++){
    buffer[k] <<= 1;
    buffer[k] |= ((data>>k & 0x01));  
  }
}

void shiftGraphic(int *data){
  
  //shift data for each row of buffer
  
  for(uint8_t k=0; k<8; k++){
    graphic[k] = data[k];
    
  }
}

void scrollChar(char c){
  c -= 32;
  for(uint8_t i=0; i<5; i++){
    shiftBuffer(pgm_read_byte((char*)((int)font5x8 + (5*c) + i)));
    delay(SCROLLSPEED);
  }
}
void scrollString(char * myString)	//Function that scrolls a given string onto the display
{
  
  while (*myString)			//Repeat until a null character is encountered (signals end of a string)
  {
    scrollChar(*myString);
    if(*myString != 'i'){
      shiftBuffer(0x00);
      delay(SCROLLSPEED);
    }
    ++myString;				//Increment the point which tracks where we are in the string
  }

}
void scrollClear(void)			//Function that clears the display by shifting everything left
{
  for(uint8_t i=0; i<16; i++)
  {
    shiftBuffer(0x00);
    delay(SCROLLSPEED);
  }
}

unsigned long previousMillis1 = 0;
//unsigned long previousMillis2 = 0;
void loop() 
{ 
  
  
   
  while(millis() - previousMillis1 <= 3000)
  {mode = 2; shiftGraphic(love1);
  delay(800);
  shiftGraphic(love2); delay(1000);} 
  previousMillis1 = millis();
  //delay(2000);  
  //while(millis() - previousMillis2 <= 5000){
  //mode = 2; shiftGraphic(love2); }
  //previousMillis2 = millis();
  
  mode = 5;
  scrollString(" To Make!");
  scrollClear();
  
 /* if(mode == 0){
    scrollString(dispString);
    scrollClear();
  }
  else if (mode == 1){
    shiftGraphic(smiley);
    delay(100);
    shiftGraphic(smiley2);
    delay(100);
  }
  
  else if (mode == 2){
    shiftGraphic(love1);
    delay(800);
    shiftGraphic(love2);
    delay(800);
    
    mode = 5;
    scrollString("Maker Faire");
    scrollClear();
    mode = 2;
    shiftGraphic(flash6);
    
  }
  else if(mode == 3){
    shiftGraphic(flash1);
    delay(100);
    shiftGraphic(flash2);
    delay(100);
    shiftGraphic(flash3);
    delay(100);
    shiftGraphic(flash4);
    delay(100);
    shiftGraphic(flash5);
    delay(100);
    shiftGraphic(flash2);
    delay(100);
    shiftGraphic(flash1);
    delay(100);
    shiftGraphic(flash4);
    delay(100);
    shiftGraphic(flash3);
    delay(100);
    shiftGraphic(flash5);
    delay(100);
    shiftGraphic(flash1);
    delay(100);
    shiftGraphic(flash2);
    delay(100);
    shiftGraphic(flash3);
    delay(100);
    shiftGraphic(flash4);
    delay(100);
    shiftGraphic(flash5);
    delay(100);
    shiftGraphic(flash2);
    delay(100);
    shiftGraphic(flash1);
    delay(100);
    shiftGraphic(flash4);
    delay(100);
    shiftGraphic(flash3);
    delay(100);
    shiftGraphic(flash5);
    delay(100);
    shiftGraphic(flash6);
    //delay(200);
  }  
*/
  
}

void updateShiftRegister(int i){
  digitalWrite(latchPin, LOW);
  
  shiftOut_Int(dataPin, clockPin, LSBFIRST, buffer[i]);
 
  digitalWrite(latchPin2, LOW);
  
  shiftOut_Int(dataPin2, clockPin2, LSBFIRST, row[i]);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin2, HIGH);
}

void updateShiftRegister2(int i){
  digitalWrite(latchPin, LOW);
  
  shiftOut_Int(dataPin, clockPin, LSBFIRST, graphic[i]);
 
  digitalWrite(latchPin2, LOW);
  
  shiftOut_Int(dataPin2, clockPin2, LSBFIRST, row[i]);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin2, HIGH);
}


ISR(TIMER1_COMPA_vect){
  if(mode == 0 || mode == 5)
    updateShiftRegister(rowNum);
  else
    updateShiftRegister2(rowNum);
  rowNum++;
  if(rowNum >= 8)
    rowNum = 0;
  
  
}

void shiftOut_Int(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{
    uint8_t i;

    for (i = 0; i < 16; i++)  {
      if (bitOrder == LSBFIRST)
        digitalWrite(dataPin, !!(val & (1 << i)));
      else      
        digitalWrite(dataPin, !!(val & (1 << (15 - i))));
            
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);            
    }
}

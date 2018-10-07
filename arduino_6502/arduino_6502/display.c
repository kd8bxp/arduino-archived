#include "display.h"
#include <util/delay.h>

void ledOn()
{
  DDRC|=1;

  PORTC|=1;
}

void ledOff()
{
  PORTC&=~1;
}

void initDisplay()
{
	  // set row lines as output
	  DDRD|=(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3);
	  // set col lines as output
	  DDRB|=(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<0);
	  DDRD|=(1<<2);
	  DDRC|=(1<<3);
}
void displayOff()
{
	  // set row lines as input
	  DDRD&=~((1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3));
	  // set col lines as input
	  DDRB&=~((1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<0));
	  DDRD&=~((1<<2));
	  DDRC&=~(1<<3);
}

uint8_t DisplayMem[NUMCOLUMNS];

/**
* @brief Set one column line of the 5x7 matrix active
*
* @param uint8_t col: 0..6
*
* example:
* setCol(2) turns the second column line ON
* to light one pixel there has to be also a active row
* e.g. with setRow(0).
*/
// col 0..6 , anodes
void setCol(uint8_t col)
{
  // the columns are the anodes of the 5x7 matrix
  if(col==6)PORTD|=(1<<2);
  else PORTD&=~(1<<2);

  if(col==5)PORTC|=(1<<3);
  else PORTC&=~(1<<3);

  if(col==4)PORTB|=(1<<5);
  else PORTB&=~(1<<5);

  if(col==3)PORTB|=(1<<4);
  else PORTB&=~(1<<4);

  if(col==2)PORTB|=(1<<3);
  else PORTB&=~(1<<3);

  if(col==1)PORTB|=(1<<2);
  else PORTB&=~(1<<2);

  if(col==0)PORTB|=(1<<0);
  else PORTB&=~(1<<0);
}

/**
* @brief Set one row line of the 5x7 matrix active
*
* @param uint8_t col: 0..4
*
*/
// row 0..4, cathodes
void setRow(uint8_t row)
{
  // the rows are cathodes of the matrix and therefore active low
  PORTD|=(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3); // clear all rows
  // resetting the line will drive the LED
  PORTD&=~(1<<(3+row));
}

/**
* @brief Set multiple row lines, used to switch on multiple LEds
*
* @param uint8_t col: bits to be set, range: b00000..b11111
*
*/
void setRowPattern(uint8_t bitPattern)
{
  uint8_t n;
  for(n=0;n<5;n++)
  {
    if((bitPattern&(1<<n))!=0)PORTD&=~(1<<(3+n));
    else PORTD|=(1<<(3+n));
  }
}

void _showMatrix()
{
  static uint8_t column=0;
//ledOn();
//_ms_delay(1);
//ledOff();
  setRowPattern(DisplayMem[column]);
  //setRows(column+1);
  //ledOn();
  setCol(column);
  //ledOff();
  column++;
  if(column>=NUMCOLUMNS)column=0;
  //if(column>=3)column=0;
}
/***************************************************************************

    show the 5x7 matrix for ms (milli seconds )

***************************************************************************/
void showMatrix(uint16_t ms)
{
  uint16_t n;
  initDisplay();
  for(n=0;n<ms;n++)
  {
    _showMatrix();
    _delay_ms(1);
  }
  displayOff();
}

void _printChar(uint16_t c)
{
  uint8_t n,temp;
  //for(k=0;k<NUMCOLUMNS;k++)
  //{
    temp=0;
    for(n=0;n<5;n++)
    {
      DisplayMem[n]=smallbitmap[c][n];
      //if(smallbitmap[c][n];
    }
  //}
}
/***************************************************************************

    void setPixel(uint8_t x, uint8_t yy, uint8_t intensity)

    set pixel on screen with desired intensity

    intensity: 0,1

***************************************************************************/
void setPixel(uint8_t x, uint8_t y, uint8_t intensity)
{
  if((x<NUMCOLUMNS)&&(y<NUMROWS))
  {
    if(intensity>0)
    {
      DisplayMem[NUMCOLUMNS-1-x]|=1<<y;
    }else DisplayMem[NUMCOLUMNS-1-x]&=~(1<<y);
  }
}
// clear screen
void cls()
{
  uint8_t n;
  for(n=0;n<NUMCOLUMNS;n++)
  {
    DisplayMem[n]=0;
  }

}
#define SHIFTLEFT 4
void printCode(uint16_t c)
{
  uint8_t n,k,temp;
  cls();
  for(k=0;k<3;k++)
  {
    temp=0;
    for(n=0;n<NUMROWS;n++)
    {
      if((smallbitmap[c][n]<<k)&0x80)setPixel(k+SHIFTLEFT,NUMROWS-1-n,1);
    }
  }
}
/***************************************************************************
*/
void print2Code(uint16_t c, uint16_t c2)
{
  uint8_t n,k,temp;
  cls();
  for(k=0;k<3;k++)
  {
    temp=0;
    for(n=0;n<NUMROWS;n++)
    {
      if((smallbitmap[c2][n]<<k)&0x80)
        setPixel(k+SHIFTLEFT,NUMROWS-1-n,1);
      
      if((smallbitmap[c][n]<<k)&0x80)
        setPixel(k,NUMROWS-1-n,1);
    }
  }
}
/***************************************************************************

  print one hex digit

  input: uint8_t 0..F

***************************************************************************/
void hex1(uint8_t x)
{
  if(x<10)printCode(x);
  else printCode(x+3);
}

/***************************************************************************

  print ASCII code to matrix display

***************************************************************************/
void _putchar(int8_t c)
{
  uint8_t k;
  if((c>='0')&&(c<='9'))k=c-'0'; // 0..9
  if((c>='A')&&(c<='Z'))k=c-'A'+13;
  if((c>='a')&&(c<='z'))k=c-'a'+13;
  printCode(k);
}
void _put2char(int8_t c, int8_t c2)
{
  uint8_t k, k2;

  if((c>='0')&&(c<='9'))k=c-'0'; // 0..9
  if((c>='A')&&(c<='Z'))k=c-'A'+13;
  if((c>='a')&&(c<='z'))k=c-'a'+13;

  if((c2>='0')&&(c2<='9'))k2=c2-'0'; // 0..9
  if((c2>='A')&&(c2<='Z'))k2=c2-'A'+13;
  if((c2>='a')&&(c2<='z'))k2=c2-'a'+13;

  print2Code(k,k2);
}

/***************************************************************************

  Use the leftmost pixels of the 5x7 matrix display as "virtual leds".

  led0  x    x    x    x    x    x
  led1  x    x    x    x    x    x
  led2  x    x    x    x    x    x
  led3  x    x    x    x    x    x
  led4  led5 led6 x    x    x    x

  led7: external ultra bright LED

***************************************************************************/
void showLeds(uint8_t k)
{
  uint8_t n;
  for(n=0;n<5;n++)
  {
    if((k>>n)&1)setPixel(0,4-n,1);
    else setPixel(0,4-n,0);
  }
  if((k>>5)&1)setPixel(1,0,1);
  else setPixel(1,0,0);
  if((k>>6)&1)setPixel(2,0,1);
  else setPixel(2,0,0);
  if((k>>7)&1)ledOn();
  else ledOff();
}




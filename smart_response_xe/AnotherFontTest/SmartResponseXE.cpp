//
// SMART Response XE library
//
// LCD and keyboard routines for the SRXE handheld classroom communicator
//
// Copyright (c) 2018 BitBank Software, Inc.
// written by Larry Bank
// email: bitbank@pobox.com
// Project started 8/4/2018
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// April 10, 2021 Added Inverse Code (this will reverse the screen)
// SRXEInverse(0) normal, SRXEInverse(1) reverse after setting it is recommended to also clear the screen
// Added SRXEClearScreen() - functionally the same as SRXEFill(0)
// removed the need to set the iCSPin, iDCPin, iResetPin when calling SRXEInit() (simplified hopefully)
// LeRoy Miller, KD8BXP April 2021

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include "SmartResponseXE.h"
//#include "font.h"
//#include <SPI.h>

// The power button is connected to PD2 (RX1) and signals INT2
#define POWER_BUTTON 0xd2

// Mapping of keyboard to GPIO pins
//static byte rowPins[ROWS] = {6,35,34,8,9,0};
//static byte colPins[COLS] = {4,A1,A3,2,1,25,16,19,23,22};
const uint8_t rowPins[ROWS] = {0xe6, 0xb7, 0xb6, 0xb5, 0xb4, 0xe0};
const uint8_t colPins[COLS] = {0xe4, 0xf1, 0xf3, 0xe2, 0xe1, 0xd7, 0xa0, 0xa5, 0xd5, 0xd4};
static byte bKeyMap[COLS]; // bits indicating pressed keys
static byte bOldKeyMap[COLS]; // previous map to look for pressed/released keys
/* fdufnews 01/2020
Corrected second position of the array
was previously  :
static byte bColorToByte[4] = {0, 0x4a, 0x92, 0xff};
*/
static byte bColorToByte[4] = {0, 0x49, 0x92, 0xff};
static byte iCSPin, iDCPin, iResetPin;

static int iScrollOffset;
void SRXEFill(byte ucData);
static void SRXEWriteCommand(unsigned char c);

byte setInverse = 0; //Default, normal display

typedef enum
{
 MODE_DATA = 0,
 MODE_COMMAND
} DC_MODE;

#ifndef HIGH
#define HIGH 1
#define LOW 0
#define INPUT 0
#define INPUT_PULLUP 1
#define OUTPUT 2
#endif

// Chip select for the external 1Mb flash module
#define CS_FLASH 0xd3

//Keyboard
//Logical Layout (SK# are screen keys: top to bottom 1-5 on left, 6-10 on right):
//                ROW1|ROW2|ROW3|ROW4|ROW5|ROW6|ROW7|ROW8|ROW9|ROW10
//           COL1    1|   2|   3|   4|   5|   6|   7|   8|   9|    0
//           COL2    Q|   W|   E|   R|   T|   Y|   U|   I|   O|    P
//           COL3    A|   S|   D|   F|   G|   H|   J|   K|   L| Bksp
//           COL4 Shft|   Z|   X|   C|   V|   B|   N|   M|Entr|   Up
//           COL5  Sym|Frac|Root| Exp| Spc|   ,|   .|Down|Left|Right
//           COL6  SK1| SK2| SK3| SK4| SK5| SK6| SK7| SK8| SK9| SK10
byte OriginalKeys[] = {'1','2','3','4','5','6','7','8','9','0',
                       'q','w','e','r','t','y','u','i','o','p',
                       'a','s','d','f','g','h','j','k','l',8,
                       0  ,'z','x','c','v','b','n',0x5,0,0x4, // 5 = down, 4 = up
                       0  ,0xd,  0,  0,' ',',','.','m',  2,  3, // 2 = left, 3 = right
                       0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9};
byte ShiftedKeys[] =  {'1','2','3','4','5','6','7','8','9','0',
                       'Q','W','E','R','T','Y','U','I','O','P',
                       'A','S','D','F','G','H','J','K','L',8,
                       0  ,'Z','X','C','V','B','N',0x5,0,0x4, // 5 = down
                       0  ,0xd,  0,  0,'_',',','.','M',  2,  3,
                       0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9};
byte SymKeys[] =       {'!','2','3','$','%','6','\'','\"','(',')',
                       'q','w','e','r','t','y','u','i','[',']',
                       '=','+','-','f','g','h','j',':','?',8,
                        0 ,'z','x','c','v','b','n',0x5,0,0x4,
                        0 ,0xd, 0 , 0 ,0x1,'<','>','m', 2, 3, // 1 = menu
                       0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9};

//
// Power on the LCD
//
const char powerup[] PROGMEM = {
1, 0x01, // soft reset
99, 120, // 120ms delay
1, 0x11,  // sleep out
1, 0x28,  // display off
99, 50, // 50ms delay
3, 0xc0, 0xf8, 0x00, // Vop = 0xF0
2, 0xc3, 0x04, // BIAS = 1/14
2, 0xc4, 0x05, // Booster = x8
2, 0xd0, 0x1d, // Enable analog circuit
2, 0xb3, 0x00, // Set FOSC divider
2, 0xb5, 0x8b, // N-Line = 0
1, 0x38,       // Set grayscale mode (0x39 = monochrome mode)
2, 0x3a, 0x02, // Enable DDRAM interface
2, 0x36, 0x00, // Scan direction setting
2, 0xB0, 0x9f, // Duty setting (0x87?)
5, 0xf0, 0x12,0x12,0x12,0x12, // 77Hz frame rate in all temperatures
1, 0x20, // Display inversion off
1, 0x29, // Display ON
0};


uint8_t getPinInfo(uint8_t pin, volatile uint8_t **iDDR, volatile uint8_t **iPort, int bInput)
{
  uint8_t port, bit;
  
  port = (pin & 0xf0); // hex port (A,B,D,E,F)
  bit = pin & 0x7;
  switch (port)
  {
    case 0xA0: // really port G
      *iPort = (bInput) ? &PING : &PORTG;
      *iDDR = &DDRG;
      break;
    case 0xB0:
      *iPort = (bInput) ? &PINB : &PORTB;
      *iDDR = &DDRB;
      break;
    case 0xD0:
      *iPort = (bInput) ? &PIND : &PORTD;
      *iDDR = &DDRD;
      break;
    case 0xE0:
      *iPort = (bInput) ? &PINE : &PORTE;
      *iDDR = &DDRE;
      break;
    case 0xF0:
      *iPort = (bInput) ? &PINF : &PORTF;
      *iDDR = &DDRF;
      break;
  }
  return bit;
} /* getPinInfo() */
//
// Simplified pin numbering scheme uses a hex number to specify the port number
// and bit. Top 4 bits = port (B/D/E/F/G), bottom 3 bits specify the bit of the port
// e.g. 0xB4 = PORTB, bit 4, 0Ax is for port G
//
void mypinMode(uint8_t pin, uint8_t mode)
{
  uint8_t bit;
  volatile uint8_t *iPort, *iDDR;
  
  bit = getPinInfo(pin, &iDDR, &iPort, 0);
  switch (mode)
  {
    case INPUT:
      *iDDR &= ~(1<<bit);
      break;
    case INPUT_PULLUP:
      *iDDR |= (1<<bit);
      *iPort |= (1<<bit); // set the output high, then set it as an input
      *iDDR &= ~(1<<bit);
      break;
    case OUTPUT:
      *iDDR |= (1<<bit);
      break;
  }
} /* mypinMode() */

void mydigitalWrite(uint8_t pin, uint8_t value)
{
  uint8_t bit;
  volatile uint8_t *iPort, *iDDR;
  
  bit = getPinInfo(pin, &iDDR, &iPort, 0);
  if (value == LOW)
  {
    *iPort &= ~(1<<bit);
  }
  else
  {
    *iPort |= (1<<bit);
  }
} /* mydigitalWrite() */

uint8_t mydigitalRead(uint8_t pin)
{
  uint8_t bit;
  volatile uint8_t *iPort, *iDDR;
  
  bit = getPinInfo(pin, &iDDR, &iPort, 1);
  if (*iPort & (1<<bit))
    return HIGH;
  else
    return LOW;
} /* mydigitalRead() */
//
// Called when the power button is pressed to wake up the system
// Power up the display
//
ISR (INT2_vect)
{
  // cancel sleep as a precaution
  sleep_disable();
} 
//
// Put the device in a deep sleep to save power
// Wakes up when pressing the "power" button
//
void SRXESleep(void)
{
  // Turn off the LCD
  SRXEPowerDown();

  TRXPR = 1 << SLPTR; // send transceiver to sleep

  // disable ADC
  ADCSRA = 0;  
  DDRD &= ~(1 << PORTD2); //PIN INT2 as input
  PORTD |= (1 << PORTD2); // pull-up resistor, the pin is forced to 1 if nothing is connected
  EIMSK &= ~(1 << INT2); //disabling interrupt on INT2
  EICRA &= ~((1<<ISC21) | (1<<ISC20)); // low level triggers interrupt
  EIFR |= (1 << INTF2); //clear interrupt flag
  EIMSK |= (1 << INT2); //enabling interrupt flag on INT2

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  sleep_enable();
 
  // turn off brown-out enable in software
  // BODS must be set to one and BODSE must be set to zero within four clock cycles
//  MCUCR = bit (BODS) | bit (BODSE);
  // The BODS bit is automatically cleared after three clock cycles
//  MCUCR = bit (BODS); 
  
  // We are guaranteed that the sleep_cpu call will be done
  // as the processor executes the next instruction after
  // interrupts are turned on.
  sleep_cpu ();   // one cycle
  SRXEPowerUp();
} /* SRXESleep() */

//
// Initialize SPI using direct register access
//
void SPI_Init(void)
{
  uint8_t temp;
  // Initialize SPI
  // Set SS to high so a connected chip will be "deselected" by default
//  digitalWrite(SS, HIGH);
  mydigitalWrite(0xb0, HIGH);
  
  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).
//  pinMode(SS, OUTPUT);
  mypinMode(0xb0, OUTPUT);

  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/16 rate
  SPCR = (1<<SPE)|(1<<MSTR) | 1;
  temp=SPSR; // clear old data
  temp=SPDR;
  if (temp != 0) {}; // suppress compiler warning
  // Set SCK as output
  //pinMode(13, OUTPUT);
  mypinMode(0xb1, OUTPUT);
  // set MOSI as output
  //pinMode(11, OUTPUT);
  mypinMode(0xb2, OUTPUT);

} /* SPI_Init() */

uint8_t SPI_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait for the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
} /* SPI_transfer() */

// Sets the D/C pin to data or command mode
static void SRXESetMode(int iMode)
{
  mydigitalWrite(iDCPin, (iMode == MODE_DATA));
} /* SRXESetMode() */
// Write a block of pixel data to the LCD
// Length can be anything from 1 to 504 (whole display)
void SRXEWriteDataBlock(unsigned char *ucBuf, int iLen)
{
  int i;
  
  mydigitalWrite(iCSPin, LOW);
  for (i=0; i<iLen; i++)
    SPI_transfer(ucBuf[i]);
  mydigitalWrite(iCSPin, HIGH);
}
//
// Command sequence to power ip the LCD controller
//
void SRXEPowerUp(void)
{
uint8_t ucTemp[4];
const char *pList = powerup;
uint8_t val, count, len = 1;

   while (len != 0)
   {
      len = pgm_read_byte(pList++);
      if (len == 99) // delay
      {
         val = pgm_read_byte(pList++);
         delay(val);
      }
      else if (len != 0) // send command with optional data
      {
         val = pgm_read_byte(pList++); // command
         SRXEWriteCommand(val);
         count = len-1;
         if (count != 0)
         {
            memcpy_P(ucTemp, pList, count);
            pList += count;
            SRXEWriteDataBlock(ucTemp, count);
         }
      }
   }
} /* SRXEPowerUp() */
//
// Initializes the LCD controller 
// Parameters: GPIO pin numbers used for the CS/DC/RST control lines
//

int SRXEInit()
//int SRXEInit(int iCS, int iDC, int iReset)
{
byte uc, ucTemp[8];

  iCSPin = 0xe7; //iCS;
  iDCPin = 0xd6; //iDC;
  iResetPin = 0xa2; //iReset;

  SPI_Init();
  mypinMode(iCSPin,OUTPUT);
  mypinMode(CS_FLASH, OUTPUT); // in case we want to use the SPI flash
  mydigitalWrite(iCSPin, HIGH);
  
  mypinMode(iDCPin, OUTPUT);
  mypinMode(iResetPin, OUTPUT);

  // Start by reseting the LCD controller
  mydigitalWrite(iResetPin, HIGH);
  delay(50);
  mydigitalWrite(iResetPin, LOW);
  delay(5);
  mydigitalWrite(iResetPin, HIGH); // take it out of reset
  delay(150); // datasheet says it must be at least 120ms

//  mydigitalWrite(iCSPin, LOW); // leave CS low forever

  SRXEPowerUp(); // turn on and initialize the display
  
  SRXEFill(0); // erase memory (it's already cleared by resetting it)
  return 0;

} /* SRXEInit() */

//
// Turn off the LCD display (lowest power mode)
//
void SRXEPowerDown()
{
    SRXEFill(0); // fill memory with zeros to go to lowest power mode
    SRXEWriteCommand(0x28); // Display OFF
    SRXEWriteCommand(0x10); // Sleep in
} /* SRXEPowerDown() */
//
// Write a 1 byte command to the LCD controller
//
static void SRXEWriteCommand(unsigned char c)
{
  mydigitalWrite(iCSPin, LOW);
  SRXESetMode(MODE_COMMAND);
  SPI_transfer(c);
  SRXESetMode(MODE_DATA);
  mydigitalWrite(iCSPin, HIGH);
} /* SRXEWriteCommand() */
//
// Send commands to position the "cursor" to the given
// row and column
//
void SRXESetPosition(int x, int y, int cx, int cy)
{
byte ucTemp[4];

  if (x > 383 || y > 159 || cx > 384 || cy > 160)
     return; // invalid
  SRXEWriteCommand(0x2a); // set column address
  ucTemp[0] = 0; // start column high byte
  ucTemp[1] = x/3; // start column low byte
  ucTemp[2] = 0; // end col high byte
  ucTemp[3] = (x+cx-1)/3; // end col low byte
  SRXEWriteDataBlock(ucTemp, 4);
  SRXEWriteCommand(0x2b); // set row address
  ucTemp[0] = 0; // start row high byte
  ucTemp[1] = y; // start row low byte
  ucTemp[2] = 0; // end row high byte
  ucTemp[3] = y+cy-1; // end row low byte
  SRXEWriteDataBlock(ucTemp, 4);
  SRXEWriteCommand(0x2c); // write RAM
} /* SRXESetPosition() */

//
// Scroll the screen N lines vertically (positive or negative)
// The value given represents a delta which affects the current scroll offset
//
void SRXEScroll(int iLines)
{
byte b;

     iScrollOffset = (iScrollOffset + iLines) % LCD_HEIGHT;
     SRXEWriteCommand(0x37); // set scroll start line
     b = (byte)iScrollOffset;
     SRXEWriteDataBlock(&b, 1);
} /* SRXEScroll() */

//
// Reset the scroll position to 0
//
void SRXEScrollReset(void)
{
byte b;
        iScrollOffset = 0;
        SRXEWriteCommand(0x37); // scroll start address
        b = 0;
        SRXEWriteDataBlock(&b, 1);
} /* SRXEcdScrollReset() */



void SRXEHorizontalLine(int x, int y, int length,byte color,int thickness){
      byte bTemp[128];

      SRXESetPosition(x*3,y,length*3,thickness);
      memset(bTemp, bColorToByte[color], length);
      for(int i = 0; i< thickness ; i++){
        SRXEWriteDataBlock(bTemp, length);
      }


}



void SRXEVerticalLine(int x, int y, int height,byte color){
      byte bTemp[128];

      SRXESetPosition(x*3,y,1, height );
      memset(bTemp, bColorToByte[color], height);
      SRXEWriteDataBlock(bTemp, height);
}



//
// Draw an outline or filled rectangle
// Only draws on byte boundaries (3 pixels wide)
// (display is treated as 128x136)
//

void SRXERectangle(int x, int y, int cx, int cy, byte color, byte bFilled)
{
byte bTemp[128];

   if (x < 0 || x > 127 || y < 0 || y > 135) return;
   if (x+cx > 127 || y+cy > 135) return;
   if (bFilled)
   {
      SRXESetPosition(x*3, y, cx*3, cy);
      for (y=0; y<cy; y++)
      {
         memset(bTemp, bColorToByte[color], cx);
         SRXEWriteDataBlock(bTemp, cx);       
      }
   } // filled
   else // outline
   {
      // Draw top part
      SRXESetPosition(x*3, y, cx*3, 1);
      memset(bTemp, bColorToByte[color], cx);
      SRXEWriteDataBlock(bTemp, cx);
      // Bottom
      SRXESetPosition(x*3, y+cy-1, cx*3, 1);
      memset(bTemp, bColorToByte[color], cx);
      SRXEWriteDataBlock(bTemp, cx);
      // Left
      SRXESetPosition(x*3, y, 3, cy);
      memset(bTemp, bColorToByte[color], cy);
      SRXEWriteDataBlock(bTemp, cy);
      // Right
      SRXESetPosition((x+cx-1)*3, y, 3, cy);
      memset(bTemp, bColorToByte[color], cy);
      SRXEWriteDataBlock(bTemp, cy);
   }
} /* SRXERectangle() */
//
// Draw a string of normal (8x8), small (6x8) or large (16x24) characters
// At the given col+row
//

//int SRXEWriteString(int x, int y, char *szMsg, int iSize, int iFGColor, int iBGColor)
int SRXEWriteString(int x, int y, char *szMsg,int iSize,const unsigned char ucFont[], int iFGColor, int iBGColor) //small font 576 bytes
{
int i, j, iLen;
unsigned char ucTemp[8], *s;
byte fgColor0, fgColor1, fgColor2, bgColor;
if (setInverse == 1) { iFGColor = map(iFGColor,0,3,3,0); } 
    if (iFGColor > 3) iFGColor = 3;
    if (iBGColor > 3) iBGColor = 3;
    if (iFGColor == 3)
    {
      fgColor0 = 0xe0; fgColor1 = 0x1c; fgColor2 = 0x3;
    }
    else
    {
    fgColor0 = (byte)iFGColor << 6; // first pixel 3-bit version of the color
    fgColor1 = (byte)iFGColor << 3; // second pixel
    fgColor2 = (byte)iFGColor;      // 3rd pixel
    }
    bgColor = bColorToByte[iBGColor];
        
    iLen = strlen(szMsg);
    if (iSize == FONT_LARGE || iSize == FONT_MEDIUM) // draw 12x16 or 15x16 font
  {
  int iWidth, iDelta;
    iWidth = (iSize == FONT_LARGE) ? 15 : 12;
    iDelta = (iSize == FONT_LARGE) ? 5 : 4;
    if ((iWidth*iLen) + x > 384) iLen = (384 - x)/iWidth; // can't display it all
    if (iLen < 0)return -1;
    for (i=0; i<iLen; i++)
    {
      int tx, ty;
      byte bTemp[84], bMask, bOut, bOut2, *d;
      if (iSize == FONT_LARGE)
      {
         s = (unsigned char *)&ucFont[((unsigned char)szMsg[i]-32) * 8];
         memcpy_P(ucTemp, s, 8); // copy from FLASH memory
      }
      else
      {
         //s = (unsigned char *)&ucSmallFont[((unsigned char)szMsg[i]-32) * 6];
		 s = (unsigned char *)&ucFont[((unsigned char)szMsg[i]-32) * 6];	
         memcpy_P(ucTemp, s, 6);
      }
       // convert from 1-bpp to 2/3-bpp
      d = bTemp;
      s = ucTemp;
      bMask = 1;
      for (ty=0; ty<8; ty++)
      {
        for (tx=0; tx<iWidth-6; tx+=3) //3) // 3 sets of 3 pixels
        {
           bOut = bOut2 = bgColor;
           if (s[tx] & bMask)
           {
              bOut &= 0x3; // clear top 6 bits
              bOut |= fgColor0 | fgColor1; // first 2 pixels (6 bits)
           }
           if (s[tx+1] & bMask)
           {
              bOut &= 0xfc; // clear bottom 2 bits
              bOut2 &= 0x1f; // clear top 3 bits
              bOut |= fgColor2; // third pixel (2 bits)
              bOut2 |= fgColor0;
           }
           if (s[tx+2] & bMask)
           {
              bOut2 &= 0xe0; // clear lower 5 bits
              bOut2 |= fgColor1 | fgColor2; // 2nd & 3rd pixel2 of second byte
           }
           d[0] = d[iDelta] = bOut;
           if (tx != 6)
                d[1] = d[iDelta+1] = bOut2;
           d += 2;
        } // for tx
        d += 4; // skip extra line (add 4 since we incremented by 6 already)
        bMask <<= 1;
      } // for ty
      SRXESetPosition(x, y, iWidth, 16);
      SRXEWriteDataBlock(bTemp, 16*iDelta); // write character pattern
      x += iWidth;
    } // for each character
  } // large+medium
  else if (iSize == FONT_NORMAL)// draw 8x8 font
   {
    if ((9*iLen) + x > 384) iLen = (384 - x)/9; // can't display it all
    if (iLen < 0)return -1;

    for (i=0; i<iLen; i++)
    {
      int tx, ty;
      byte bTemp[24], bMask, bOut, *d;
      s = (unsigned char *)&ucFont[((unsigned char)szMsg[i]-32) * 8];
      memcpy_P(ucTemp, s, 8); // copy from FLASH memory
       // convert from 1-bpp to 2/3-bpp
      d = bTemp;
      for (ty=0; ty<8; ty++)
      {
        bMask = 1<<ty;
        for (tx=0; tx<9; tx+=3) //3) // 3 sets of 3 pixels
        {
           bOut = bgColor;
           if (ucTemp[tx] & bMask)
           {
              bOut &= 0x1f; // clear top 3 bits
              bOut |= fgColor0; // first pixel (3 bits)
           }
           if (ucTemp[tx+1] & bMask)
           {
              bOut &= 0xe3; // clear middle 3 bits
              bOut |= fgColor1; // second pixel (3 bits)
           }
           if (tx != 6 &&
               ucTemp[tx+2] & bMask)
           {
              bOut &= 0xfc; // clear lower 2 bits
              bOut |= fgColor2; // third pixel (2 bits)
           }
           *d++ = bOut;
        } // for tx
      } // for ty
      SRXESetPosition(x, y, 9, 8);
      x += 9;
      SRXEWriteDataBlock(bTemp, 24); // write character pattern
    } 
   } // normal
   else // 6x8
   {
    if ((6*iLen) + x > 384) iLen = (384 - x)/6; // can't display it all
    if (iLen < 0)return -1;

    for (i=0; i<iLen; i++)
    {
      int tx, ty;
      byte bTemp[16], bMask, bOut, *d;
      //s = (unsigned char *)&ucSmallFont[((unsigned char)szMsg[i]-32) * 6];
      s = (unsigned char *)&ucFont[((unsigned char)szMsg[i]-32) * 6];
      memcpy_P(ucTemp, s, 6); // copy from FLASH memory
       // convert from 1-bpp to 2/3-bpp
      d = bTemp;
      for (ty=0; ty<8; ty++)
      {
        bMask = 1<<ty;
        for (tx=0; tx<6; tx+=3) //3) // 2 sets of 3 pixels
        {
           bOut = bgColor;
           if (ucTemp[tx] & bMask)
           {
              bOut &= 0x1f; // clear top 3 bits
              bOut |= fgColor0; // first pixel (3 bits)
           }
           if (ucTemp[tx+1] & bMask)
           {
              bOut &= 0xe3; // clear middle 3 bits
              bOut |= fgColor1; // second pixel (3 bits)
           }
           if (ucTemp[tx+2] & bMask)
           {
              bOut &= 0xfc; // clear lower 2 bits
              bOut |= fgColor2; // third pixel (2 bits)
           }
           *d++ = bOut;
        } // for tx
      } // for ty
      SRXESetPosition(x, y, 6, 8);
      x += 6;
      SRXEWriteDataBlock(bTemp, 16); // write character pattern
    } 
   } // small
  return 0;
} /* SRXEWriteString() */

// Fill the frame buffer with a byte pattern
// e.g. all off (0x00) or all on (0xff)
void SRXEFill(byte ucData)
{
  if (setInverse == 1) {ucData = map(ucData,0,255,255,0);}
int y;
byte temp[128];

     SRXESetPosition(0, 0, 384, 136);
     for (y=0; y<136; y++)
     {
          memset(temp, ucData, 128); // have to do this because the bytes get overwritten
          SRXEWriteDataBlock(temp, 128); // fill with data byte
     }
} /* SRXEFill() */
//
// External SPI flash functions (MX25L1005C - Macronix 1Mb/128KB serial flash memory)
// Data sheet here: http://www1.futureelectronics.com/doc/MACRONIX/MX25L1005CZUI-12GTR.pdf
// The CS line must be set low before each command and then set high after
// If you try to send 2 commands with the CS line left low, they won't execute
//
// ------------------
//
// Erase a 4k sector
// This is the smallest area that can be erased
// It can take around 60ms
// This function optionally waits until it completes
// returns 1 for success, 0 for failure
//
int SRXEFlashEraseSector(uint32_t ulAddr, int bWait)
{
  uint8_t rc;
  int timeout;
  if (ulAddr & 4095L) // invalid address
    return 0;
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x05); // read status register
  rc = SPI_transfer(0);
  mydigitalWrite(CS_FLASH, HIGH);
  if (rc & 1) // indicates the chip is busy in a write operation
  {
    return 0; // fail
  }
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x06); // WREN - Write enable
  mydigitalWrite(CS_FLASH, HIGH);
  
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x20); // Sector Erase
  // send 3-byte address (big-endian order)
  SPI_transfer((uint8_t)(ulAddr >> 16)); // AD1
  SPI_transfer((uint8_t)(ulAddr >> 8)); // AD2
  SPI_transfer((uint8_t)ulAddr); // AD3
  mydigitalWrite(CS_FLASH, HIGH);
  
  // wait for the erase to complete
  if (bWait)
  {
    rc = 1;
    timeout = 0;
    mydigitalWrite(CS_FLASH, LOW);
    while (rc & 1)
    {
      SPI_transfer(0x05); // read status register
      rc = SPI_transfer(0);
      delay(1);
      timeout++;
      if (timeout >= 120) // took too long, bail out
      {
        mydigitalWrite(CS_FLASH, HIGH);
        return 0;
      }
    }
    mydigitalWrite(CS_FLASH, HIGH);
  } // if asked to wait
  return 1;
} /* SRXEFlashEraseSector() */

//
// Write a 256-byte flash page
// Address must be on a page boundary
// returns 1 for success, 0 for failure
//
int SRXEFlashWritePage(uint32_t ulAddr, uint8_t *pSrc)
{
  int i, timeout;
  uint8_t rc;
   if (ulAddr & 255L) // invalid address
     return 0;
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x05); // read status register
  rc = SPI_transfer(0);
  mydigitalWrite(CS_FLASH, HIGH);
  if (rc & 1) // indicates the chip is busy in a write operation
  {
    return 0; // fail
  }
  // Disable write protect by clearing the status bits
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x01); // WRSR - write status register
  SPI_transfer(0x00); // set all bits to 0
  mydigitalWrite(CS_FLASH, HIGH);

  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x06); // WREN - Write enable
  mydigitalWrite(CS_FLASH, HIGH);
  
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x02); // PP - page program
  // send 3-byte address (big-endian order)
  SPI_transfer((uint8_t)(ulAddr >> 16)); // AD1
  SPI_transfer((uint8_t)(ulAddr >> 8)); // AD2
  SPI_transfer((uint8_t)ulAddr); // AD3
  for (i=0; i<256; i++)
  {
    SPI_transfer(pSrc[i]); // write the 256 data bytes
  }
  mydigitalWrite(CS_FLASH, HIGH); // this executes the command internally
  // wait for the write to complete
  rc = 1;
  timeout = 0;
  mydigitalWrite(CS_FLASH, LOW);
  while (rc & 1)
  {
    SPI_transfer(0x05); // read status register
    rc = SPI_transfer(0);
    delay(1);
    timeout++;
    if (timeout >= 20) // took too long, bail out
    {
      mydigitalWrite(CS_FLASH, HIGH);
      return 0;
    }
  }
  mydigitalWrite(CS_FLASH, HIGH);
  return 1;
} /* SRXEFlashWritePage() */
//
// Read N bytes from SPI flash
//
int SRXEFlashRead(uint32_t ulAddr, uint8_t *pDest, int iLen)
{
  int i;
  
  mydigitalWrite(CS_FLASH, LOW);
  SPI_transfer(0x03); // issue read instruction
  // send 3-byte address (big-endian order)
  SPI_transfer((uint8_t)(ulAddr >> 16)); // AD1
  SPI_transfer((uint8_t)(ulAddr >> 8)); // AD2
  SPI_transfer((uint8_t)ulAddr); // AD3
  for (i=0; i<iLen; i++) // read the bytes out
  {
    *pDest++ = SPI_transfer(0); // need to write something to read from SPI
  }
  mydigitalWrite(CS_FLASH, HIGH); // de-activate
  return 1;
} /* SRXEFlashRead() */

//
// Scan the rows and columns and store the results in the key map
//
void SRXEScanKeyboard(void)
{
byte r, c;

  for (r=0; r<ROWS; r++)
  {
    mypinMode(rowPins[r], INPUT_PULLUP);
  }
  // save current keymap to compare for pressed/released keys
  memcpy(bOldKeyMap, bKeyMap, sizeof(bKeyMap));
  
  for (c=0; c<COLS; c++)
  {
     bKeyMap[c] = 0;
     mypinMode(colPins[c], OUTPUT);
     mydigitalWrite(colPins[c], LOW); // test this column
     for (r=0; r<ROWS; r++)
     {
        if (mydigitalRead(rowPins[r]) == LOW)
           bKeyMap[c] |= (1 << r); // set a bit for each pressed key
     } // for r
     mydigitalWrite(colPins[c], HIGH); // leave pin in high impedance state
     mypinMode(colPins[c], INPUT);
  } // for c
} /* SRXEScanKeyboard() */
//
// Return a pointer to the internal key map
// (10 bytes with 6 bits each)
//
byte *SRXEGetKeyMap(void)
{
   return bKeyMap;
}
//
// Return the current key pressed
// includes code to provide shift + sym adjusted output
//
byte SRXEGetKey(void)
{
byte bShift, bSym, *pKeys;
byte iCol, iRow;
byte bMask;
byte bKey = 0;

   SRXEScanKeyboard();
   bShift =  bKeyMap[0] & 0x08;
   bSym = bKeyMap[0] & 0x10;
   for (iCol = 0; iCol < COLS; iCol++)
   {
       bMask = 1;
       for (iRow=0; iRow < ROWS; iRow++, bMask <<= 1)
       {
          if ((bKeyMap[iCol] & bMask) == bMask && (bOldKeyMap[iCol] & bMask) == 0)
          {
             // make sure it's not shift/sym
             if (iCol == 0 && (iRow == 3 || iRow == 4)) // shift/sym, ignore
                continue;
             // valid key, adjust it and return
             pKeys = OriginalKeys;
             if (bShift) pKeys = ShiftedKeys;
             else if (bSym) pKeys = SymKeys;
             bKey = pKeys[(iRow*COLS)+iCol];
          }
       } // for iRow
   } // for iCol
   return bKey; // 0 if no keys pressed
} /* SRXEGetKey() */

//Reverse Color order if Inverse is set true
int SRXESetInverse(byte Inverse) {
  if (setInverse == 0 && Inverse == 1) {
  int BUFFERSIZE = 4;
  for (int i=0, j = BUFFERSIZE-1; i< BUFFERSIZE/2; i++, j--)
  {
    int temp = bColorToByte[i];
    bColorToByte[i] = bColorToByte[j];
    bColorToByte[j] = temp;
  }
  setInverse = Inverse;
  }
  if (setInverse == 1 && Inverse == 1) { 
    //do nothing
  }
  if (setInverse == 1 && Inverse == 0) {
   int BUFFERSIZE = 4;
  for (int i=0, j = BUFFERSIZE-1; i< BUFFERSIZE/2; i++, j--)
  {
    int temp = bColorToByte[i];
    bColorToByte[i] = bColorToByte[j];
    bColorToByte[j] = temp;
  }
    setInverse = Inverse;
  }
  if (setInverse == 0 && Inverse == 0) {
    //do nothing
  }
}


void SRXEClearScreen() {
  SRXEFill(0x00); //clear screen
}

//
// SMART Response XE support library
// written by Larry Bank
// Copyright (C) 2018 BitBank Software, Inc.
// Project started 8/4/2018
//

/*
Added SRXEdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color )
Added SRXEwriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
Added SRXEdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
LeRoy Miller, kd8bxp April 2021
Brutally lifted from Adafruit GFX library and modified to work with Smart Response XE 
*/

#ifndef __SMART_RESPONSE_XE__
#define __SMART_RESPONSE_XE__

// Font sizes (9x8, 6x8, 12x16, 15x16)
#define FONT_NORMAL 0
#define FONT_SMALL 1
#define FONT_MEDIUM 2
#define FONT_LARGE 3

// Keyboard info
#define ROWS 6
#define COLS 10

// Display info
#define LCD_WIDTH 384
#define LCD_HEIGHT 136

// keyboard special keys
#define KEY_MENU 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_UP 4
#define KEY_DOWN 5

//
// Simplified pin numbering scheme uses a hex number to specify the port number
// and bit. Top 4 bits = port (B/D/E/F/G), bottom 3 bits specify the bit of the port
// e.g. 0xB4 = PORTB, bit 4, 0Ax is for port G
//
void mypinMode(uint8_t pin, uint8_t mode);
void mydigitalWrite(uint8_t pin, uint8_t value);
uint8_t mydigitalRead(uint8_t pin);

//
// Power Sleep (low power mode to save battery)
// Wakes up when the "power" button is pressed
//
void SRXESleep(void);

//
// Power on the LCD
//
void SRXEPowerUp(void);

//
// Power off the LCD
//
void SRXEPowerDown(void);
//
// Initializes the LCD controller
// Parameters: GPIO pin numbers used for the CS/DC/RST control lines
//

int SRXEInit();
//int SRXEInit(int iCS, int iDC, int iReset);
//
// Send commands to position the "cursor" to the given
// row and column and width and height of the memory window
//
void SRXESetPosition(int x, int y, int cx, int cy);
//
// Write a block of pixel data to the LCD
// Length can be anything from 1 to 17408 (whole display)
//
void SRXEWriteDataBlock(unsigned char *ucBuf, int iLen);

void SRXELoadBitmapRLE(int x, int y, const uint8_t *btmp);

//
//  Set Scroll Area
// inputs:
// TA: top fixed area
// SA: scroll area
// BA: bottom fixed area
// TA + SA + BA = 160
//
//  fdufnews 12/2019
//
void SRXEScrollArea(int TA, int SA, int BA);
//
// Scroll the screen N lines vertically (positive or negative)
// The value given represents a delta which affects the current scroll offset
//
void SRXEScroll(int iLines);
//
// Reset the scroll position to 0
//
void SRXEScrollReset(void);

void SRXEHorizontalLine(int x, int y, int length,byte color,int thickness);
void SRXEVerticalLine(int x, int y, int height,byte color);

//
// Draw an outline or filled rectangle
// Only draws on byte boundaries (3 pixels wide)
// (display is treated as 128x136)
//
void SRXERectangle(int x, int y, int cx, int cy, byte color, byte bFilled);
//
// Draw a string of normal (9x8), small (6x8) or large (15x16) characters
// At the given col+row
//
//int SRXEWriteString(int x, int y, char *szMsg, int iSize, int iFGColor, int iBGColor);
int SRXEWriteString(int x, int y, char *szMsg,int iSize, const unsigned char ucFont[], int iFGColor, int iBGColor); //small font 576 bytes

// Fill the frame buffer with a byte pattern
// e.g. all off (0x00) or all on (0xff)
void SRXEFill(byte ucData);
//
// Scan the rows and columns and store the results in the key map
//
void SRXEScanKeyboard(void);
//
// Return a pointer to the internal key map
// (10 bytes with 6 bits each)
//
byte *SRXEGetKeyMap(void);
//
// Return the current key pressed
// includes code to provide shift + sym adjusted output
// internally calls SRXEScanKeyboard()
//
byte SRXEGetKey(void);

//
// Erase a 4k sector
// This is the smallest area that can be erased
// It can take around 60ms
// This function optionally waits until it completes
// returns 1 for success, 0 for failure
//
int SRXEFlashEraseSector(uint32_t ulAddr, int bWait);
//
// Write a 256-byte flash page
// Address must be on a page boundary
// returns 1 for success, 0 for failure
//
int SRXEFlashWritePage(uint32_t ulAddr, uint8_t *pSrc);
//
// Read N bytes from SPI flash
//
int SRXEFlashRead(uint32_t ulAddr, uint8_t *pDest, int iLen);


int SRXESetInverse(byte Inverse);
void SRXEClearScreen();

// Brutally lifted from Adafruit GFX library and modified to almost work with Smart Response XE - LeRoy Miller, kd8bxp April 2021

/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 0 - off, to 3 (full on)
*/
void SRXEdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color );

/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 0 - off, to 3 (full on)
*/
void SRXEwriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

/*!
   @brief   Draw a triangle with no fill color
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 0 - off, to 3 (full on)
*/
void SRXEdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

//end lifted from Adafruit GFX - LeRoy Miller April 2021

#endif // __SMART_RESPONSE_XE__

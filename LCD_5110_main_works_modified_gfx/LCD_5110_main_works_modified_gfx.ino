//
//  LCD_5110_main.ino
//  Sketch
//  ----------------------------------
//  Developed with embedXcode
//
//  Project LCD BoosterPack for MSP430G2553 and StallarPad LM4F
//  Created by Rei VILO on 28/05/12
//  Copyright (c) 2012 http://embeddedcomputing.weebly.com
//  Licence CC = BY SA NC
//
//  Ported to LM4F120 by Bryan Schremp (bajabug@gmail.com) 11/10/2012
//


// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"   
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Board not supported
#endif

#define CS 18  //P3_0
#define CLK 7 //P1_5
#define Din 15 //P1_6
#define DC 17  //P5_7
#define RST 13  //P5_0
#define BCKLIT 19 //P2_5
 
// Include application, user and local libraries
#include "LCD_5110.h"

// Variables
LCD_5110 myScreen(CS,CLK,Din,DC,RST,BCKLIT,PUSH2);
boolean	backlight = false;
uint8_t k = 0;

void _write(uint8_t dataCommand, uint8_t c) {
  digitalWrite(DC, dataCommand);
  digitalWrite(CS, LOW);
  shiftOut(Din, CLK, MSBFIRST, c);
  digitalWrite(CS, HIGH);
}

// Brutally lifted from Adafruit GFX library and modified to almost work with Smart Response XE - LeRoy Miller, kd8bxp April 2021

#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }

void setPosition(uint8_t x, uint8_t y, bool on) {
  _write(0x00, 0x40 | y);
  _write(0x00, 0x80 | x);
  int hex;
  if (on == true) { hex = 0x43; } else if (on == false) { hex = 0x00;}
  _write(0x01,  on);
  //_write(0x01, 0x00);
}

/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 0 - off, to 3 (full on)
*/
void drawCircle(int16_t x0, int16_t y0, int16_t r, bool on) {
 int16_t f = 1  - r; //int16_t f = 1 - r;
  int16_t ddF_x = 1; //int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r; //int16_t ddF_y = -2 * r;
  int16_t x = 0; //int16_t x = 0;
  int16_t y = r; //int16_t y = r;
  
  setPosition(x0, y0 + r, on);
     
  setPosition(x0, y0 - r, on);
        
  setPosition(x0 + r, y0, on);
      
  setPosition(x0 - r, y0, on);
    
  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2; //ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2; //ddF_x += 2;
    f += ddF_x;

    //writePixel(x0 + x, y0 + y, color);
    setPosition(x0 + x , (y0 + y), on); //lower right arch
      
    //writePixel(x0 - x, y0 + y, color);
    setPosition(x0 - x, y0 + y, on);
   
    //writePixel(x0 + x, y0 - y, color);
    setPosition(x0 + x, y0 - y, on );
        
    //writePixel(x0 - x, y0 - y, color);
    setPosition(x0 - x, y0 - y, on);
        
    //writePixel(x0 + y, y0 + x, color);
    setPosition(x0 + y, y0 + x, on);
        
    //writePixel(x0 - y, y0 + x, color);
    setPosition(x0 - y, y0 + x, on);
        
    //writePixel(x0 + y, y0 - x, color);
    setPosition(x0 + y, y0 - x, on);
       
    //writePixel(x0 - y, y0 - x, color);
    setPosition(x0 - y, y0 - x, on);
        
  }  
}

/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 0 - off, to 3 (full on)
*/
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool on) {
 
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      
      //writePixel(y0, x0, color);
      setPosition(y0, x0, on);
            
    } else {
      
      //writePixel(x0, y0, color);
      setPosition(x0, y0, on);
            
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

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
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool on) {
  writeLine(x0, y0, x1, y1, on);
  writeLine(x1, y1, x2, y2, on);
  writeLine(x2, y2, x0, y0, on);
}

//end lifted from Adafruit GFX - LeRoy Miller April 2021

// Add setup code
void setup() {
    myScreen.begin();
    
    myScreen.setBacklight(backlight);
 
   //set contrast to DEC 60 (Max 127 0x7f)
    _write(0x00, 0x20 | 0x01);
    _write(0x00, 0x80 | 0x3C);
    _write(0x00, 0x20);

   //set bias max 7
   _write(0x00, 0x20 | 0x01);
   _write(0x00, 0x10 | 0x07);
   _write(0x00, 0x20);
    
   myScreen.clear();
   
}


// Add loop code
void loop() {
myScreen.setFont(1);
myScreen.text(1, 1, "Shape");
delay(5000);
//myScreen.clear();
setPosition(0,0, 1);
	  writeLine(0, 0, 0, 100, 1);
    drawCircle(40,40, 40, 1);
    drawTriangle(100, 50, 50, 30, 30, 100, 1);  

delay(6000);
setPosition(0,0, 1);
	  writeLine(0,0, 0, 100, 0);
    drawCircle(40,40, 40, 0);
    drawTriangle(100, 50, 50, 30, 30, 100, 0); 
delay(6000);

}

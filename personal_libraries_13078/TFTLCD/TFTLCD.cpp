#include &quot;TFTLCD.h&quot;



#ifdef USE_ADAFRUIT_SHIELD_PINOUT
// special defines for the dataport
 #define DATAPORT1 PORTD
 #define DATAPIN1 PIND
 #define DATADDR1 DDRD

 #define DATAPORT2 PORTB
 #define DATAPIN2 PINB
 #define DATADDR2 DDRB

 #define DATA1_MASK 0xD0
 #define DATA2_MASK 0x2F

// for mega &amp; shield usage, we just hardcoded it (its messy)

#else
 // for the breakout board tutorial, two ports are used :/
 #define DATAPORT1 PORTD
 #define DATAPIN1  PIND
 #define DATADDR1  DDRD

 #define DATAPORT2 PORTB
 #define DATAPIN2  PINB
 #define DATADDR2  DDRB

 #define DATA1_MASK 0xFC  // top 6 bits
 #define DATA2_MASK 0x03  // bottom 2 bits


 #define MEGA_DATAPORT PORTA
 #define MEGA_DATAPIN  PINA
 #define MEGA_DATADDR  DDRA
#endif


#include &quot;glcdfont.c&quot;
#include &lt;avr/pgmspace.h&gt;
#include &quot;pins_arduino.h&quot;
#include &quot;wiring_private.h&quot;


void TFTLCD::goHome(void) {
  goTo(0,0);
}

uint16_t TFTLCD::width(void) {
  return _width;
}
uint16_t TFTLCD::height(void) {
  return _height;
}

void TFTLCD::goTo(int x, int y) {
  writeRegister(0x0020, x);     // GRAM Address Set (Horizontal Address) (R20h)
  writeRegister(0x0021, y);     // GRAM Address Set (Vertical Address) (R21h)
  writeCommand(0x0022);            // Write Data to GRAM (R22h)
}

void TFTLCD::setCursor(uint16_t x, uint16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void TFTLCD::setTextSize(uint8_t s) {
  textsize = s;
}

void TFTLCD::setTextColor(uint16_t c) {
  textcolor = c;
}

size_t TFTLCD::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textsize);
    cursor_x += textsize*6;
  }
}

void TFTLCD::drawString(uint16_t x, uint16_t y, char *c, 
			uint16_t color, uint8_t size) {
  while (c[0] != 0) {
    drawChar(x, y, c[0], color, size);
    x += size*6;
    c++;
  }
}
// draw a character
void TFTLCD::drawChar(uint16_t x, uint16_t y, char c, 
		      uint16_t color, uint8_t size) {
  for (uint8_t i =0; i&lt;5; i++ ) {
    uint8_t line = pgm_read_byte(font+(c*5)+i);
    for (uint8_t j = 0; j&lt;8; j++) {
      if (line &amp; 0x1) {
	if (size == 1) // default size
	  drawPixel(x+i, y+j, color);
	else {  // big size
	  fillRect(x+i*size, y+j*size, size, size, color);
	} 
      }
      line &gt;&gt;= 1;
    }
  }
}

// draw a triangle!
void TFTLCD::drawTriangle(uint16_t x0, uint16_t y0,
			  uint16_t x1, uint16_t y1,
			  uint16_t x2, uint16_t y2, uint16_t color)
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color); 
}

void TFTLCD::fillTriangle ( int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
  if (y0 &gt; y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 &gt; y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 &gt; y1) {
    swap(y0, y1); swap(x0, x1);
  }

  int32_t dx1, dx2, dx3; // Interpolation deltas
  int32_t sx1, sx2, sy; // Scanline co-ordinates

  sx2=(int32_t)x0 * (int32_t)1000; // Use fixed point math for x axis values
  sx1 = sx2;
  sy=y0;

  // Calculate interpolation deltas
  if (y1-y0 &gt; 0) dx1=((x1-x0)*1000)/(y1-y0);
    else dx1=0;
  if (y2-y0 &gt; 0) dx2=((x2-x0)*1000)/(y2-y0);
    else dx2=0;
  if (y2-y1 &gt; 0) dx3=((x2-x1)*1000)/(y2-y1);
    else dx3=0;

  // Render scanlines (horizontal lines are the fastest rendering method)
  if (dx1 &gt; dx2)
  {
    for(; sy&lt;=y1; sy++, sx1+=dx2, sx2+=dx1)
    {
      drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
    sx2 = x1*1000;
    sy = y1;
    for(; sy&lt;=y2; sy++, sx1+=dx2, sx2+=dx3)
    {
      drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
  }
  else
  {
    for(; sy&lt;=y1; sy++, sx1+=dx1, sx2+=dx2)
    {
      drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
    sx1 = x1*1000;
    sy = y1;
    for(; sy&lt;=y2; sy++, sx1+=dx3, sx2+=dx2)
    {
      drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
    }
  }
}

uint16_t TFTLCD::Color565(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t c;
  c = r &gt;&gt; 3;
  c &lt;&lt;= 6;
  c |= g &gt;&gt; 2;
  c &lt;&lt;= 5;
  c |= b &gt;&gt; 3;

  return c;
}

// draw a rectangle
void TFTLCD::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
		      uint16_t color) {
  // smarter version
  drawHorizontalLine(x, y, w, color);
  drawHorizontalLine(x, y+h-1, w, color);
  drawVerticalLine(x, y, h, color);
  drawVerticalLine(x+w-1, y, h, color);
}

// draw a rounded rectangle
void TFTLCD::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r,
			   uint16_t color) {
  // smarter version
  drawHorizontalLine(x+r, y, w-2*r, color);
  drawHorizontalLine(x+r, y+h-1, w-2*r, color);
  drawVerticalLine(x, y+r, h-2*r, color);
  drawVerticalLine(x+w-1, y+r, h-2*r, color);
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}


// fill a rounded rectangle
void TFTLCD::fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r,
			   uint16_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

// fill a circle
void TFTLCD::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
  writeRegister(TFTLCD_ENTRY_MOD, 0x1030);
  drawVerticalLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}


// used to do circles and roundrects!
void TFTLCD::fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta,
			uint16_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x&lt;y) {
    if (f &gt;= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    if (cornername &amp; 0x1) {
      drawVerticalLine(x0+x, y0-y, 2*y+1+delta, color);
      drawVerticalLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername &amp; 0x2) {
      drawVerticalLine(x0-x, y0-y, 2*y+1+delta, color);
      drawVerticalLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}


// draw a circle outline

void TFTLCD::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, 
			uint16_t color) {
  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  drawCircleHelper(x0, y0, r, 0xF, color);
}

void TFTLCD::drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername,
			uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;


  while (x&lt;y) {
    if (f &gt;= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername &amp; 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername &amp; 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername &amp; 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername &amp; 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

// fill a rectangle
void TFTLCD::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, 
		      uint16_t fillcolor) {
  // smarter version
  while (h--)
    drawHorizontalLine(x, y++, w, fillcolor);
}


void TFTLCD::drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
  if (x &gt;= _width) return;

  drawFastLine(x,y,length,color,1);
}

void TFTLCD::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
  if (y &gt;= _height) return;
  drawFastLine(x,y,length,color,0);
}

void TFTLCD::drawFastLine(uint16_t x, uint16_t y, uint16_t length, 
			  uint16_t color, uint8_t rotflag)
{
  uint16_t newentrymod;

  switch (rotation) {
  case 0:
    if (rotflag)
    
      newentrymod = 0x1028;   // we want a 'vertical line'1028
    else 
      newentrymod = 0x1030;   // we want a 'horizontal line'1030
    break;
  case 1:
    swap(x, y);
    // first up fix the X
    x = TFTWIDTH - x - 1;//
    if (rotflag)
      newentrymod = 0x1000;   // we want a 'vertical line'1000
    else 
      newentrymod = 0x1028;   // we want a 'horizontal line'1028
    break;
  case 2:
    x =  TFTWIDTH - x - 1;
    y =  TFTHEIGHT - y - 1;
    if (rotflag)
      newentrymod = 0x1008;   // we want a 'vertical line'1008
    else 
      newentrymod = 0x1020;   // we want a 'horizontal line'1020
    break;
  case 3:
    swap(x,y);
    y = TFTHEIGHT - y - 1;//TFTHEIGHT
    if (rotflag)
      newentrymod = 0x1030;   // we want a 'vertical line'1030
    else 
      newentrymod = 0x1008;   // we want a 'horizontal line'1008
    break;
  }
  
  
  writeRegister(0x03, newentrymod);

  writeRegister(0x20, x); // GRAM Address Set (Horizontal Address) (R20h)
  writeRegister(0x21, y); // GRAM Address Set (Vertical Address) (R21h)
  writeCommand(0x22);  // Write Data to GRAM (R22h)


  *portOutputRegister(csport) &amp;= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) |= cdpin;
  //digitalWrite(_cd, HIGH);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  *portOutputRegister(wrport) |= wrpin;
  //digitalWrite(_wr, HIGH);

  setWriteDir();
  while (length--) {
    writeData_unsafe(color); 
  }

  // set back to default
  *portOutputRegister(csport) |= cspin;
  //digitalWrite(_cs, HIGH);
  writeRegister(TFTLCD_ENTRY_MOD, 0x1028);
}



// bresenham's algorithm - thx wikpedia
void TFTLCD::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
		      uint16_t color) {
  // if you're in rotation 1 or 3, we need to swap the X and Y's

  int16_t steep = abs(y1 - y0) &gt; abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 &gt; x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  //dy = abs(y1 - y0);
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 &lt; y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0&lt;=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err &lt; 0) {
      y0 += ystep;
      err += dx;
    }
  }
}


void TFTLCD::fillScreen(uint16_t color) {
  goHome();
  uint32_t i;
  
  i = 320;
  i *= 240;
  
  *portOutputRegister(csport) &amp;= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) |= cdpin;
  //digitalWrite(_cd, HIGH);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  *portOutputRegister(wrport) |= wrpin;
  //digitalWrite(_wr, HIGH);

  setWriteDir();
  while (i--) {
    writeData_unsafe(color); 
  }

  *portOutputRegister(csport) |= cspin;
  //digitalWrite(_cs, HIGH);
}

void TFTLCD::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  // check rotation, move pixel around if necessary
  switch (rotation) {
  case 1:
    swap(x, y);
    x = TFTWIDTH - x - 1;
    break;
  case 2:
    x = TFTWIDTH - x - 1;
    y = TFTHEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = TFTHEIGHT - y - 1;
    break;
  }
    
  if ((x &gt;= TFTWIDTH) || (y &gt;= TFTHEIGHT)) return;
  writeRegister(TFTLCD_GRAM_HOR_AD, x); // GRAM Address Set (Horizontal Address) (R20h)
  writeRegister(TFTLCD_GRAM_VER_AD, y); // GRAM Address Set (Vertical Address) (R21h)
  writeCommand(TFTLCD_RW_GRAM);  // Write Data to GRAM (R22h)
  writeData(color);
}

static const uint16_t _regValues[] PROGMEM = {
  0x00e5,0x8000,
		0x0000,0x0001,

		0x0001,0x0100,
		0x0002,0x0700,
		0x0003,0x1028,
		0x0004,0x0000,
		0x0008,0x0202,
		0x0009,0x0000,
		0x000a,0x0000,	
		0x000c,0x0000,
		0x000d,0x0000,
		0x000f,0x0000,
//*********************************************Power On
		0x0010,0x0000,
		0x0011,0x0000,
		0x0012,0x0000,
		0x0013,0x0000,

		0x0010,0x17b0,
		0x0011,0x0037,

		0x0012,0x0138,

		0x0013,0x1700,
		0x0029,0x000d,

		0x0020,0x0000,
		0x0021,0x0000,
//*********************************************Set gamma
		0x0030,0x0001,
		0x0031,0x0606,
		0x0032,0x0304,
		0x0033,0x0202,
		0x0034,0x0202,
		0x0035,0x0103,
		0x0036,0x011d,
		0x0037,0x0404,
		0x0038,0x0404,
		0x0039,0x0404,
		0x003c,0x0700,
		0x003d,0x0a1f,
//**********************************************Set Gram aera
		0x0050,0x0000,
		0x0051,0x00ef,
		0x0052,0x0000,
		0x0053,0x013f,
		0x0060,0x2700,
		0x0061,0x0001,
		0x006a,0x0000,
//*********************************************Paratial display
		0x0090,0x0010,
		0x0092,0x0000,
		0x0093,0x0003,
		0x0095,0x0101,
		0x0097,0x0000,
		0x0098,0x0000,
//******************************************** Plan Control
		0x0007,0x0021,

		0x0007,0x0031,

		0x0007,0x0173,
};

void TFTLCD::initDisplay(void) {
  uint16_t a, d;

  reset();
  
  for (uint8_t i = 0; i &lt; sizeof(_regValues) / 4; i++) {
    a = pgm_read_word(_regValues + i*2);
    d = pgm_read_word(_regValues + i*2 + 1);

    if (a == 0xFF) {
      delay(d);
    } else {
      writeRegister(a, d);
      //Serial.print(&quot;addr: &quot;); Serial.print(a); 
      //Serial.print(&quot; data: &quot;); Serial.println(d, HEX);
    }
  }
}

uint8_t TFTLCD::getRotation(void) {
  return rotation;
}

void TFTLCD::setRotation(uint8_t x) {
  writeRegister(0x03, 0x1028);

  x %= 4;  // cant be higher than 3
  rotation = x;
  switch (x) {
  case 0:
    _width = TFTWIDTH; 
    _height = TFTHEIGHT;
    break;
  case 1:
    _width = TFTHEIGHT; 
    _height = TFTWIDTH;
    break;
  case 2:
    _width = TFTWIDTH; 
    _height = TFTHEIGHT;
    break;
  case 3:
    _width = TFTHEIGHT; 
    _height = TFTWIDTH;
    break;
 }
}

/********************************* low level pin initialization */

TFTLCD::TFTLCD(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t reset) {
  _cs = cs;
  _cd = cd;
  _wr = wr;
  _rd = rd;
  _reset = reset;
  
  rotation = 0;
  _width = TFTWIDTH;
  _height = TFTHEIGHT;

  // disable the LCD
  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);  
  
  digitalWrite(_cd, HIGH);
  pinMode(_cd, OUTPUT);  
  
  digitalWrite(_wr, HIGH);
  pinMode(_wr, OUTPUT);  
  
  digitalWrite(_rd, HIGH);
  pinMode(_rd, OUTPUT);  

  digitalWrite(_reset, HIGH); 
  pinMode(_reset, OUTPUT); 

  csport = digitalPinToPort(_cs);
  cdport = digitalPinToPort(_cd);
  wrport = digitalPinToPort(_wr);
  rdport = digitalPinToPort(_rd);

  cspin = digitalPinToBitMask(_cs);
  cdpin = digitalPinToBitMask(_cd);
  wrpin = digitalPinToBitMask(_wr);
  rdpin = digitalPinToBitMask(_rd);

  cursor_y = cursor_x = 0;
  textsize = 1;
  textcolor = 0xFFFF;
}


/********************************** low level pin interface */

void TFTLCD::reset(void) {
  if (_reset)
    digitalWrite(_reset, LOW);
  delay(2); 
  if (_reset)
    digitalWrite(_reset, HIGH);

  // resync
  writeData(0);
  writeData(0);
  writeData(0);  
  writeData(0);
}

inline void TFTLCD::setWriteDir(void) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)
  DATADDR2 |= DATA2_MASK;
  DATADDR1 |= DATA1_MASK;
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 

  #ifdef USE_ADAFRUIT_SHIELD_PINOUT
  DDRH |= 0x78;
  DDRB |= 0xB0;
  DDRG |= _BV(5);
  #else
  MEGA_DATADDR = 0xFF;
  #endif
#else
  #error &quot;No pins defined!&quot;
#endif
}

inline void TFTLCD::setReadDir(void) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)
  DATADDR2 &amp;= ~DATA2_MASK;
  DATADDR1 &amp;= ~DATA1_MASK;
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 

  #ifdef USE_ADAFRUIT_SHIELD_PINOUT
  DDRH &amp;= ~0x78;
  DDRB &amp;= ~0xB0;
  DDRG &amp;= ~_BV(5);
  #else
  MEGA_DATADDR = 0;
  #endif
#else
  #error &quot;No pins defined!&quot;
#endif
}

inline void TFTLCD::write8(uint8_t d) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)

  DATAPORT2 = (DATAPORT2 &amp; DATA1_MASK) | 
    (d &amp; DATA2_MASK);
  DATAPORT1 = (DATAPORT1 &amp; DATA2_MASK) | 
    (d &amp; DATA1_MASK); // top 6 bits
  
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 


#ifdef USE_ADAFRUIT_SHIELD_PINOUT

  // bit 6/7 (PH3 &amp; 4)
  // first two bits 0 &amp; 1 (PH5 &amp; 6)
  PORTH &amp;= ~(0x78);
  PORTH |= ((d&amp;0xC0) &gt;&gt; 3) | ((d&amp;0x3) &lt;&lt; 5);

  // bits 2 &amp; 3 (PB4 &amp; PB5)
  // bit 5 (PB7)
  PORTB &amp;= ~(0xB0); 
  PORTB |= ((d &amp; 0x2C) &lt;&lt; 2);

  // bit 4  (PG5)
  if (d &amp; _BV(4))
    PORTG |= _BV(5);
  else
    PORTG &amp;= ~_BV(5);

  #else
     MEGA_DATAPORT = d;  
  #endif

#else
  #error &quot;No pins defined!&quot;
#endif
}

inline uint8_t TFTLCD::read8(void) {
 uint8_t d;
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)

 d = DATAPIN1 &amp; DATA1_MASK; 
 d |= DATAPIN2 &amp; DATA2_MASK; 

#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__)  || defined(__AVR_ATmega1280__) 

#ifdef USE_ADAFRUIT_SHIELD_PINOUT

  // bit 6/7 (PH3 &amp; 4)
  // first two bits 0 &amp; 1 (PH5 &amp; 6)
 d = (PINH &amp; 0x60) &gt;&gt; 5;
 d |= (PINH &amp; 0x18) &lt;&lt; 3;

  // bits 2 &amp; 3 &amp; 5 (PB4 &amp; PB5, PB7)
 d |= (PINB &amp; 0xB0) &gt;&gt; 2;

  // bit 4  (PG5)
  if (PING &amp; _BV(5))
    d |= _BV(4);

#else
 d = MEGA_DATAPIN;  
#endif

#else

  #error &quot;No pins defined!&quot;

#endif

 return d;
}

/********************************** low level readwrite interface */

// the C/D pin is high during write
void TFTLCD::writeData(uint16_t data) {
  volatile uint8_t *wrportreg = portOutputRegister(wrport);

  *portOutputRegister(csport) &amp;= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) |= cdpin;
  //digitalWrite(_cd, HIGH);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  setWriteDir();
  write8(data &gt;&gt; 8);
  
  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  write8(data);

  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  *portOutputRegister(csport) |= cspin;
  //digitalWrite(_cs, HIGH);
}

// this is a 'sped up' version, with no direction setting, or pin initialization
// not for external usage, but it does speed up stuff like a screen fill
inline void TFTLCD::writeData_unsafe(uint16_t data) {
  volatile uint8_t *wrportreg = portOutputRegister(wrport);

  write8(data &gt;&gt; 8);

  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  write8(data);

  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);
}

// the C/D pin is low during write
void TFTLCD::writeCommand(uint16_t cmd) {
  volatile uint8_t *wrportreg = portOutputRegister(wrport);

  *portOutputRegister(csport) &amp;= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) &amp;= ~cdpin;
  //digitalWrite(_cd, LOW);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  setWriteDir();
  write8(cmd &gt;&gt; 8);

  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  write8(cmd);

  *wrportreg &amp;= ~wrpin;
  //digitalWrite(_wr, LOW);
  *wrportreg |=  wrpin;
  //digitalWrite(_wr, HIGH);

  *portOutputRegister(csport) |= cspin;
}

uint16_t TFTLCD::readData() {
 uint16_t d = 0;
 
  *portOutputRegister(csport) &amp;= ~cspin;
  //digitalWrite(_cs, LOW);
  *portOutputRegister(cdport) |= cdpin;
  //digitalWrite(_cd, HIGH);
  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  *portOutputRegister(wrport) |= wrpin;
  //digitalWrite(_wr, HIGH);
  
  setReadDir();

  *portOutputRegister(rdport) &amp;= ~rdpin;
  //digitalWrite(_rd, LOW);

  delayMicroseconds(10);
  d = read8();
  d &lt;&lt;= 8;

  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  *portOutputRegister(rdport) &amp;= ~rdpin;
  //digitalWrite(_rd, LOW);

  delayMicroseconds(10);
  d |= read8();

  *portOutputRegister(rdport) |= rdpin;
  //digitalWrite(_rd, HIGH);
  
  *portOutputRegister(csport) |= cspin;
  //digitalWrite(_cs, HIGH);
   
  return d;
}


/************************************* medium level data reading/writing */

uint16_t TFTLCD::readRegister(uint16_t addr) {
   writeCommand(addr);
   return readData();
}

void TFTLCD::writeRegister(uint16_t addr, uint16_t data) {
   writeCommand(addr);
   writeData(data);
}






#ifndef _ADAFRUIT_ST7735H_
#define _ADAFRUIT_ST7735H_

#define SPARK 1 // !!!!!!!!!!! TEMPORARILY !!!!!!!!!

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#elif defined (SPARK)
#include "application.h"
#else
 #include "WProgram.h"
#endif


#if defined(SPARK)
#include "Adafruit_mfGFX.h"
#else
#include <Adafruit_GFX.h>
#endif

#if defined(SPARK)
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#if defined(__SAM3X8E__)
#include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
typedef unsigned char prog_uchar;
#endif
#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

// RST pin use define
#define NO_RST_PIN	255

// some flags for initR() :(
#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB   0x2

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0  
#define ST7735_WHITE   0xFFFF


class Adafruit_ST7735 : public Adafruit_GFX {

 public:

#if defined(SPARK) // hardware spi only
#else
  Adafruit_ST7735(uint8_t CS, uint8_t RS, uint8_t SID, uint8_t SCLK, uint8_t RST);
#endif
  //Adafruit_ST7735(uint8_t CS, uint8_t RS, uint8_t RST);
  Adafruit_ST7735(uint8_t CS, uint8_t RS, uint8_t SID, uint8_t SCLK, uint8_t RST);

  void     initB(void),                             // for ST7735B displays
           initR(uint8_t options = INITR_GREENTAB), // for ST7735R
           setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1),
           pushColor(uint16_t color),
           fillScreen(uint16_t color),
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(boolean i);
  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only!
  uint8_t  readdata(void),
           readcommand8(uint8_t);
  uint16_t readcommand16(uint8_t);
  uint32_t readcommand32(uint8_t);
  void     dummyclock(void);
  */

 private:
  uint8_t  tabcolor;

  void     spiwrite(uint8_t),
           writecommand(uint8_t c),
           writedata(uint8_t d),
           commandList(const uint8_t *addr),
           commonInit(const uint8_t *cmdList);
//uint8_t  spiread(void);

  boolean  hwSPI;

#if defined(SPARK)
  uint8_t  _cs, _rs, _rst;
  uint8_t  colstart, rowstart; // some displays need this changed
#endif

#if defined(__AVR__) || defined(CORE_TEENSY)
volatile uint8_t *dataport, *clkport, *csport, *rsport;
  uint8_t  _cs, _rs, _rst, _sid, _sclk,
           datapinmask, clkpinmask, cspinmask, rspinmask,
           colstart, rowstart; // some displays need this changed
#endif //  #ifdef __AVR__

#if defined(__SAM3X8E__)
  Pio *dataport, *clkport, *csport, *rsport;
  uint32_t  _cs, _rs, _rst, _sid, _sclk,
            datapinmask, clkpinmask, cspinmask, rspinmask,
            colstart, rowstart; // some displays need this changed
#endif //  #if defined(__SAM3X8E__)
  


};

#endif

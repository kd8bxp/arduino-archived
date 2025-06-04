/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.14" TFT breakout
  ----> https://www.adafruit.com/product/4383
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include "src/Adafruit_GFX/Adafruit_GFX.h"    // Core graphics library
//#include "src/Adafruit_ST7735/Adafruit_ST7735.h" // Hardware-specific library for ST7735
#include "src/Adafruit_ST7735/Adafruit_ST7789.h"
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

   #define TFT_CS    42     
  #define TFT_RST   39
  #define TFT_DC    41
  #define TFT_MOSI  45  // Data out
  #define TFT_SCLK  40  // Clock out
#define TFT_LED 48

#define Offset_X 34

// For ST7735-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


// OR for the ST7789-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

Adafruit_ST7789 *_tft = NULL;

float p = 3.1415926;

void setup(void) {
  Serial.begin(115200);
  Serial.print(F("Hello! ST77xx TFT Test"));
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

     SPIClass *spi = new SPIClass(HSPI);
  spi->begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  _tft = new Adafruit_ST7789(spi, TFT_CS, TFT_DC, TFT_RST);
  // 80MHz should work, but you may need lower speeds
  _tft->setSPISpeed(80000000);
 _tft->init(172+Offset_X, 320, SPI_MODE0);
 
 //_tft->initSPI( 80000000, SPI_MODE0);
 //_tft->initR(INITR_GREENTAB); //MINI160x80);      // Init ST7735S chip, green tab
 //_tft->invertDisplay(true);
 //_tft->setRotation(ST7735_MADCTL_MH);
 //_tft->init(206, 320, SPI_MODE0);
 _tft->setRotation(2);
 _tft->fillScreen(ST77XX_BLACK); // Black screen fill

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  _tft->fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  _tft->fillScreen(ST77XX_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
  delay(1000);

  // tft print function!
  tftPrintTest();
  delay(4000);

  // a single pixel
  _tft->drawPixel(_tft->width()/2, _tft->height()/2, ST77XX_GREEN);
  delay(500);

  // line draw test
  testlines(ST77XX_YELLOW);
  delay(500);

  // optimized lines
  testfastlines(ST77XX_RED, ST77XX_BLUE);
  delay(500);

  testdrawrects(ST77XX_GREEN);
  delay(500);

  testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
  delay(500);

  _tft->fillScreen(ST77XX_BLACK);
  testfillcircles(10, ST77XX_BLUE);
  testdrawcircles(10, ST77XX_WHITE);
  delay(500);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

  mediabuttons();
  delay(500);

  Serial.println("done");
  delay(1000);
}

void loop() {
  _tft->invertDisplay(true);
  delay(500);
  _tft->invertDisplay(false);
  delay(500);
}

void testlines(uint16_t color) {
  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=0+Offset_X; x < _tft->width(); x+=6) {
    _tft->drawLine(0, 0, x, _tft->height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < _tft->height(); y+=6) {
    _tft->drawLine(0, 0, _tft->width()-1, y, color);
    delay(0);
  }

  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=0+Offset_X; x < _tft->width(); x+=6) {
    _tft->drawLine(_tft->width()-1, 0, x, _tft->height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < _tft->height(); y+=6) {
    _tft->drawLine(_tft->width()-1, 0, 0, y, color);
    delay(0);
  }

  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=0+Offset_X; x < _tft->width(); x+=6) {
    _tft->drawLine(0, _tft->height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < _tft->height(); y+=6) {
    _tft->drawLine(0, _tft->height()-1, _tft->width()-1, y, color);
    delay(0);
  }

  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=0+Offset_X; x < _tft->width(); x+=6) {
    _tft->drawLine(_tft->width()-1, _tft->height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < _tft->height(); y+=6) {
    _tft->drawLine(_tft->width()-1, _tft->height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  _tft->setCursor(0, 0);
  _tft->setTextColor(color);
  _tft->setTextWrap(true);
  _tft->print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < _tft->height(); y+=5) {
    _tft->drawFastHLine(0, y, _tft->width(), color1);
  }
  for (int16_t x=0+Offset_X; x < _tft->width(); x+=5) {
    _tft->drawFastVLine(x, 0, _tft->height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < _tft->width(); x+=6) {
    _tft->drawRect(_tft->width()/2 -x/2, _tft->height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  _tft->fillScreen(ST77XX_BLACK);
  for (int16_t x=_tft->width()+Offset_X-1; x > 6; x-=6) {
    _tft->fillRect(_tft->width()/2 -x/2, _tft->height()/2 -x/2 , x, x, color1);
    _tft->drawRect(_tft->width()/2 -x/2, _tft->height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius+Offset_X; x < _tft->width(); x+=radius*2) {
    for (int16_t y=radius; y < _tft->height(); y+=radius*2) {
      _tft->fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0+Offset_X; x < _tft->width()+radius; x+=radius*2) {
    for (int16_t y=0; y < _tft->height()+radius; y+=radius*2) {
      _tft->drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  _tft->fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = _tft->width()/2;
  int x = _tft->height()-1;
  int y = 0;
  int z = _tft->width();
  for(t = 0 ; t <= 15; t++) {
    _tft->drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  _tft->fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0+Offset_X;
    int y = 0;
    int w = _tft->width()-2;
    int h = _tft->height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      _tft->drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  _tft->setTextWrap(false);
  _tft->fillScreen(ST77XX_BLACK);
  _tft->setCursor(0+Offset_X, 30);
  _tft->setTextColor(ST77XX_RED);
  _tft->setTextSize(1);
  _tft->println("Hello World!");
  _tft->setTextColor(ST77XX_YELLOW);
  _tft->setTextSize(2);
  _tft->println("Hello World!");
  _tft->setTextColor(ST77XX_GREEN);
  _tft->setTextSize(3);
  _tft->println("Hello World!");
  _tft->setTextColor(ST77XX_BLUE);
  _tft->setTextSize(4);
  _tft->print(1234.567);
  delay(1500);
  _tft->setCursor(0+Offset_X, 0);
  _tft->fillScreen(ST77XX_BLACK);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->setTextSize(0);
  _tft->println("Hello World!");
  _tft->setTextSize(1);
  _tft->setTextColor(ST77XX_GREEN);
  _tft->print(p, 6);
  _tft->println(" Want pi?");
  _tft->println(" ");
  _tft->print(8675309, HEX); // print 8,675,309 out in HEX!
  _tft->println(" Print HEX!");
  _tft->println(" ");
  _tft->setTextColor(ST77XX_WHITE);
  _tft->println("Sketch has been");
  _tft->println("running for: ");
  _tft->setTextColor(ST77XX_MAGENTA);
  _tft->print(millis() / 1000);
  _tft->setTextColor(ST77XX_WHITE);
  _tft->print(" seconds.");
}

void mediabuttons() {
  // play
  _tft->fillScreen(ST77XX_BLACK);
  _tft->fillRoundRect(25+Offset_X, 10, 78, 60, 8, ST77XX_WHITE);
  _tft->fillTriangle(42+Offset_X, 20, 42+Offset_X, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  _tft->fillRoundRect(25+Offset_X, 90, 78, 60, 8, ST77XX_WHITE);
  _tft->fillRoundRect(39+Offset_X, 98, 20, 45, 5, ST77XX_GREEN);
  _tft->fillRoundRect(69+Offset_X, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  _tft->fillTriangle(42+Offset_X, 20, 42+Offset_X, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  _tft->fillRoundRect(39+Offset_X, 98, 20, 45, 5, ST77XX_RED);
  _tft->fillRoundRect(69+Offset_X, 98, 20, 45, 5, ST77XX_RED);
  // play color
  _tft->fillTriangle(42+Offset_X, 20, 42+Offset_X, 60, 90, 40, ST77XX_GREEN);
}

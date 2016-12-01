/***************************************************
  This is an example sketch for the Adafruit 2.2&quot; SPI display.
  This library works with the Adafruit 2.2&quot; TFT Breakout w/SD card
  ----&gt; http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
#include &quot;SPI.h&quot;
#include &quot;Adafruit_GFX.h&quot;
#include &quot;Adafruit_ILI9340.h&quot;

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8

// Using software SPI is really not suggested, its incredibly slow
//Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(&quot;Adafruit 2.2\&quot; SPI TFT Test!&quot;); 
 
  tft.begin();

  Serial.println(F(&quot;Benchmark                Time (microseconds)&quot;));
  Serial.print(F(&quot;Screen fill              &quot;));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F(&quot;Text                     &quot;));
  Serial.println(testText());
  delay(3000);

  Serial.print(F(&quot;Lines                    &quot;));
  Serial.println(testLines(ILI9340_CYAN));
  delay(500);

  Serial.print(F(&quot;Horiz/Vert Lines         &quot;));
  Serial.println(testFastLines(ILI9340_RED, ILI9340_BLUE));
  delay(500);

  Serial.print(F(&quot;Rectangles (outline)     &quot;));
  Serial.println(testRects(ILI9340_GREEN));
  delay(500);

  Serial.print(F(&quot;Rectangles (filled)      &quot;));
  Serial.println(testFilledRects(ILI9340_YELLOW, ILI9340_MAGENTA));
  delay(500);

  Serial.print(F(&quot;Circles (filled)         &quot;));
  Serial.println(testFilledCircles(10, ILI9340_MAGENTA));

  Serial.print(F(&quot;Circles (outline)        &quot;));
  Serial.println(testCircles(10, ILI9340_WHITE));
  delay(500);

  Serial.print(F(&quot;Triangles (outline)      &quot;));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F(&quot;Triangles (filled)       &quot;));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F(&quot;Rounded rects (outline)  &quot;));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F(&quot;Rounded rects (filled)   &quot;));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F(&quot;Done!&quot;));
}

void loop(void) {
  for(uint8_t rotation=0; rotation&lt;4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(2000);
  }
}


unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9340_BLACK);
  tft.fillScreen(ILI9340_RED);
  tft.fillScreen(ILI9340_GREEN);
  tft.fillScreen(ILI9340_BLUE);
  tft.fillScreen(ILI9340_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9340_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(1);
  tft.println(&quot;Hello World!&quot;);
  tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9340_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(5);
  tft.println(&quot;Groop&quot;);
  tft.setTextSize(2);
  tft.println(&quot;I implore thee,&quot;);
  tft.setTextSize(1);
  tft.println(&quot;my foonting turlingdromes.&quot;);
  tft.println(&quot;And hooptiously drangle me&quot;);
  tft.println(&quot;with crinkly bindlewurdles,&quot;);
  tft.println(&quot;Or I will rend thee&quot;);
  tft.println(&quot;in the gobberwarts&quot;);
  tft.println(&quot;with my blurglecruncheon,&quot;);
  tft.println(&quot;see if I don't!&quot;);
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9340_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2&lt;w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2&lt;h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(ILI9340_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2&lt;w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2&lt;h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9340_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2&lt;w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2&lt;h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9340_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2&lt;w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2&lt;h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(y=0; y&lt;h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x&lt;w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9340_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i&lt;n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i&gt;0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(x=radius; x&lt;w; x+=r2) {
    for(y=radius; y&lt;h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x&lt;w; x+=r2) {
    for(y=0; y&lt;h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i&lt;n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.Color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(i=min(cx,cy); i&gt;10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.Color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i&lt;w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9340_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i&gt;20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.Color565(0, i, 0));
  }

  return micros() - start;
}

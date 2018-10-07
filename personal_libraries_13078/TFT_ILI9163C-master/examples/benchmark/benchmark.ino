
#include &lt;SPI.h&gt;
#include &lt;Adafruit_GFX.h&gt;
#include &lt;TFT_ILI9163C.h&gt;

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint8_t errorCode = 0;

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS 10
#define __DC 6
/*
Teensy 3.x can use: 2,6,9,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
*/


TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, 23);

void setup() {
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial &amp;&amp; ((millis () - debug_start) &lt;= 5000)) ;
  tft.begin();
  //the following it's mainly for Teensy
  //it will help you to understand if you have choosed the 
  //wrong combination of pins!
  errorCode = tft.errorCode();
  if (errorCode != 0) {
    Serial.print(&quot;Init error! &quot;);
    if (bitRead(errorCode, 0)) Serial.print(&quot;MOSI or SCLK pin mismach!\n&quot;);
    if (bitRead(errorCode, 1)) Serial.print(&quot;CS or DC pin mismach!\n&quot;);
  } else {
    Serial.println(F(&quot;Benchmark                Time (microseconds)&quot;));
  }
  if (errorCode == 0) {
    Serial.print(F(&quot;Screen fill              &quot;));
    Serial.println(testFillScreen());
    delay(500);

    Serial.print(F(&quot;Text                     &quot;));
    Serial.println(testText());
    delay(3000);

    Serial.print(F(&quot;Text2                    &quot;));
    Serial.println(testText2());
    delay(3000);

    Serial.print(F(&quot;Lines                    &quot;));
    Serial.println(testLines(CYAN));
    delay(500);

    Serial.print(F(&quot;Horiz/Vert Lines         &quot;));
    Serial.println(testFastLines(RED, BLUE));
    delay(500);

    Serial.print(F(&quot;Rectangles (outline)     &quot;));
    Serial.println(testRects(GREEN));
    delay(500);

    Serial.print(F(&quot;Rectangles (filled)      &quot;));
    Serial.println(testFilledRects(YELLOW, MAGENTA));
    delay(500);

    Serial.print(F(&quot;Circles (filled)         &quot;));
    Serial.println(testFilledCircles(10, MAGENTA));

    Serial.print(F(&quot;Circles (outline)        &quot;));
    Serial.println(testCircles(10, WHITE));
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
}

void loop(void) {
  for (uint8_t rotation = 0; rotation &lt; 4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(2000);
  }
}


unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen();
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen();
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println(&quot;Hello World!&quot;);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.println(0xDEAD, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.println(&quot;Hello&quot;);
  return micros() - start;


}

unsigned long testText2() {
  tft.fillScreen();
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
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

  tft.fillScreen();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 &lt; w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 &lt; h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 &lt; w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 &lt; h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 &lt; w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 &lt; h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 &lt; w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 &lt; h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen();
  start = micros();
  for (y = 0; y &lt; h; y += 5) tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x &lt; w; x += 5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen();
  n     = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i &lt; n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.fillScreen();
  n = min(tft.width(), tft.height());
  for (i = n; i &gt; 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen();
  start = micros();
  for (x = radius; x &lt; w; x += r2) {
    for (y = radius; y &lt; h; y += r2) {
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
  for (x = 0; x &lt; w; x += r2) {
    for (y = 0; y &lt; h; y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = (tft.height() / 2) - 1;

  tft.fillScreen();
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i &lt; n; i += 5) {
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
  int           i, cx = (tft.width() / 2) - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen();
  start = micros();
  for (i = min(cx, cy); i &gt; 10; i -= 5) {
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
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.fillScreen();
  w     = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i &lt; w; i += 6) {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = (tft.width()  / 2) - 1,
                cy = (tft.height() / 2) - 1;

  tft.fillScreen();
  start = micros();
  for (i = min(tft.width(), tft.height()); i &gt; 20; i -= 6) {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(0, i, 0));
  }

  return micros() - start;
}
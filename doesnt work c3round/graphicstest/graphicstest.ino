//Compiles with ESP32 board Core 2.0.17

#include "SPI.h"
//#include "src/Adafruit_GFX/Adafruit_GFX.h"
//#include "Adafruit_GC9A01A.h"
#include "src/Arduino_GFX/Arduino_GFX_Library.h"

// Define pins for display interface. You'll probably need to edit this for
// your own needs:


#define TFT_DC_PIN  2
#define TFT_CS_PIN 10
#define TFT_BL_PIN 3
#define TFT_CLK_PIN 6
#define TFT_MOSI_PIN 7
#define TFT_MISO_PIN -1
#define TFT_RST -1
const int pinResetTFT = -1;

// Display constructor for primary hardware SPI connection -- the specific
// pins used for writing to the display are unique to each board and are not
// negotiable. "Soft" SPI (using any pins) is an option but performance is
// reduced; it's rarely used, see header file for syntax if needed.
//Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Arduino_DataBus *tftBus = new Arduino_HWSPI( TFT_DC_PIN, TFT_CS_PIN, TFT_CLK_PIN, TFT_MOSI_PIN, -1 );
Arduino_G/*FX*/ *tft = new Arduino_GC9A01( tftBus, -1, /*tftBus, pinResetTFT,*/ 0 /* rotation */, true /*false*/ /* IPS */ );
Arduino_GFX *gfx = new Arduino_Canvas (240, 240, tft);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("GC9A01A Test!");

  //gfx->begin();
if (!gfx->begin(80000000)) {Serial.println("gfx begin fail"); }


#if defined(TFT_BL_PIN)
  pinMode(TFT_BL_PIN, OUTPUT);
  digitalWrite(TFT_BL_PIN, HIGH); // Backlight on
#endif // end TFT_BL
 //tft->invertDisplay( true );
  gfx->fillScreen( BLACK );
  gfx->flush();



  Serial.println(F("Benchmark                Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  //Serial.println(testFillScreen());
  testFillScreen();
  delay(500);

  Serial.print(F("Text                     "));
  testText();
  delay(3000);

  Serial.print(F("Lines                    "));
  testLines(CYAN);
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  testFastLines(RED, BLUE);
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  testRects(GREEN);
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  testFilledRects(YELLOW, MAGENTA);
  delay(500);

  Serial.print(F("Circles (filled)         "));
  testFilledCircles(10, MAGENTA);

  Serial.print(F("Circles (outline)        "));
  testCircles(10, WHITE);
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  testTriangles();
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  testFilledTriangles();
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  testRoundRects();
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  testFilledRoundRects();
  delay(500);

  Serial.println(F("Done!"));
}

void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    gfx->setRotation(rotation);
    testText();
    delay(1000);
  }
}

unsigned long testFillScreen() {
  //unsigned long start = micros();
  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();
  gfx->fillScreen(RED);
  gfx->flush();
  yield();
  gfx->fillScreen(GREEN);
  gfx->flush();
  yield();
  gfx->fillScreen(BLUE);
  gfx->flush();
  yield();
  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();
  //return micros() - start;
}

unsigned long testText() {
  gfx->fillScreen(BLACK);
  gfx->flush();
  unsigned long start = micros();
  gfx->setCursor(0, 0);
  gfx->setTextColor(WHITE);  gfx->setTextSize(1);
  gfx->println("Hello World!");
  
  gfx->setTextColor(YELLOW); gfx->setTextSize(2);
  gfx->println(1234.56);
  gfx->setTextColor(RED);    gfx->setTextSize(3);
  gfx->println(0xDEADBEEF, HEX);
  gfx->println();
  gfx->setTextColor(GREEN);
  gfx->setTextSize(5);
  gfx->println("Groop");
  gfx->setTextSize(2);
  gfx->println("I implore thee,");
  gfx->setTextSize(1);
  gfx->println("my foonting turlingdromes.");
  gfx->println("And hooptiously drangle me");
  gfx->println("with crinkly bindlewurdles,");
  gfx->println("Or I will rend thee");
  gfx->println("in the gobberwarts");
  gfx->println("with my blurglecruncheon,");
  gfx->println("see if I don't!");
  gfx->flush();
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = gfx->width(),
                h = gfx->height();

  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing
gfx->flush();
  yield();
  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;
gfx->flush();
  yield();
  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;
gfx->flush();
  yield();
  gfx->fillScreen(BLACK);
  gfx->flush();
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) gfx->drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) gfx->drawLine(x1, y1, x2, y2, color);
gfx->flush();
  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = gfx->width(), h = gfx->height();

  gfx->fillScreen(BLACK);
  gfx->flush();
  start = micros();
  for(y=0; y<h; y+=5) gfx->drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) gfx->drawFastVLine(x, 0, h, color2);
gfx->flush();
  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = gfx->width()  / 2,
                cy = gfx->height() / 2;

  gfx->fillScreen(BLACK);
  gfx->flush();
  n     = min(gfx->width(), gfx->height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    gfx->drawRect(cx-i2, cy-i2, i, i, color);
    gfx->flush();
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = gfx->width()  / 2 - 1,
                cy = gfx->height() / 2 - 1;

  gfx->fillScreen(BLACK);
  n = min(gfx->width(), gfx->height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    gfx->fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    gfx->drawRect(cx-i2, cy-i2, i, i, color2);
    gfx->flush();
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = gfx->width(), h = gfx->height(), r2 = radius * 2;

  gfx->fillScreen(BLACK);
  gfx->flush();
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      gfx->fillCircle(x, y, radius, color);
    }
  }
gfx->flush();
  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = gfx->width()  + radius,
                h = gfx->height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      gfx->drawCircle(x, y, radius, color);
      gfx->flush();
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = gfx->width()  / 2 - 1,
                      cy = gfx->height() / 2 - 1;

  gfx->fillScreen(BLACK);
  gfx->flush();
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    gfx->drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      gfx->color565(i, i, i));
      gfx->flush();
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = gfx->width()  / 2 - 1,
                   cy = gfx->height() / 2 - 1;

  gfx->fillScreen(BLACK);
  gfx->flush();
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    gfx->fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      gfx->color565(0, i*10, i*10));
    t += micros() - start;
    gfx->drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      gfx->color565(i*10, i*10, 0));
      gfx->flush();
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = gfx->width()  / 2 - 1,
                cy = gfx->height() / 2 - 1;

  gfx->fillScreen(BLACK);
  gfx->flush();
  w     = min(gfx->width(), gfx->height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    gfx->drawRoundRect(cx-i2, cy-i2, i, i, i/8, gfx->color565(i, 0, 0));
    gfx->flush();
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = gfx->width()  / 2 - 1,
                cy = gfx->height() / 2 - 1;

  gfx->fillScreen(BLACK);
  gfx->flush();
  start = micros();
  for(i=min(gfx->width(), gfx->height()); i>20; i-=6) {
    i2 = i / 2;
    gfx->fillRoundRect(cx-i2, cy-i2, i, i, i/8, gfx->color565(0, i, 0));
    gfx->flush();
    yield();
  }

  return micros() - start;
}

#include "src/U8g2/U8g2lib.h"
#include "src/Arduino_GFX/Arduino_GFX_Library.h"
//#include <Arduino_GFX_Library.h>
#include "time.h"
#include "FreeMono8pt7b.h"

// === Display ===

#define TFT_BL_PIN 3
#define TFT_DC_PIN 2 
#define TFT_CS_PIN 10 
#define TFT_CLK_PIN 6 
#define TFT_MOSI_PIN 7 

Arduino_DataBus *tftBus = new Arduino_HWSPI( TFT_DC_PIN, TFT_CS_PIN, TFT_CLK_PIN, TFT_MOSI_PIN, -1 );

const int pinResetTFT = -1;

Arduino_GFX *tft = new Arduino_GC9A01( tftBus, -1, /*tftBus, pinResetTFT,*/ 0 /* rotation */, false /* IPS */ );
//Arduino_GFX *tft = new Arduino_Canvas( 240 /* width */, 240 /* height */, tftset, 0, 0 );




void setup() {
 // Init screen
if (!tft->begin()) {Serial.println("gfx begin fail"); }

 // Turn on display backlight
  #ifdef TFT_BL_PIN
  pinMode( TFT_BL_PIN, OUTPUT );
  digitalWrite( TFT_BL_PIN, HIGH );
  #endif
  
  tft->invertDisplay( true );
  tft->fillScreen( BLACK );
  tft->flush();
tft->setCursor(100, 100);
  tft->setFont(&FreeMono8pt7b);
  
  tft->setTextColor(RED);
  tft->println("Hello World!");

delay(1000);
tft->fillScreen( BLACK );
paintDragonBallMockup();

  while(1);

//paintDragonBallMockup();
//delay(10 * 1000);
}

void loop() {
 tft->setCursor(random(tft->width()), random(tft->height()));
  tft->setTextColor(random(0xffff), random(0xffff));
  tft->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
  tft->println("Hello World!");

  delay(100); // 1 second
}

void paintDragonBallMockup() {

  int width = tft->width();
  int height = tft->height();

  int grid = 20;

  for ( int x = grid; x < width; x += grid ) {
    tft->drawFastVLine( x, 0, height, RGB565( 20, 70, 0 ) );
  }

  for ( int y = grid; y < width; y += grid ) {
    tft->drawFastHLine( 0, y, width, RGB565( 20, 70, 0 ) );
  }

  int ballRadius = 4;

  tft->fillCircle( 56, 78, ballRadius, YELLOW );
  tft->fillCircle( 145, 18, ballRadius, YELLOW );
  tft->fillCircle( 125, 180, ballRadius, YELLOW );
  tft->fillCircle( 235, 146, ballRadius, YELLOW );

  int w2 = width / 2;
  int h2 = height / 2;

  tft->fillTriangle(
    w2, h2,
    w2 - 9, h2 + 9,
    w2 + 9, h2 + 9,
    RED
  );

}

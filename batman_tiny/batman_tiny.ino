
//Compiles with ESP32 board Core 2.0.17

#include "src/Arduino_GFX/Arduino_GFX_Library.h"
#include "xbm.h" 
//#include "FreeMono8pt7b.h"
//https://www.digole.com/tools/PicturetoC_Hex_converter.php

#define TFT_BL_PIN 33
#define TFT_DC_PIN 36
#define TFT_CS_PIN 35 
#define TFT_CLK_PIN 40 
#define TFT_MOSI_PIN 38 

#define GIF_FILENAME "/ezgif.com-resize.gif"

Arduino_DataBus *tftBus = new Arduino_HWSPI( TFT_DC_PIN, TFT_CS_PIN, TFT_CLK_PIN, TFT_MOSI_PIN, 34 );

const int pinResetTFT = 34;

//Arduino_GFX *tft = new Arduino_GC9A01( tftBus, 34, /*tftBus, pinResetTFT,*/ 0 /* rotation */, true /*false*/ /* IPS */ );
Arduino_GFX *tft = new Arduino_GC9107( tftBus, 34, /*tftBus, pinResetTFT,*/ 0 /* rotation */, true /*false*/ /* IPS */ );


//Arduino_GFX *tft = new Arduino_Canvas( 240 /* width */, 240 /* height */, tftset, 0, 0 );


void setup() {
  if (!tft->begin(80000000)) {Serial.println("gfx begin fail"); }

 // Turn on display backlight
  #ifdef TFT_BL_PIN
  pinMode( TFT_BL_PIN, OUTPUT );
  digitalWrite( TFT_BL_PIN, HIGH );
  #endif
  
  //tft->invertDisplay( true );
  tft->fillScreen( BLACK );
  tft->flush();
tft->setCursor(0, 0);
  //tft->setFont(&FreeMono8pt7b);
  
  tft->setTextColor(RED);
//  tft->fillScreen( BLACK );

tft->print("Hello World!!!");

//tft->draw16bitRGBBitmap((int16_t)0, (int16_t)0, logo, (int16_t)logoWidth, (int16_t)logoHeight);
}

void loop() {
  tft->setCursor(random(tft->width()), random(tft->height()));
  tft->setTextColor(random(0xffff), random(0xffff));
  tft->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
  tft->println("Hello World!");
 delay(50);
}

// Use https://littlevgl.com/image-to-c-array to convert your images
// Use 16bit color with pixel format Red: 5bit, Green: 6bit, Blue: 5bit


#include "xbm.h"             // Sketch tab header for xbm images
#include "src/Adafruit_GFX/Adafruit_GFX.h"    // Core graphics library
//#include "src/Adafruit_ST7735/Adafruit_ST7735.h" // Hardware-specific library for ST7735
#include "src/Adafruit_ST7735/Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include <SPI.h>



//#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
  #define TFT_CS    14     
  #define TFT_RST   21
  #define TFT_DC    15
  #define TFT_MOSI  6  // Data out
  #define TFT_SCLK  7  // Clock out

#define TFT_LED 22
// For ST7735-based displays, we will use this call
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
 
 //tft.initSPI( 80000000 /*14000000*/, SPI_MODE0); //80000000, SPI_MODE2);
 pinMode(TFT_LED, OUTPUT);
 //digitalWrite(TFT_LED, HIGH);
 analogWrite(TFT_LED, 50);
  
 tft.init(172+34, 320, SPI_MODE0); //(172, 320, SPI_MODE0); //INITR_GREENTAB); //MINI160x80);      // Init ST7735S chip, green tab
// tft.invertDisplay(true);
// tft.setRotation(ST7735_MADCTL_MH);
 tft.setRotation(2);
 tft.fillScreen(ST77XX_BLACK); // Black screen fill
 tft.drawRGBBitmap((int16_t)90, (int16_t)30, logo, (int16_t)logoWidth, (int16_t)logoHeight);
 delay(500);
  
}

void loop() {

  
}

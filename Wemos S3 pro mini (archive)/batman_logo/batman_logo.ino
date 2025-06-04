// Use https://littlevgl.com/image-to-c-array to convert your images
// Use 16bit color with pixel format Red: 5bit, Green: 6bit, Blue: 5bit


#include "xbm.h"             // Sketch tab header for xbm images
#include "src/Adafruit_GFX/Adafruit_GFX.h"    // Core graphics library
#include "src/Adafruit_ST7735/Adafruit_ST7735.h" // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
  #define TFT_CS         35
  #define TFT_RST        34
  #define TFT_DC         36
  #define TFT_MOSI 38  // Data out
  #define TFT_SCLK 40  // Clock out

#define TFT_LED 33
// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {

 pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

tft.initR(INITR_GREENTAB);
tft.setRotation(2);
tft.invertDisplay(1);
  tft.fillScreen(ST77XX_BLACK); // Black screen fill

// Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  tft.drawRGBBitmap((int16_t)0, (int16_t)0, logo, (int16_t)logoWidth, (int16_t)logoHeight);

  delay(500);

  
}

void loop() {

  
}

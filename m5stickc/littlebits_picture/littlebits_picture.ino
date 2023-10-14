// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/14/18

//use https://convertio.co/jpg-xbm/ to convert files

#include "xbm.h"             // Sketch tab header for xbm images

#include "src/M5StickC/M5StickC.h"        // Hardware-specific library


void setup() {

  M5.begin();               // Initialise the display

  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
}

void loop() {

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  M5.Lcd.drawXBitmap(0, 0, logo, logoWidth, logoHeight, TFT_WHITE, TFT_PURPLE);

  delay(500);

  // Erase old one by drawing over with background colour
//  M5.Lcd.drawXBitmap(0, 0, logo, logoWidth, logoHeight, TFT_BLUE, TFT_BLACK);

  delay(500);
}

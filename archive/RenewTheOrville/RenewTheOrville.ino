// RenewTheOrville-M5StickC.png
// https://lang-ship.com/tools/image2data/
// RGB565 Dump(big endian)

#include "RenewTheOrville-M5StickCpng.h"             // Sketch tab header for xbm images

#include "src/M5StickC/M5StickC.h"        // Hardware-specific library


void setup() {

  M5.begin();               // Initialise the display
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
}

void loop() {

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  M5.Lcd.drawBitmap(0, 0, imgWidth, imgHeight, img);

  delay(500);

  // Erase old one by drawing over with background colour
//  M5.Lcd.drawXBitmap(0, 0, logo, logoWidth, logoHeight, TFT_BLUE, TFT_BLACK);

  delay(500);
}

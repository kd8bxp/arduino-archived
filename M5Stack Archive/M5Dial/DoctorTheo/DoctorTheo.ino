// use: https://lang-ship.com/tools/image2data/
// 16 bit RGB565 Big Endian

#include "doctortheo.h"             // Sketch tab header for xbm images
#include "M5Dial.h"        // Hardware-specific library


void setup() {

  M5.begin();               // Initialise the display

  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
}

void loop() {

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
 // M5.Lcd.drawBitmap(0, 0, imgWidth, imgHeight, img);

  delay(500);

  // Erase old one by drawing over with background colour
  M5.Lcd.drawBitmap(0, 0, imgWidth, imgHeight, img);

  delay(500);
}

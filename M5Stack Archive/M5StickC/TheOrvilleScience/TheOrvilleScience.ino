// Use https://littlevgl.com/image-to-c-array to convert your images
// Use 16bit color with pixel format Red: 5bit, Green: 6bit, Blue: 5bit


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
  M5.Lcd.drawBitmap(0, 0, logoWidth, logoHeight, logo);

  delay(500);

  // Erase old one by drawing over with background colour
//  M5.Lcd.drawXBitmap(0, 0, logo, logoWidth, logoHeight, TFT_BLUE, TFT_BLACK);

  delay(500);
}

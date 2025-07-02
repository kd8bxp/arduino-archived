#include "src/U8g2/U8g2lib.h"


/*
 * Use GIMP resize image to 128x64.
 * convert to indexed bitmap, 2 levels, black & white, 
 * export (convert) to xbm (.xbm extension)
 * 
 * 1st test this works, but colors are inverted on display.
 */

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "xbm.h"

U8G2_SH1107_64X128_F_4W_HW_SPI u8g2(U8G2_R3, /* cs=*/ 14, /* dc=*/ 27, /* reset=*/ 33);

void setup() {
   u8g2.begin();
  // lightness
  // u8g2.setContrast(32);
  //u8g2.setContrast(10);
  
  //u8g2.setFont(NORMAL_FONT);
 //  u8g2.setFont(u8g2_font_6x10_tf);
 // u8g2.setFontRefHeightExtendedText();
  // u8g2.setDrawColor(1);
 // u8g2.setFontPosTop();
  // u8g2.setFontDirection(0);
  
  u8g2.clearBuffer();
  u8g2.drawXBM(0, 0,logoWidth, logoHeight, logo);
  //u8g2.drawStr(0, 9, "012345678901234567890");
  u8g2.sendBuffer();
}

void loop() {
  // put your main code here, to run repeatedly:

}

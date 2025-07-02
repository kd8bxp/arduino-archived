/* The Orville Font Display Test
 *  The font is the called Space Age Font and can be downloaded from
 *  https://fontmeme.com/fonts/space-age-font/
 *  It was converted for use with the Arduino IDE using this tool
 *  https://rop.nl/truetype2gfx/
 *  
 *  LeRoy Miller (c) 2020
 */

#include "src/M5StickC/M5StickC.h"
#include "space_age8pt7b.h"

void setup() {
  m5.begin();
  m5.lcd.fillScreen(TFT_BLACK);
  m5.lcd.setRotation(3);
  m5.lcd.setTextColor(0x02EE); //DARKCYAN);
  //m5.lcd.setTextDatum(CC_DATUM);
  m5.lcd.setFreeFont(&space_age8pt7b);
  m5.lcd.setTextSize(1);
  m5.lcd.drawString("The Orville", 8, 28);
  m5.lcd.drawString("ECV-197", 32, 42);
}

void loop() {
}

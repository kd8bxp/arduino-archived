/* The Orville Font Display Test
 *  The font is the called Space Age Font and can be downloaded from
 *  https://fontmeme.com/fonts/space-age-font/
 *  It was converted for use with the Arduino IDE using this tool
 *  https://rop.nl/truetype2gfx/
 *  
 *  LeRoy Miller (c) 2020
 */

#include "src/M5StickC/M5StickC.h"
#include "MorseCode_15pt7b.h" //MorsoKode_dnXX15pt7b
#include "MorseCode_35pt7b.h" //MorseCode_wlGw35pt7b
#include "MorseCode_lgvy15pt7b.h" //Setiperu_lgvy15pt7b

void setup() {
  m5.begin();
  m5.lcd.fillScreen(TFT_BLACK);
  m5.lcd.setRotation(3);
  m5.lcd.setTextColor(0x02EE); //DARKCYAN);
  //m5.lcd.setTextDatum(CC_DATUM);
  m5.lcd.setFreeFont(&MorsoKode_dnXX15pt7b);
  m5.lcd.setTextSize(0);
  m5.lcd.drawString("123", 0, 0);
  //m5.lcd.drawString("ECV-197", 32, 42);
  delay(5000);
  m5.lcd.setFreeFont(&MorseCode_wlGw35pt7b);
  m5.lcd.drawString("123", 0, 12);
  delay(5000);
  m5.lcd.setFreeFont(&Setiperu_lgvy15pt7b);
  m5.lcd.drawString("123", 0, 25);
}

void loop() {
}

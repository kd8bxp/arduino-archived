/*
Copyright (c) 2019 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
#include <M5StickC.h>

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

void setup() {
M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(45, 0);
  M5.Lcd.println("Binary Clock");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(115, 12);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(95, 12);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(75, 12);
  M5.Lcd.println("4");
  M5.Lcd.setCursor(55, 12);
  M5.Lcd.println("8");
  M5.Lcd.setCursor(24, 12);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(8,37);
  M5.Lcd.println("32");
  M5.Lcd.setCursor(34,37);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(65,37);
  M5.Lcd.print("8");
  M5.Lcd.setCursor(125, 37);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(105, 37);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(85, 37);
  M5.Lcd.println("4");

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(8,65);
  M5.Lcd.println("32");
  M5.Lcd.setCursor(34,65);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(65,65);
  M5.Lcd.print("8");
  M5.Lcd.setCursor(125, 65);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(105, 65);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(85, 65);
  M5.Lcd.println("4");

}

void loop() {
  

}

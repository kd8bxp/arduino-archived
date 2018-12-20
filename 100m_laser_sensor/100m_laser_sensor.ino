/*
Copyright (c) 2018 LeRoy Miller

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

#include <Wire.h>
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
boolean laseron = 0;
int previousButton = 5;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup() {
  Serial.begin(19200);
  lcd.begin(16, 2);        
  lcd.setCursor(0, 0);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void loop() {
  lcd_key = readButtons();  // read the buttons

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      //serialFlush();
      Serial.println("M"); //Take Slow Reading
      while(Serial.available()) {
        char inByte = Serial.read();
        lcd.print(inByte);
      }
      //serialFlush();
      break;
    }
    case btnLEFT:
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      //serialFlush();
      Serial.println("S"); //Show Status
      while(Serial.available()) {
        char inByte = Serial.read();
        lcd.write(inByte);
      }
      //serialFlush();
      break;
    }
    case btnUP:
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      //serialFlush();
      Serial.println("D"); //Take Normal Reading
      while(Serial.available()) {
        char inByte = Serial.read();
        lcd.print(inByte);
      }
      //serialFlush();
      break;
    }
    case btnDOWN:
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      //serialFlush();
      Serial.println("F"); //Take Fast Reading
      while(Serial.available()) {
        char inByte = Serial.read();
        lcd.print(inByte);
      }
      //serialFlush();
      break;
    }
    case btnSELECT:
    {
      //Toggle Laser ON/OFF
      
      laseron = !laseron;
      if (laseron == 1) {
        Serial.println("O");
      } 
      if (laseron == 0) { 
        Serial.println("C");
      }
      serialFlush();
      break;
    }
    case btnNONE:
    {
      //delay(50);
      break;
    }
  }
  
delay(200);
}

int readButtons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
// we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) {previousButton = btnNONE; return btnNONE; }// We make this the 1st option for speed reasons since it will be the most likely result


// For V1.0 comment the other threshold and use the one below:

  if (adc_key_in < 50 && previousButton != btnRIGHT) { previousButton = btnRIGHT;  return btnRIGHT; }
  if (adc_key_in < 150 && previousButton != btnUP)  { previousButton = btnUP; return btnUP; }
  if (adc_key_in < 265 && previousButton != btnDOWN) { previousButton = btnDOWN;  return btnDOWN; }
  if (adc_key_in < 450 && previousButton != btnLEFT) { previousButton = btnLEFT;  return btnLEFT; }
  if (adc_key_in < 690 && previousButton != btnSELECT) { previousButton = btnSELECT; return btnSELECT; }

  previousButton = btnNONE;
  return btnNONE;  // when all others fail, return this...
}

void serialFlush() {
  lcd.setCursor(0,1); 
  while(Serial.available()) {
    char t = Serial.read();
    
    //lcd.print(t);
  }
}


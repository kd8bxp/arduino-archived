// IMPORTANT USE ESP32 CORE 1.0.6 and ESP32 Dev Module
// The Kano Kit has problems with CORE 2.0.0 and above (USE ONLY CORE 1.0.6)

/*
Copyright (c) 2021 LeRoy Miller

Portions of this code based on Miles Nash's "New Year's Countdown"
https://create.arduino.cc/projecthub/milesnash_/new-year-s-countdown-aeefff
CC BY-NC-SA https://creativecommons.org/licenses/by-nc-sa/4.0/

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

#include "src/FastLED/FastLED.h"
#include "src/FastLED-GFX/FastLED_GFX.h" //https://github.com/Jorgen-VikingGod/FastLED-GFX
#include "time.h"
#include "src/Time/TimeLib.h"
#include <WiFi.h>

#define LED_PIN            4
#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B
#define BRIGHTNESS      10 //57

#define CANVAS_WIDTH    16
#define CANVAS_HEIGHT   8
#define NUM_LEDS        (CANVAS_WIDTH * CANVAS_HEIGHT)
#define MATRIX_TYPE         HORIZONTAL_ZIGZAG_MATRIX

CRGB leds[NUM_LEDS];

GFXcanvas matrix(CANVAS_WIDTH, CANVAS_HEIGHT);

int x    = matrix.width();
bool flag = false;

char ssid[] = "MySpectrumWiFi68-2G";
char pass[] = "loudowl369";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400 + -3600;
const int   daylightOffset_sec = -14400 +-3600;

//date and time of the event being counted to
int countHour = 0; //(0-23)
int countDay = 1; //(1 - 30ish)
int countMonth = 1; //(1- 12)
int countYear = 2024; // the date limited to being less than a year (364 days) away
int daysLeft;
int hoursLeft;
int minutesLeft;
int secondsLeft;
int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
int currentNum;
int countDownNum;

void setup() {
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix.getBuffer(), NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);
  matrix.setTextWrap(false);
  matrix.setTextColor(CRGB(0,255,0));
  randomSeed(analogRead(A0));
  Serial.begin(115200);
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  currentNum = 20;
  printLocalTime();
}

void loop() {
  printLocalTime();
  delay(1000);

  matrix.setTextColor(CRGB(random(0,256),random(0,256),random(0,256)));

  //based on Miles Nash code, Modified to work with the Kano Kit ESP32 matrix
  int lastSecondsLeft = secondsLeft;
  int lastMinutesLeft = minutesLeft;
  int lastHoursLeft = hoursLeft;
  int lastDaysLeft = daysLeft;
  countdownClock(countHour, countDay, countMonth, countYear); //update countdown
  //hour(0-23),day, month(1-12), year

  //clear the display when a digit being displayed changes
  if(hoursLeft < 1 && lastSecondsLeft != secondsLeft){
    matrix.fillScreen(0);
  }else if(daysLeft < 1 && lastMinutesLeft!= minutesLeft){
    matrix.fillScreen(0);
  }else if(lastHoursLeft != hoursLeft){
    matrix.fillScreen(0);
  }else if(lastDaysLeft != daysLeft){
    matrix.fillScreen(0);
  }
  
  matrix.setTextColor(CRGB(random(0,256),random(0,256),random(0,256)));
  if(secondsLeft == -1){
        displayText("Happy New Year!", 100);
        delay(3);
  }else if(daysLeft > 0 && daysLeft < 364){
            if(daysLeft < 10){
      //matrix.setCursor(4,1);
    }else{
      //matrix.setCursor(1,0);
    }
    displayText(String(daysLeft),100);
    displayText("day",100);
 
    if(hoursLeft < 10){
    //  matrix.setCursor(4,9);
    }else{
    //  matrix.setCursor(1,9);
    }

    displayText(String(hoursLeft),70); //100);
    displayText("hrs", 70);
   
  }else if(hoursLeft > 0){
    //hour and min display
    
    if(hoursLeft < 10){
    //  matrix.setCursor(4,1);
    }else{
    //  matrix.setCursor(1,0);
    }
    displayText(String(hoursLeft),70);
    displayText("hrs",70);

    if(minutesLeft < 10){
    //  matrix.setCursor(4,9);
    }else{
    //  matrix.setCursor(1,9);
    }

    displayText(String(minutesLeft),70);
    displayText("min", 70);
    
  }else if(minutesLeft > 0){
    //min and sec display
      if(minutesLeft < 10){
    //  matrix.setCursor(4,1);
    }else{
    //  matrix.setCursor(1,0);
    }
    displayText(String(minutesLeft),70);
    displayText("min", 70);

    if(secondsLeft < 10){
    //  matrix.setCursor(4,9);
    }else{
    //  matrix.setCursor(1,9);
    }

    displayText(String(secondsLeft),70);
    displayText("sec", 70);
    
  }else if(secondsLeft < 10 && secondsLeft > 0){
   
        displayText(String(secondsLeft), 70);
    
  }  else if(secondsLeft < 60 && secondsLeft > 9){
     
    displayText(String(secondsLeft),70);

  
  } else if(secondsLeft < -1){
    
      displayText("Happy New Year!", 100);
  }
  //end
}

int displayText(String temp, int speed) {
  flag = false;
  int len = temp.length();
  if (len < 11) { len = ((len*2)/4); } else { len = len/4; }
   do {
    matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(temp);
  
  if (--x < (-36*len)) {
    x = matrix.width();
    flag = true;
      }
      
  FastLED.show();
  delay(speed);
   } while(flag == false);
   
}

//Lifted from Miles Nash "New Year's Countdown"

void countdownClock(int h, int m, int d, int y){
  //takes in hour(24 hour time), month, date, and year
  //displays time left until that date
  int currentDate = totalDate(month(),day(),year());
  int countdownDate = totalDate(m, d, y);
  daysLeft = countdownDate - currentDate -1;
  

  if (hour() >= h){
    hoursLeft = 23 + h - hour() ;
  }else{
    hoursLeft = h - hour() - 1;
  }

  minutesLeft = 59 - minute();
  secondsLeft = 59 - second();

  if((hoursLeft < 0 || daysLeft < 0) || (daysLeft == 0 && hoursLeft == 0 && minutesLeft == 0 && secondsLeft == 0)){
    //return negative seconds
    Serial.println("event has occured");
    secondsLeft = -1;

  }
  Serial.print(String(month()) +","+ String(day()) + "," + String(year())+ " " + String(hour()) +":"+String(minute())+":"+String(second())+"     ");
  Serial.println(String(daysLeft) + "." + String(hoursLeft) + "." + String(minutesLeft) + "." + String(secondsLeft));

}

int totalDate(int m, int d, int y){
  //takes in a month date and year
  //calculates and returns the number of days since the December 31 1999
  //1/1/2000 = 1
 int days = 0;
 for(int monthID = 1; monthID < m; monthID++){
   days += daysInMonth[monthID -1];
 }
 days += d;
 days += (y - 2000)*365;
 return days;
}

// End lifted code.

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
 
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year+1900);
}

// ESP32 Time library info (of sorts): https://forum.arduino.cc/index.php?topic=536464.0

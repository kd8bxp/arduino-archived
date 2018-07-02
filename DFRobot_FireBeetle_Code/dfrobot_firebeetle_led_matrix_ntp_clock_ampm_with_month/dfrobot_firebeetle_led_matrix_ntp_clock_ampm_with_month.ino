/*
 * FireBeetle LED MATRIX NTP Clock
 * 
 * Sketch will display time in either 24 hour format (Also know as Military Time)
 * or with AM or PM.  See below for more on how to setup.
 * 
 * version 1.0.0 - June 1, 2018 
 * version 1.0.1 - June 6, 2018 - replaced SmartConfig with WiFiManager
 * cleaned up code, and added leading zeros where/when needed.
 * 
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

#include <TimeLib.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          
#endif

//needed for library
#include <DNSServer.h>  //https://github.com/bbx10/DNSServer_tng
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h> //https://github.com/bbx10/WebServer_tng
#endif
#include <WiFiManager.h>         //https://github.com/bbx10/WiFiManager/tree/esp32

#include <NTPClient.h>  //https://github.com/arduino-libraries/NTPClient
#include "DFRobot_HT1632C.h"
#include <WiFi.h>
#include <WiFiUdp.h>

#define DATA D6
#define CS D2
#define WR D7

DFRobot_HT1632C display = DFRobot_HT1632C(DATA, WR,CS);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#define TIMEOFFSET -14400 //Find your Time Zone off set Here https://www.epochconverter.com/timezones OFF Set in Seconds
#define AMPM 1 //1 = AM PM time, 0 = MILITARY/24 HR Time

char temp1[30];
int hours,minutes,seconds;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.isLedOn(true);
  display.clearScreen();
  display.setCursor(0,0);
  display.setFont(FONT8X4);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  display.print("Connected.....",30);
  timeClient.begin();
  timeClient.setTimeOffset(TIMEOFFSET);
}

void loop() {

display.setPwm(15);
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  seconds = timeClient.getSeconds();

       if (AMPM) {
        if (hours > 13) {
          hours = hours -12;
          sprintf(temp1,"%d:%02d:%02d pm",hours,minutes,seconds);
  } else { 
    if (hours == 0) {
      sprintf(temp1,"12:%02d:%02d am",minutes,seconds);
    } else {
    sprintf(temp1,"%d:%02d:%02d am",hours,minutes,seconds);
  }
 }
} else {
  
  sprintf(temp1,"%02d:%02d:%02d",hours,minutes,seconds);
}

display.print(temp1,40);
Serial.println(timeClient.getDay());
setTime(timeClient.getEpochTime());
//Serial.print("Month: ");
//Serial.println(monthStr(month()));
String mon = (String)dayStr(timeClient.getDay()+1) + ' ' + (String)monthStr(month()) + ' ' + day();
//Serial.println(mon);
//Serial.print("Day: ");
//Serial.println(dayStr(timeClient.getDay()+1));
//char *da = dayStr(timeClient.getDay()+1);
//Serial.print("Year: ");
//Serial.println(year());
sprintf(temp1," %d/%d/%d ",month(),day(),year());
display.print(temp1, 40);
//display.print(da,40);
//display.print(monthStr(month()), 40);
//sprintf (temp1,"%d", day());
//display.print(temp1,40);
char temps[51];
mon.toCharArray(temps,50);
display.print(temps,40);
delay(150);


}


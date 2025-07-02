//Compiles with M5 Board Core 1.0.6 (but still resets randomly)

/*
 * ISS Tracker for M5Stack-Core-ESP32
 * Inspired from Code by Giuliano Pisoni
 * portions of code by Giuliano Pisoni (Aug 2018) *MIT License
 * Portions of code by Tom Igoe (21 Jan 2010/30 Aug 2011) part of the Star Trek DS9 Theme Music
 * modified for use with the M5 Stack. (Aug 11, 2018 LeRoy Miller)
 * 
  v1.0.0 - Aug 12 - 13, 2018
  
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

#include "src/M5Stack/M5Stack.h"
#include "fonts.h"
#include <WiFi.h>
#include "src/ArduinoJson5/ArduinoJson.h" //version 5.13.4
#include <HTTPClient.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "FS.h"
#include "SPIFFS.h"
#include "src/WiFiManager/WiFiManager.h"  //https://github.com/bbx10/WiFiManager/tree/esp32 (change HEAD to HEADER)
#include "src/Time/TimeLib.h"  
#include "pitches.h"

String TZAPIKEY="";  //http://timezonedb.com used to get gmtOffset

const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec; 
const int   daylightOffset_sec = 0; 

String coordinate; // = lat,lon
String coordinatePrev; // lat=xx.xxx&lon=xxx.xxx  //Are we using this anywhere (?)
String coordinateTZ; //lat=xx.xxx&lng=xxx.xxx
String ISSCoord;
String payload, TZone, lastUpdate;
int distance, number, count;
int musicFlag = 0;
String name[10], craft[10],risetimeUTC[5],risetimeLocal[5];
float duration[5];
const String iss = "http://api.open-notify.org/iss-now.json"; 
const String ppl = "http://api.open-notify.org/astros.json";
String pas = "http://api.open-notify.org/iss-pass.json?";
float mylat, mylon, isslat, isslon;

void timeZone();
void printLocalTime();
void getJson(String url);
void downloadFile(String url, String filename);
void displayTime();
int zoom = 0;
int mode = 0; //start on map

unsigned long locpreviousMillis = 0; //Update map if 10 seconds have passed., this is based on blink without delay
int zoomChange = 0; //We also need to update if the zoom level changes.

void setup() {
  //We need to start setting up some variables and other things needed.
  Serial.begin(115200);
  M5.begin();
  title();
  SPIFFS.begin(true);
     if(!SPIFFS.begin()){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
  M5.Lcd.print("Connecting....");
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Connected....");
  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(0,15);
  M5.Lcd.println("Connected!");
  geolocation(); //get Latitude/Longitude from external IP address 
  timeZone(); //get timeZone GMT offset
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
  pas = pas + coordinatePrev;
  Serial.println(pas);
  xTaskCreate(
                    decodeLocJson,          /* Task function. */
                    "decodeLocJson",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
  xTaskCreate(
                    decodePassJson,          /* Task function. */
                    "decodePassJson",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */                      
  xTaskCreate(
                    decodePeopleJson,          /* Task function. */
                    "decodePeopleJson",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */  
  //xTaskCreate(
  //                  decodeWhereAT,          /* Task function. */
  //                  "decodeWhereAT",        /* String with name of task. */
  //                  10000,            /* Stack size in words. */
  //                  NULL,             /* Parameter passed as input of the task */
  //                  1,                /* Priority of the task. */
  //                  NULL);            /* Task handle. */   
                   
  M5.Lcd.setCursor(0,225);
  M5.Lcd.println("Updating ISS Location....");
  delay(1500);
  playMusic();
}

void loop() {
  
   if (M5.BtnC.wasPressed()){
     Serial.println("Button C pressed");
     zoom += 1;
     if (zoom > 5) {zoom = 5;}
     M5.Lcd.drawString("UPDATING MAP ...", 80, 100,GFXFF);
     updateMap(zoom);
  }
 
  if (M5.BtnA.wasPressed()){
    Serial.println("Button A pressed");
      zoom -= 1;
      if (zoom < 0) {zoom = 0;}
      M5.Lcd.drawString("UPDATING MAP ...", 80, 100,GFXFF);
     updateMap(zoom); 
      }

  if (M5.BtnB.wasPressed()){
     changeMode();
  }
  checkDistance();
  M5.update();

}

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

#include "src/Dweet/Dweet.h" //https://github.com/quentinpigne/arduino-dweet.io
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_D1_SSD1306.h>

#define OLED_RESET 0

String mode;
char* thingName = "relaystatus";

WiFiClient espClient;
Dweet dweet(espClient); 
Adafruit_D1_SSD1306 display(OLED_RESET);
unsigned long previousMillis = 0;
const long interval = 1000; //check every 1/2 second

void setup() {
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    Serial.begin(9600);
  WiFiManager wifiManager;
     wifiManager.autoConnect("AutoConnectAP");
display.clearDisplay();
  display.display();

}

void loop() {
   unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  getMode();
  display.clearDisplay();  
   }
   
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(mode);
  display.display();
  ESP.wdtFeed();
  yield();
}

void getMode() {
 // Get latest dweet !
 String json;
  char* res = dweet.get_latest(thingName);
  json = (String)res;
  //json.remove(json.indexOf("["));
  json.replace("[","");
  json.replace("]","}");
  char res1[json.length()+1];
  json.toCharArray(res1, json.length()+1);
  Serial.println(res1);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(res1);
  if (!root.success()) {
    Serial.println("parseObject() failed!");
    return;
  }
  mode = root["with"]["content"]["status"].as<String>();
  //Serial.println(mode);
  //mode.remove(0,140); //126
  //mode.remove(2);
  Serial.println(mode);
  //  ESP.wdtFeed();
  //yield();
}

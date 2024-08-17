/* Copyright (c) 2019 LeRoy Miller
 *  
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

 
 */

#include <Dweet.h> //https://github.com/quentinpigne/arduino-dweet.io
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>

#define relayPin D1

String mode;
char* thingName = "my-thing-name";

unsigned long previousMillis = 0;
const long interval = 1000; //check every 1/2 second

WiFiClient espClient;
Dweet dweet(espClient); 

void setup() {
   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(relayPin, LOW); //turn relay off
  Serial.begin(9600);
  WiFiManager wifiManager;
     wifiManager.autoConnect("AutoConnectAP");

}

void loop() {
   unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  getMode();
  if (mode == "off") {digitalWrite(relayPin, LOW); dweet.dweet("relaystatus", "{\"status\":\"off\"}"); }
  if (mode == "on") {digitalWrite(relayPin, HIGH); dweet.dweet("relaystatus", "{\"status\":\"on\"}"); }
  if (mode == "status") {
    int stats = digitalRead(relayPin);
    if (stats == 1) {dweet.dweet("relaystatus", "{\"status\":\"on\"}"); }
    if (stats == 0) {dweet.dweet("relaystatus", "{\"status\":\"off\"}");}
  }
   }
  
  ESP.wdtFeed();
  yield();
  //delay(100); 
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
  mode = root["with"]["content"]["relay"].as<String>();
  //Serial.println(mode);
  //mode.remove(0,140); //126
  //mode.remove(2);
  Serial.println(mode);
  //  ESP.wdtFeed();
  //yield();
}



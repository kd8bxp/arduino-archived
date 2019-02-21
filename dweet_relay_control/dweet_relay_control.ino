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

String mode;

unsigned long previousMillis = 0;
const long interval = 1000; //check every 1/2 second

WiFiClient espClient;
Dweet dweet(espClient); 

void setup() {
   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(0, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(0, LOW); //turn relay off
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
  if (mode == "of") {digitalWrite(0, LOW);}
  if (mode == "on") {digitalWrite(0, HIGH);}
   }
  
  ESP.wdtFeed();
  yield();
  //delay(100); 
}

void getMode() {
 // Get latest dweet !
  char* res = dweet.get_latest("my-thing-name");
  mode = (String)res;
  //Serial.println(mode);
  mode.remove(0,140); //126
  mode.remove(2);
  Serial.println(mode);
  //  ESP.wdtFeed();
  //yield();
}



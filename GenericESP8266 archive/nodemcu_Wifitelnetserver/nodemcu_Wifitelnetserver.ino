// File: TelnetServer.ino for ESP8266 NodeMCU
// 2015-12-07 Rudolf Reuter www.rudiswiki.de/wiki9 (search for "wifi")
// 2015-12-17 RR, structure copied from example WiFiTelnetToSerial.ino
//
// Developed for debug purpose, use like the Arduino Serial Monitor.
// Needs Arduino 1.6.5/6 to compile.
//
/*
 *   This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <ESP8266WiFi.h>
#include <Arduino.h>

// Set these to your desired softAP credentials. They are not configurable at runtime.
const char *ssid = "Motel6";
const char *password = "";

boolean debug = false;  // true = more messages
//boolean debug = true;

// LED is needed for failure signalling
const short int BUILTIN_LED2 = 16;  //GPIO16 on NodeMCU (ESP-12)

unsigned long startTime = millis();

// provide text for the WiFi status
const char *str_status[]= {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};

// provide text for the WiFi mode
const char *str_mode[]= { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };

//----------------------- WiFi handling
void connectWifi() {
  Serial.print("Connecting as wifi client to SSID: ");
  Serial.println(ssid);

  // use in case of mode problem
  WiFi.disconnect();
  // switch to Station mode
  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
  }

  WiFi.begin ( ssid, password );

  if (debug ) WiFi.printDiag(Serial);

  // ... Give ESP 10 seconds to connect to station.
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // Check connection
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected; IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.print("WiFi connect failed to ssid: ");
    Serial.println(ssid);
    Serial.print("WiFi password <");
    Serial.print(password);
    Serial.println(">");
    Serial.println("Check for wrong typing!");
  }
}  // connectWiFi()

void signalError() {  // loop endless with LED blinking in case of error
  while(1) {
      digitalWrite(BUILTIN_LED2, LOW);
      delay(300); // ms
      digitalWrite(BUILTIN_LED2, HIGH);
      delay(300); // ms
  }
}


// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);
WiFiClient serverClient;

void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Sync,Sync,Sync,Sync,Sync");
  delay(500);
  Serial.println();
  // signal start
  pinMode(BUILTIN_LED2, OUTPUT);
  digitalWrite(BUILTIN_LED2, LOW);
  delay(100); // ms
  digitalWrite(BUILTIN_LED2, HIGH);
  delay(300); // ms

  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  Serial.println ( "Connect to Router requested" );
  connectWifi();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi mode: ");
    Serial.println(str_mode[WiFi.getMode()]);
    Serial.print ( "Status: " );
    Serial.println (str_status[WiFi.status()]);
    // signal WiFi connect
    digitalWrite(BUILTIN_LED2, LOW);
    delay(300); // ms
    digitalWrite(BUILTIN_LED2, HIGH);      
  } else {
    Serial.println("");
    Serial.println("WiFi connect failed, push RESET button.");
    signalError();
  }

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");

  Serial.print("Free Heap[B]: ");
  Serial.println(ESP.getFreeHeap());
} // setup()


void loop() {
  // look for Client connect trial
  if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush();  // clear input buffer, else you get strange characters 
    }
  }

    while(serverClient.available()) {  // get data from Client
    Serial.write(serverClient.read());
  }
  
  if (millis() - startTime > 2000) { // run every 2000 ms
    startTime = millis();

    if (serverClient && serverClient.connected()) {  // send data to Client
      serverClient.print("Telnet Test, millis: ");
      serverClient.println(millis());
      serverClient.print("Free Heap RAM: ");
      serverClient.println(ESP.getFreeHeap());
    }
  }
  delay(10); // to avoid strange characters left in buffer
}


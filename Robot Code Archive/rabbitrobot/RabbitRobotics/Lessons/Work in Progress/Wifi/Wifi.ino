/*
 *  Simple HTTP get webclient test
 */

#include <ESP8266WiFi.h>
#include <WiFiManager.h>  
 
const char* ssid     = "RabbitGuest";
const char* password = "BeMyGuest";
 
const char* host = "wifitest.adafruit.com";
 
void setup() {
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status()); //3 Means the device is connected, 4 means the device failed to connect, 6 means that the device is just normally disconnected
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}
 
 
void loop() {

}

#include <WiFi.h>

const char *ssid= "ESP32";
const char *password = "pass1234";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Configuring access point...");
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.print(myIP);
}

void loop() {
  // put your main code here, to run repeatedly:

}

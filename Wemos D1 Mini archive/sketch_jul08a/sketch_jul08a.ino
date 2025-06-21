//found at:
// https://www.sysrun.io/2017/03/25/basic-d-duino-code-oled-wifimanager/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
 
#include "SH1106.h"
 
#define D1 5
#define D2 4

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
 
SH1106  display(0x3c, D1, D2);
 
// This is called if the WifiManager is in config mode (AP open)
void configModeCallback (WiFiManager *myWiFiManager) {
  display.clear();
  display.drawString(DISPLAY_WIDTH/2, 10, "Config Mode");
  // display the AP name
  display.drawString(DISPLAY_WIDTH/2, 30, myWiFiManager->getConfigPortalSSID());
  display.display();
}
 
void setup() {
  Serial.begin(115200);
  Serial.println("Boot");
 
  // Setup display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
 
  // Clear display buffer
  display.clear();
  // Write some text
  display.drawString(DISPLAY_WIDTH/2, 20, "Boot...");
  display.drawString(DISPLAY_WIDTH/2, 32, "Connecting to WiFi");
  // Send display buffer to display
  display.display();
 
  // Setup WifiManager
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  // The programm will stop here until the WiFi is configured
  wifiManager.autoConnect();
 
  // Wifi connected
  display.clear();
  display.drawString(DISPLAY_WIDTH/2, 20, "Setup done");
  display.drawString(DISPLAY_WIDTH/2, 32, "IP:" + WiFi.localIP().toString());
  display.display();
 
  // Setup ArduinoOTA
  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 - 10, "OTA Update");
    display.display();
  });
 
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // Draw nice progress bar
    display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
    display.display();
  });
 
  ArduinoOTA.onEnd([]() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, "Restart");
    display.display();
  });
}
 
 
void loop() {
  // Keep OTA running
  ArduinoOTA.handle();
}

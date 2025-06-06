 /*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
*/

//Compiles with ESP32 Core 1.0.6

  #include <WiFi.h>
   #include "src/AsyncTCP/AsyncTCP.h"

#include "src/ESPAsyncWebServer/ESPAsyncWebServer.h"
#include "src/WebSerial/WebSerial.h"
#include "src/AsyncElegantOTA/AsyncElegantOTA.h"
#include "src/AsyncWifiManager/WiFiManager.h" // https://github.com/tzapu/WiFiManager async branch
#include <ESPmDNS.h>


// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 39


AsyncWebServer server(80);
//WiFiManager wm;

int count;
String rec;

unsigned int  timeout   = 120; // seconds to run for
unsigned int  startTime = millis();
bool portalRunning      = false;
bool startAP            = false; // start AP and webserver if true, else start only webserver

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "reset") {count = 0;}
  
}

void setup(void) {
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  Serial.println("");
WiFiManager wm;
  // Wait for connection
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}

wm.setHostname("MDNSEXAMPLE");
  // wm.setEnableConfigPortal(false);
  // wm.setConfigPortalBlocking(false);
  bool res;
  res = wm.autoConnect();
 if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  //doWiFiManager();
  count++;
  WebSerial.print(count);
  WebSerial.println(" Hello World....");
  //Serial.println();
  delay(1000);
}

/*void doWiFiManager(){
  // is auto timeout portal running
  if(portalRunning){
    wm.process(); // do processing

    // check for timeout
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false;
      if(startAP){
        wm.stopConfigPortal();
      }
      else{
        wm.stopWebPortal();
      } 
   }
  }

  // is configuration portal requested?
  if(digitalRead(TRIGGER_PIN) == LOW && (!portalRunning)) {
    if(startAP){
      Serial.println("Button Pressed, Starting Config Portal");
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();
    }  
    else{
      Serial.println("Button Pressed, Starting Web Portal");
      wm.startWebPortal();
    }  
    portalRunning = true;
    startTime = millis();
  }
}
*/

//https://www.dfrobot.com/blog-1169.html

#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "";
const char* password =  "";

AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);

  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/test.html", "text/html");
  });

  server.on("/test.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/test.js", "text/javascript");
  });

  server.begin();
}

void loop(){}

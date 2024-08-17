//uses http://www.ourmanna.com/verses/api/

#include <WiFi.h>
#include <HTTPClient.h>
#include "src/M5StickC/M5StickC.h"
#include "src/ArduinoJson5/ArduinoJson.h" 
#include <DNSServer.h>

const char* ssid       = "";
const char* password   = "";
const String url = "https://beta.ourmanna.com/api/v1/get/?format=json&order=daily";
String payload, text, ref, ver, notice;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
Serial.begin(115200);
  M5.begin();
  setup_wifi();
  getJson(url);
  decodeJson();
  Serial.println(text);
  Serial.println(ref);
  Serial.println(ver);
  Serial.println(notice);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void getJson(String url) {
  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 0) { //Check the returning code
       payload = http.getString();   //Get the request response payload
     Serial.println(payload);
    }
 
    http.end();   //Close connection
 
  }
}

void decodeJson() {
 // const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 260;
//DynamicJsonBuffer jsonBuffer(capacity);
  
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //JsonObject& details = root["verse"]["details"];
  //const char* temp1 = details["text"];
  //const char* temp2 = details["reference"];
  //const char* temp3 = details["version"];
  //const char* temp4 = root["verse"]["notice"];
  text = root["verse"]["details"]["text"].as<String>(); //String(temp1);
  ref =  root["verse"]["details"]["reference"].as<String>(); //String(temp2);
  ver =  root["verse"]["details"]["version"].as<String>(); //String(temp3);
  notice = root["verse"]["notice"].as<String>(); //String(temp4);
  //Serial.println(temp1);
  //Serial.println(temp2);
  //Serial.println(temp3);
  //Serial.println(temp4);
  
}

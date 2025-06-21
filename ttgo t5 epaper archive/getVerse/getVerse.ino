//uses http://www.ourmanna.com/verses/api/

#include <WiFi.h>
#include <HTTPClient.h>
#include "src/epd/epd_driver.h"
#include "firasans.h"
#include "src/ArduinoJson5/ArduinoJson.h" 
#include <DNSServer.h>

const char* ssid = "";
const char* password = "";
const String url = "https://beta.ourmanna.com/api/v1/get/?format=json&order=daily";
String payload, text, ref, ver, notice;
uint8_t *framebuffer;

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
   
  epd_init();
  framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  setup_wifi();
  getJson(url);
  decodeJson();
  Serial.println(text);
  Serial.println(ref);
  Serial.println(ver);
  Serial.println(notice);
  char textArray[text.length()+1]; text.toCharArray(textArray, text.length()+1);
  char refArray[ref.length()+1];  ref.toCharArray(refArray, ref.length()+1);
  char verArray[ver.length()+1];  ver.toCharArray(verArray, ver.length()+1);
  char noticeArray[notice.length()+1]; notice.toCharArray(noticeArray, notice.length()+1); 
  //char *textArray= "The world and its desires pass away,\nbut the man who does the will of\nGod lives forever.";
  epd_poweron();
  epd_clear();
  epd_poweroff();
  int cursor_x = 0;
  int cursor_y = 100;
  epd_poweron();
  //writeln((GFXfont *)&FiraSans, textArray, &cursor_x, &cursor_y, NULL);
  write_string((GFXfont *)&FiraSans, textArray, &cursor_x, &cursor_y, NULL);
  cursor_x = 0;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, refArray, &cursor_x, &cursor_y, NULL);
  //cursor_x = 0;
  //cursor_y += 50;
  writeln((GFXfont *)&FiraSans, " ", &cursor_x, &cursor_y, NULL);
  writeln((GFXfont *)&FiraSans, verArray, &cursor_x, &cursor_y, NULL);
  cursor_x = 0;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, noticeArray, &cursor_x, &cursor_y, NULL);
  epd_poweroff();
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

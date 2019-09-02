/*
Copyright (c) 2019 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
#include <M5StickC.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <SPI.h>
#include <ESPmDNS.h>

 #define TFT_BLACK           0x0000
  #define TFT_BLUE            0x001F
  #define TFT_RED             0xF800
  #define TFT_GREEN           0x07E0
  #define TFT_CYAN            0x07FF
  #define TFT_MAGENTA         0xF81F
  #define TFT_YELLOW          0xFFE0
  #define TFT_WHITE           0xFFFF

 // Replace with your network credentials
const char* ssid     = "";
const char* password = "";

int gmtOffset_sec = 0;
String payload;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
WiFiClient espClient;

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

void updateNTP (void *pvParams);

void setup() {
  Serial.begin(9600);
M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(45, 0);
  M5.Lcd.println("Binary Clock");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(115, 12);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(95, 12);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(75, 12);
  M5.Lcd.println("4");
  M5.Lcd.setCursor(55, 12);
  M5.Lcd.println("8");
  M5.Lcd.setCursor(24, 12);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(8,37);
  M5.Lcd.println("32");
  M5.Lcd.setCursor(34,37);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(65,37);
  M5.Lcd.print("8");
  M5.Lcd.setCursor(125, 37);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(105, 37);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(85, 37);
  M5.Lcd.println("4");

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(8,65);
  M5.Lcd.println("32");
  M5.Lcd.setCursor(34,65);
  M5.Lcd.println("16");
  M5.Lcd.setCursor(65,65);
  M5.Lcd.print("8");
  M5.Lcd.setCursor(125, 65);
  M5.Lcd.println("1");
  M5.Lcd.setCursor(105, 65);
  M5.Lcd.println("2");
  M5.Lcd.setCursor(85, 65);
  M5.Lcd.println("4");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print (".");
  }
  Serial.println(WiFi.localIP());
 timeClient.begin();
  geolocation(); // guess where I am
 timeClient.setTimeOffset(gmtOffset_sec);
  xTaskCreate (updateNTP, "NTP Client", 4096, NULL, 2, NULL);
}

void loop() {
  M5.Rtc.GetTime(&RTC_TimeStruct);
M5.Lcd.setCursor(0, 15);
M5.Lcd.printf("Time: %02d : %02d : %02d\n",RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  delay(500);
}

void getJson(String url) {  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 0) { //Check the returning code
       payload = http.getString();   //Get the request response payload
     
    }
 
    http.end();   //Close connection
 
  }
}

int geolocation(){
  String url = "http://worldtimeapi.org/api/ip";
  getJson(url);
 
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonDocument jsonBuffer(1024);
  DeserializationError error = deserializeJson (jsonBuffer, payload);
  if (error){
      Serial.println ("deserializeJson () failed");
  }
 int raw_offset = jsonBuffer["raw_offset"];
 int dst_offset = jsonBuffer["dst_offset"];
 bool dst = jsonBuffer["dst"];
 if (dst == true) {
  gmtOffset_sec = raw_offset + dst_offset;
 } else { gmtOffset_sec = raw_offset; }
 
}

void updateNTP (void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
       setTime (timeClient.getEpochTime ());
       time_t t = now ();
       RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = hour(t);
  TimeStruct.Minutes = minute(t);
  TimeStruct.Seconds = second(t);
  M5.Rtc.SetTime(&TimeStruct);
   vTaskDelay ((1000/portTICK_PERIOD_MS) * 60 * 59);  // update every 59 minutes.
    }
}

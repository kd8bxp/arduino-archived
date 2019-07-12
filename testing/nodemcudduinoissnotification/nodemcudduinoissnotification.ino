#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "SSD1306.h"

float mylat = 39.360095;
float mylon = -84.58558;
float isslat, isslon;
int distance;
String payload;

const char* ssid = "";
const char* password = "";
const String iss = "http://api.open-notify.org/iss-now.json"; 

SSD1306  display(0x3c, D1, D2);

void setup() {
  Serial.begin(9600);
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  startWifi();

}

void loop() {
 
 getLocation();
  //Serial.println(payload);   //Print the response payload
 decodeJson();
 getDistance();
 issLocOLEDDisplay();
 issLocSerialDisplay();
 delay(5000);    //Send a request every 30 seconds
 
}
void issLocOLEDDisplay() {
  display.clear();
  display.drawString(0,0,"The ISS is currently at: ");
 char temp[15];
 sprintf(temp, "%d.%02d,%d.%02d",(int)isslat,abs((int)(isslat*100)%100),(int)isslon,abs((int)(isslon*100)%100));
 display.drawString(25,15,temp);
 char temp1[30];
 sprintf(temp1, "ISS is about %d miles", distance);
 display.drawString(0,27,temp1);
 display.drawString(30,38, "from you.");
 display.drawString(12,51, "And moving fast!!");
 display.display();
}

void issLocSerialDisplay() {
  Serial.print("The ISS is currently at ");
  Serial.print(isslat, 4); Serial.print(","); Serial.println(isslon,4);
  Serial.print("The ISS is about "); Serial.print(distance); Serial.println(" miles from you now.\nAnd moving fast!");
    
}

void getLocation() {
  
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin(iss);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 0) { //Check the returning code
       payload = http.getString();   //Get the request response payload
     
    }
 
    http.end();   //Close connection
 
  }
}

void startWifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("..");
 
  }
 Serial.println("");
}

void decodeJson() {
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  isslat=root["iss_position"]["latitude"];
  isslon=root["iss_position"]["longitude"];
}

void getDistance() {
  float theta, dist, miles;
  theta = mylon - isslon;
  dist = sin(deg2rad(mylat)) * sin(deg2rad(isslat)) + cos(deg2rad(mylat)) * cos(deg2rad(isslat)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  miles = dist * 60 * 1.1515;
  distance = miles;
}

float deg2rad(float n) {
  float radian = (n * 71)/4068;
  return radian;
}

float rad2deg(float n) {
  float degree = (n*4068)/71;
  return degree;
}


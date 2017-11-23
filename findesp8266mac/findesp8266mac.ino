#include <ESP8266WiFi.h>
 
void setup(){
 
   Serial.begin(9600);
   delay(500);
 Serial.println("Check ID in:");
  Serial.println("https://www.wemos.cc/verify_products");
  Serial.printf("Chip ID = %06X\n", ESP.getChipId());
  Serial.println(ESP.getChipId(),HEX);
  //Serial.println("");
   //Serial.println();
   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());
    uint32_t chipid=ESP.getChipId();
   
    char clientid[25];
    snprintf(clientid,25,"WIFI-Display-%08X",chipid);
    Serial.print("Client ID: ");
    Serial.println(clientid);
 
}
 
void loop(){}


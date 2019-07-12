#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
  #include "user_interface.h"
}
WiFiClient wifiClient;

String deviceMac;
char buffer[100];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
WiFi.mode(WIFI_STA);
  Serial.print("This node AP mac: "); Serial.println(WiFi.softAPmacAddress());
  Serial.print("This node STA mac: "); Serial.println(WiFi.macAddress());
initEspNow();
  
}

void loop() {
  // put your main code here, to run repeatedly:
initEspNow();
delay(1000);
yield();
}

void initEspNow() {
  if (esp_now_init()!=0) {
    Serial.println("*** ESP_Now init failed");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {

    deviceMac = "";
    deviceMac += String(mac[0], HEX);
    deviceMac += String(mac[1], HEX);
    deviceMac += String(mac[2], HEX);
    deviceMac += String(mac[3], HEX);
    deviceMac += String(mac[4], HEX);
    deviceMac += String(mac[5], HEX);
    
    memcpy(&buffer, data, sizeof(data));

    Serial.print("recv_cb, msg from device: "); Serial.println(deviceMac);
    //Serial.printf(" Temp=%0.1f, Hum=%0.0f%%, pressure=%0.0fmb\n", 
    //   sensorData.temp, sensorData.humidity, sensorData.pressure);    

    //haveReading = true;
    Serial.println(buffer);
  });
}


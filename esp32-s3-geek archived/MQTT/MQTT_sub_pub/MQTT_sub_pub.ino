#include <ArduinoJson.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#define MSG_BUFFER_SIZE (50)

// The name and password of the WiFi access point
#define STASSID "ESP32-S3-GEEK" // Fill in your WIFI name
#define STAPSK  "Waveshare" // Fill in your WIFI password

const char* mqtt_server = "mqtt.waveshare.cloud";
int PORT = 1883;

const char* ID = "";  // Defining device ID
char  pub[] = "";      // MQTT release topic
char  sub[] = "";      // MQTT subscribe to topics


StaticJsonDocument<400> sendJson;
StaticJsonDocument<400> readJson;

unsigned long lastUpdateTime = 0;
const char* ssid = STASSID;
const char* password = STAPSK;
char msg[MSG_BUFFER_SIZE];

WiFiClient espClient;
PubSubClient client(espClient);

const unsigned long updateInterval = 5000;

void setup() {
  Serial.begin(115200);

  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(BLACK);

  setup_wifi();
  client.setServer(mqtt_server, PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (millis() - lastUpdateTime > updateInterval) {  // Periodic data reporting
    sendJsonData();
    lastUpdateTime = millis();
  }
}

// MQTT subscribes to callback functions for processing received messages
void callback(char* topic, byte* payload, unsigned int length) {
  String inputString;
  for (int i = 0; i < length; i++) {
    inputString += (char)payload[i];
  }
  Serial.println(inputString);
  int dataBegin = inputString.indexOf("\"data\"");
  if (dataBegin == -1) {
    Serial.println(F("Missing 'data' field in JSON."));
    return;
  }
  int builtInBegin = inputString.indexOf("\"key\"", dataBegin); // Please change to your data identifier
  if (builtInBegin == -1) {
    Serial.println(F("Missing 'builtIn' field in 'data' object."));
    return;
  }
  int valueBegin = inputString.indexOf(':', builtInBegin);
  int valueEnd = inputString.indexOf('}', valueBegin);
  if (valueBegin == -1 || valueEnd == -1) {
    Serial.println(F("Invalid 'builtIn' value."));
    return;
  }
  String builtInValueStr = inputString.substring(valueBegin + 1, valueEnd);
  int builtInValue = builtInValueStr.toInt();
  if (builtInValue == 0) {
    LCD_Clear(BLACK);
    Paint_DrawString_EN(75, 55, "close!", &Font24, BLACK, GREEN);
    Serial.println("close!");
  } else {
    LCD_Clear(BLACK);
    Paint_DrawString_EN(75, 55, "open!", &Font24, BLACK, GREEN);
    Serial.println("open!");
  }
}

void setup_wifi() {
  Paint_DrawString_EN(20, 50, "Wifi Connecting...", &Font20, BLACK, GREEN);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "Wifi Connected", &Font20, BLACK, GREEN);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to the MQTT server
void reconnect() {
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "MQTT Connecting...", &Font20, BLACK, GREEN);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ID)) {
      Serial.println("connected");
      // Subscribe to the topic when the connection is successful
      client.subscribe(sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "MQTT Connected", &Font20, BLACK, GREEN);
}

// Send data in JSON format to MQTT server
void sendJsonData() {
  sendJson["ID"] = ID;
  String pubres;
  serializeJson(sendJson, pubres);
  int str_len = pubres.length() + 1;
  char char_array[str_len];
  pubres.toCharArray(char_array, str_len);
  client.publish(pub, char_array);
}
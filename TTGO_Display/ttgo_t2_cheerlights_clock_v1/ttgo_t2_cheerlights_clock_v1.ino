/*
  Cheerlights Clock for TTGO T2 board
 based on @martinbateman clock code
 https://t.co/1gRc56wNOE
 https://pastebin.com/4Ec6d4xY

ported to TTGO T2 Board by LeRoy Miller July 25, 2019

TODO: * Geolocate Timezone

*/


#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

#define sclk 14
#define mosi 13
#define cs   15
#define rst  4
#define dc   16

#define TFT_BL          -1  // Display backlight control pin
// Color definitions
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
const char* mqtt_server = "simplesi.cloud";

int cheer_red = 0;
int cheer_green = 0;
int cheer_blue = 0;
unsigned int rgb565Decimal = 0x8410;
unsigned int newrgb565Decimal;
String colourString = "";
String newColourString;

String strData;
String topicStr;


Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
WiFiClient espClient;
PubSubClient client(espClient);

SemaphoreHandle_t serialMutex = NULL;
void Task1 (void *pvParams);
void Task2 (void *pvParams);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  topicStr = topic;

  Serial.print("Message:");
  
  strData = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    strData += (char)payload[i];
  }
  
  Serial.println();
  Serial.println("-----------------------");

  if (topicStr.endsWith("cheerlights/rgb565Decimal")) {
   
    colourString = newColourString;
    rgb565Decimal = strData.toInt();
    Serial.println("*******");
  
    Serial.println(rgb565Decimal);
  }  
  if (topicStr.endsWith("cheerlights")) {
   
    newColourString = "\n" + strData + "                "; //newColourString = "Cheerlights:\n" + strData;
    //sixteenBitHex = newSixteenBitHex;
    Serial.println(strData);
  }  
} // end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("cheerlights",1);
      client.subscribe("cheerlights/rgb565Decimal",1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  serialMutex = xSemaphoreCreateMutex ();

  if (TFT_BL > 0) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  }
  
  tft.begin(); //tft.SSD1331_Init();
  //tft.setRotation(1);
  
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println (".");
  }
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(3600);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


  xTaskCreate (updateNTP, "NTP Client", 4096, NULL, 2, NULL);
  xTaskCreate (updateScreen, "Screen", 4096, NULL, 1, NULL);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void updateNTP (void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    if (xSemaphoreTake (serialMutex, (TickType_t)10) == pdTRUE) {
      while(!timeClient.update()) {
        timeClient.forceUpdate();
      }
      setTime (timeClient.getEpochTime ());
      xSemaphoreGive (serialMutex);
    }
    vTaskDelay ((1000/portTICK_PERIOD_MS) * 60 * 60);  // update every hour
  }
}

void updateScreen (void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    if (xSemaphoreTake (serialMutex, (TickType_t)10) == pdTRUE) {
      char timeString[25];
      char colourString2[25];
      colourString.toCharArray(colourString2,25);

      time_t t = now (); 
      sprintf (timeString, "%02i:%02i:%02i", hour (t), minute (t), second (t));
      xSemaphoreGive (serialMutex);
      tft.setTextSize(2);
      tft.setTextColor(0x39C4, TFT_BLACK);
      //tft.setCursor(0,0);
      //tft.print("88:88:88"); //tft.drawString("88:88:88",10,10,7);
      tft.setTextColor(rgb565Decimal, TFT_BLACK);
      tft.setCursor(0,0);
      tft.print(timeString); //tft.drawString (timeString, 10, 10, 7);
      tft.setTextSize(1);
      tft.setCursor(0,18);
      //tft.print(colourString2); //tft.drawString (colourString2, 0, 80, 4);
      tft.println("Cheerlights:");
      tft.setTextSize(2);
      tft.print(colourString2);
    }
    vTaskDelay (1000/portTICK_PERIOD_MS);
  }
}

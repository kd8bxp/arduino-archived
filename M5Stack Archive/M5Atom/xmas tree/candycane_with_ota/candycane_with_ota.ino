#include "src/FastLED/FastLED.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "";
const char* password = "";



#define DATA_PIN 22 // 27

#define COLOR_ORDER RGB //GRB
#define CHIPSET     WS2812
#define NUM_LEDS 50
#define BRIGHTNESS 30

CRGB leds[NUM_LEDS];

void setup() {
Serial.begin(115200);
  //M5.begin();
  //setup_wifi();
Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
   ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

/*void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

 while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}*/

void loop() {
  ArduinoOTA.handle();
     //for (int i=0; i<NUM_LEDS*5; i++) {
          rgwsequence();
          delay(25);
          ArduinoOTA.handle();
     //}
    /* for (int i=0; i<NUM_LEDS*5; i++) {
         rollingwheels();
         delay(25);
         ArduinoOTA.handle();
     }
     */
    // clearLEDs();
    // FastLED.show();
}

void clearLEDs()
{
  for (int i=0; i<NUM_LEDS; i++)
  {
    leds[i] = CRGB(0,0,0);
    //leds.setPixelColor(i, 0);
  }
}

void rgwsequence()
{
  int pause = 200;
  for(int j=0;j<20;j++)
  {
    
   for(int i=0;i<(NUM_LEDS-3);i=i+3)
   {
    leds[i] = CRGB(255,0,0);
    leds[i+1] = CRGB(0,255,0);
    leds[i+2] = CRGB(255,255,255);
      //leds.setPixelColor(i, RED);
      //leds.setPixelColor(i+1, GREEN);
      //leds.setPixelColor(i+2, WHITE);
      FastLED.show();
   }
   delay(pause);
   for(int i=0;i<(NUM_LEDS-3);i=i+3)
   {
      leds[i] = CRGB(255,255,255);
      leds[i+1] = CRGB(255,0,0);
      leds[i+2] = CRGB(0,255,0);
      //leds.setPixelColor(i, WHITE);
      //leds.setPixelColor(i+1, RED);
      //leds.setPixelColor(i+2, GREEN);
      FastLED.show();
   }
   delay(pause);
   for(int i=0;i<(NUM_LEDS-3);i=i+3)
   {
      leds[i] = CRGB(0,255,0);
      leds[i+1] = CRGB(255,255,255);
      leds[i+2] = CRGB(255,0,0);
      //leds.setPixelColor(i, GREEN);
      //leds.setPixelColor(i+1, WHITE);
      //leds.setPixelColor(i+2, RED);
      FastLED.show();
   }
   delay(pause);
  } 

  //clearLEDs();
  //FastLED.show();
  
}

// NOT YET
/*
void rollingwheels()  //assumes that you are using 12 LED ring neopixels
{
  int pause = 100;
  
  for(int k=0; k<96; k++)
  {
    
    for(int j=0; j<LED_COUNT/4; j++)
    {
      for(int i=0; i<4; i++)
      {
        leds.setPixelColor(((j*4)+i+k)%LED_COUNT, colorarray[(j%3)]);
        leds.show();
      }
    }  

  delay(pause);  
  
  }
  
  clearLEDs();
  leds.show();  
}
*/

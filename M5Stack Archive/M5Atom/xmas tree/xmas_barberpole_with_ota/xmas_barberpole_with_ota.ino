//main code found: https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
// Red, White, and Blue stripes with "glitter" flashes
// Mark Kriegsman, June 30, 2014
// requires FastLED v2.1 or later
//ota added,  and modified color pallet for more of a Xmas effect. Nov 10, 2021 - LeRoy Miller

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
#define NUM_LEDS   50
#define BRIGHTNESS 30

CRGB leds[NUM_LEDS];
uint8_t data[ NUM_LEDS];

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
      fill_data_array();
  render_data_with_palette();   
 // add_glitter();
  
  FastLED.show();
  FastLED.delay(50); //20);
}

void fill_data_array()
{
  static uint8_t startValue = 0;
  startValue = startValue + 2;
  
  uint8_t value = startValue;  
  for( int i = 0; i < NUM_LEDS; i++) {
    data[i] = triwave8( value); // convert value to an up-and-down wave
    value += 7;
  }
}

//original barber pole pattern
/*CRGBPalette16 gPalette ( 
    CRGB::Black, CRGB::Black, 
    CRGB::Red,   CRGB::Red,  CRGB::Red,  CRGB::Red, 
    CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
    CRGB::Blue,  CRGB::Blue, CRGB::Blue, CRGB::Blue,
    CRGB::Black, CRGB::Black
);*/

//xmas color (Red, Green) pattern
CRGBPalette16 gPalette ( 
    CRGB::Black, CRGB::Black, 
    CRGB::Red,   CRGB::Red,  CRGB::Red,  CRGB::Red, 
    CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
    CRGB::Green,  CRGB::Green, CRGB::Green, CRGB::Green,
    CRGB::Black, CRGB::Black
);

/*CRGBPalette16 gPalette ( 
    CRGB::Blue, CRGB::Blue, 
    CRGB::Red,   CRGB::Red,  CRGB::Black,  CRGB::Black, 
    CRGB::White,  CRGB::White, CRGB::Green, CRGB::Green,
    CRGB::Black,  CRGB::Black, CRGB::White, CRGB::White,
    CRGB::Black, CRGB::Black
);*/

/*CRGBPalette16 gPalette ( 
    CRGB::Blue, CRGB::Gray, 
    CRGB::Gray,   CRGB::Gray,  CRGB::Blue,  CRGB::Gray, 
    CRGB::Gray,  CRGB::Gray, CRGB::Blue, CRGB::Gray,
    CRGB::Gray,  CRGB::Gray, CRGB::Blue, CRGB::Gray,
    CRGB::Gray, CRGB::Gray
);*/

//This pattern needs to move at about 20
/*CRGBPalette16 gPalette ( 
    CRGB::Blue, CRGB::Gray, 
    CRGB::Gray,   CRGB::Gray,  CRGB::Gray,  CRGB::Gray, 
    CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
    CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
    CRGB::Gray, CRGB::Gray
);*/


void render_data_with_palette()
{
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( gPalette, data[i], 128, LINEARBLEND);
  }
}

void add_glitter()
{
  int chance_of_glitter =  5; // percent of the time that we add glitter
  int number_of_glitters = 3; // number of glitter sparkles to add
  
  int r = random8(100);
  if( r < chance_of_glitter ) {
    for( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS-1);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}

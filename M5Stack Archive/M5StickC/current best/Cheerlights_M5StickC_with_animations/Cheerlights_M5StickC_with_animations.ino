/* Copyright (c) 2017 LeRoy Miller
 *  
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

    Dec 24, 2019 - Modified "Cheerlights D1 mini 50 neopixel string center out" for use with
    M5StickC and Neoflash Hat - many changes, biggest is using cheerlights mqtt server
    and fastled now.
    Copyright (c) 2019 
 */

#include <WiFi.h>
#include "src/PubSubClient/PubSubClient.h"
//#include <Adafruit_NeoPixel.h>
#include "src/FastLED/FastLED.h"
#include "src/M5StickC/M5StickC.h"

#define BUILTIN_LED 10
// Update these with values suitable for your network.
#define NUM_LEDS 50 //37
//#define DATA_PIN 32 //stickC
#define DATA_PIN 22 //atomU
#define BRIGHTNESS          45

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "mqtt.cheerlights.com";

String color;
String previousColor;

int p = 45; //p = neopixel delay
int half = (NUM_LEDS /2);
int counter = 0;

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(126, 26, NEO_RGB + NEO_KHZ800);
CRGB leds[NUM_LEDS];

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  Serial.begin(115200);
  M5.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //strip.begin();
  //strip.show();
  //strip.setBrightness(brightness);
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  color = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    color+=(char)payload[i];
   
  }
  
  Serial.println();
  Serial.println("Color: " + color);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    uint32_t chipid=45367565498; //ESP.getChipId();
    char clientid[25];
    snprintf(clientid,25,"Cheerlights-%08X",chipid);
    if (client.connect(clientid)) {
            client.subscribe("cheerlights");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (color == "") {color = previousColor;}
   M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10,10);
    M5.Lcd.print(color);
setcolor();  
colorDisplay(0,0,0,p/2); //black

}

//colors from https://flaviocopes.com/rgb-color-codes/ (mostly)
void setcolor() {
   if (color == "white") {colorDisplay(255,255,255,p);
} else if (color == "warmwhite" || color == "oldlace" ) {colorDisplay(253,245,230,p); //has a cyan color to me.
} else if (color == "black" || color == "off") {colorDisplay(0,0,0,p);
} else if (color == "red") { colorDisplay(255,0,0,p);
} else if (color == "green") { colorDisplay(0, 255, 0, p);
} else if (color == "blue") { colorDisplay(0,0,255,p);
} else if (color == "cyan") {colorDisplay(0,255,255,p);
} else if (color == "magenta") {colorDisplay(255,0,255,p);
} else if (color == "yellow") {colorDisplay(255,220,0,p); //looks green/yellow to me
} else if (color == "purple") {colorDisplay(128,0,128,p); //colorDisplay(102,51,204,p); //looks more blue to me (maybe 160, 32, 240)
} else if (color == "orange") {colorDisplay(255,69,0,p); 
} else if (color == "pink") { colorDisplay(255, 105, 180,p);}
previousColor = color;
}


void colorDisplay(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  
  switch (random(0,4)) {
    case 0:
    colorDisplay0(r,g,b,wait);
    break;
    case 1:
    colorDisplay1(r,g,b,wait);
    break;
    case 2:
    colorDisplay2(r,g,b,wait);
    break;
    case 3:
    colorDisplay4(r,g,b,wait);
    break;
  }
  
}

//colorDisplay3 and colorDisplay2 are really the same, just done differently and one is speed up a little
void colorDisplay3(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
for (int i=0; i <= half; i++) { 
leds[half - i] = CRGB(r,g,b);
leds[i + half] = CRGB(r,g,b);
FastLED.show();
delay(wait);
}
}

void colorDisplay2(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (int i=0; i<=NUM_LEDS; i++) {
    if ((half+i)>NUM_LEDS) {leds[NUM_LEDS] = CRGB(r,g,b);} else {
    leds[half + i] = CRGB(r,g,b); }
    //leds[i+9+18] = CRGB(r,g,b);
    //leds[i+9+36] = CRGB(r,g,b);
    //leds[i+9+54] = CRGB(r,g,b);
    //leds[i+9+72] = CRGB(r,g,b);
    //leds[i+9+90] = CRGB(r,g,b);
    //leds[i+9+108] = CRGB(r,g,b);
   
    if ((half-i)<0) {leds[0] = CRGB(r,g,b); } 
    else {leds[half - i] = CRGB(r,g,b);}
    //leds[8 - i+18] = CRGB(r,g,b);
    //leds[8-i+36] = CRGB(r,g,b);
    //leds[8-i+54] = CRGB(r,g,b);
    //leds[8-i+72] = CRGB(r,g,b);
    //leds[8-i+90] = CRGB(r,g,b);
    //leds[8-i+108] = CRGB(r,g,b);    
  
  FastLED.show();
  delay(wait/2);
  }
}

void colorDisplay1(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (int i=0; i<=NUM_LEDS; i++) {
    leds[i] = CRGB(r,g,b);
    FastLED.show();
    delay(wait*1.2);
    leds[i] = CRGB(0,0,0);
   }
   for (int i=NUM_LEDS; i>=0; i--) {
    leds[i]=CRGB(r,g,b);
    FastLED.show();
    delay(wait*1.2);
    leds[i]=CRGB(0,0,0);
   }
}


void colorDisplay0(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (int y=0; y<=30; y++) {
  int ran[10];
  for (int i=0; i<=10; i++) {
    ran[i]=random(0,NUM_LEDS+1);
   // int ran1=random(0,NUM_LEDS+1);
  }
  
  for (int i=0; i<=3; i++) {
    leds[ran[i]] = CRGB(r,g,b);
    leds[ran[i+1]] = CRGB(r,g,b);
    leds[ran[i+2]] = CRGB(r,g,b);
    leds[ran[i+3]] = CRGB(0,0,0);
    leds[ran[i+4]] = CRGB(r,g,b);
    leds[ran[i+5]] = CRGB(r,g,b);
    FastLED.show();
    delay(wait);
    leds[ran[i]] = CRGB(0,0,0);
    leds[ran[i+1]] = CRGB(r,g,b);
    leds[ran[i+2]] = CRGB(0,0,0);
    leds[ran[i+3]] = CRGB(r,g,b);
    leds[ran[i+4]] = CRGB(0,0,0);
    leds[ran[i+5]] = CRGB(0,0,0);
    FastLED.show();
      
  }
  }
}

void colorDisplay4(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (int y=NUM_LEDS; y>=0; y--) {
    for (int i=0; i<=y; i++) {
      leds[i] = CRGB(r,g,b);
      FastLED.show();
      delay(wait/3);
      leds[i] = CRGB(0,0,0);
      FastLED.show();
    }
   leds[y]=CRGB(r,g,b);
   FastLED.show();
  }
}

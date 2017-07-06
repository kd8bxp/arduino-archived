/*
Raytracing on ILI9341 2.2" TFT LCD display
ported to regular Arduino, generic STM32 board ("blue pill") and ESP8266 NodeMCU V1
Pawel A. Hernik, 2016
Raytracing code found in the internet and adapted for my tests

Source code for my YouTube videos:
https://www.youtube.com/watch?v=RD5VO8o9bD4
https://www.youtube.com/watch?v=H9uUO-UQtzE
*/

/*
CONNECTIONS:
------------
STM32:
Board: STM32F103C
Upload via serial

For programming via serial:
  PA9 /TX to PC RX
  PA10/RX to PC TX
  3V3
  GND

 STM32 SPI1 pins:
  PA4 CS1
  PA5 SCK1
  PA6 MISO1
  PA7 MOSI1

  PA11 RST
  PA12 DC

TFT2.2 ILI9341 from top left:
  MISO  PA6
  LED   +3.3V
  SCK   PA5
  MOSI  PA7
  DC    PA12
  RST   PA11 or +3V3
  CS    PA4
  GND   GND
  VCC   +3.3V

ESP8266:
Board: ESP8266 NodeMCU, 80 or 160MHz
  
TFT2.2 ILI9341 from top left:
  MISO  D6 (HMISO/GPIO12)
  LED   +3.3V
  SCK   D5 (HSCLK/GPIO14)
  MOSI  D7 (HMOSI/GPIO13)
  DC    D4 (GPIO2)
  RST   RST or 3V3
  CS    D1 (GPIO5)
  GND   GND
  VCC   +3.3V
*/

/*
Rendering times:
----------------
AVR@16      320x240x1 1S 293s nodraw 280s/270s
AVR@16      320x240x1 8S 2213s

STM32@72    320x240x1 1S 52s nodraw 49s/51s
STM32@72    320x240x1 8S 403s

ESP8266@80  320x240x1 1S 65s nodraw 61s  (72s using slow lib)
ESP8266@160 320x240x1 1S 33s nodraw 30s  (37s using slow lib)
ESP8266@80  320x240x1 8S ??? reboot
ESP8266@160 320x240x1 8S 246s
*/

#include "SPI.h"

// Below ifdefs should work automaticaly but there must be a bug in Arduino 1.6.5 IDE because some conditions are wrong
// so in such case comment/uncomment below blocks manually

// STM32
//#if defined (__STM32F1__)
//#include <Adafruit_GFX_AS.h>
//#include <Adafruit_ILI9341_STM.h>
//#define TFT_CS  PA4                  
//#define TFT_DC  PA12              
//#define TFT_RST PA11 
//Adafruit_ILI9341_STM display = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST);
//#endif

// Arduino
//#if defined (__AVR__)
//#include <Adafruit_GFX.h>
//#include <Adafruit_ILI9341.h>
//#define TFT_DC 5
//#define TFT_CS 4
//// RST connected to 3.3V to save pins
//Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC);
//#endif

// ESP8266
#ifdef ESP8266
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_DC 2
#define TFT_CS 5
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC);
#endif

#define RGBTO565(_r, _g, _b) ((((_r) & B11111000)<<8) | (((_g) & B11111100)<<3) | ((_b) >>3))
#include "raytracer.h"

void setup() {
//  Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("ILI9341 raytracing"); 
 
  display.begin();
  display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  display.setTextSize(1);
    
  // read diagnostics (optional but can help debug problems)
  uint8_t x = display.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = display.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = display.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = display.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = display.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  Serial.print("ScreenWidth:  "); Serial.println(display.width()); 
  Serial.print("ScreenHeight: "); Serial.println(display.height()); 

  display.setRotation(1);
  display.fillScreen(0);
  
  unsigned long t = millis();
  doRaytrace(1,320,240,1); // full 320x240 1 sample
//  doRaytrace(1,320,240,4); // fast preview
//  doRaytrace(1,320,240,2); // slower preview
//  doRaytrace(1,160,120,2); // quarter of the screen
//  doRaytrace(8);  // very high quality 320x240 antialiased, 8 samples
  Serial.println(F("Done!"));
  Serial.print(F("Time = "));
  Serial.print((millis() - t)/1000);
  Serial.println(F(" seconds"));
}


void loop(void) {
}


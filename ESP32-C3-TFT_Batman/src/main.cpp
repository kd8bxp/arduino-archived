#include <Arduino.h>
#include <TFT_eSPI.h>
#include "xbm.h"

TFT_eSPI tft;
TFT_eSprite drawBuf(&tft);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  tft.begin();
  //drawBuf.setColorDepth(16);
  //drawBuf.createSprite(logoWidth, logoHeight);
  drawBuf.setTextFont(1);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
 
  drawBuf.createPalette(palette, 16);
  tft.pushImage(0,0, 160, 80, (const uint16_t *)myGraphic);
  
  //drawBuf.pushSprite(tft.width() / 2 - logoWidth / 2, tft.height() / 2 - logoHeight / 2);
  drawBuf.pushSprite(0,0);
}

void loop() {}

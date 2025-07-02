// use: https://lang-ship.com/tools/image2data/
// 16 bit RGB565 Big Endian

#include "1hal9000.h"             
#include "2hal9000.h"
#include "3hal9000.h"
#include "4hal9000.h"
#include "5hal9000.h"
#include "6hal9000.h"
#include "M5Dial.h"        // Hardware-specific library

#define waitTime 250

void setup() {

  M5.begin();               // Initialise the display

  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
}

void loop() {

  
  M5.Lcd.drawBitmap(0, 0, img1Width, img1Height, img1);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img2Width, img2Height, img2);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img3Width, img3Height, img3);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img4Width, img4Height, img4);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img5Width, img5Height, img5);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img6Width, img6Height, img6);
  delay(waitTime); 
  M5.Lcd.drawBitmap(0, 0, img5Width, img5Height, img5);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img4Width, img4Height, img4);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img3Width, img3Height, img3);
  delay(waitTime);
  M5.Lcd.drawBitmap(0, 0, img2Width, img2Height, img2);
  delay(waitTime);
  
}

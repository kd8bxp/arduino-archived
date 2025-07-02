/*
 * use: https://lang-ship.com/tools/image2data/
 * 16Bit RGB565 Dump (big endian)
 * 
 * Export JPG with no optimization, and turn progressive off 
 */


#include "camera.c"         
#include "fingerTouch.c"
#include "gMail.c"
#include "journal.c"
#include "noTouch.c"
#include "print.c"
#include "share.c"
#include "calc.c"

#include "src/M5AtomS3/M5AtomS3.h"        // Hardware-specific library

int rotate = 1;

void setup() {

  M5.begin(true, true, false, false);         // Initialise the display
  M5.Lcd.setRotation(2);
 // M5.Axp.ScreenBreath(9);
  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
  rotateIcon(rotate);
}

void loop() {
  
  
 if (M5.Btn.pressedFor(1000) && M5.Btn.wasReleased() == false) {
    rotate++;
    if (rotate > 8) {rotate = 1;}
    rotateIcon(rotate);
    delay(500);
    M5.update();
    goto bailout;
  }  
  
  // M5.update();
  if (M5.Btn.lastChange() > 2000 && M5.Btn.wasReleased() /*M5.Btn.wasReleased()*/) { 
    //M5.update();
    //if (M5.Btn.wasReleased()) {
    USBSerial.println(rotate); } //do button push
  //} 
  bailout:
    delay(1);
   M5.update();
}

void rotateIcon(int s) {

  switch (s) {

    case 1:
    M5.Lcd.drawBitmap(20, 20, journalWidth, journalHeight, journal);
  break;
    case 2:
    M5.Lcd.drawBitmap(20, 20, cameraWidth, cameraHeight, camera);
  break;
    case 3:
  M5.Lcd.drawBitmap(20, 20, fingerTouchWidth, fingerTouchHeight, fingerTouch);
  break;
    case 4:
  M5.Lcd.drawBitmap(20, 20, gMailWidth, gMailHeight, gMail);
  break;
    case 5:
  M5.Lcd.drawBitmap(20, 20, noTouchWidth, noTouchHeight, noTouch);
  break;
    case 6:
  M5.Lcd.drawBitmap(20, 20, printerWidth, printerHeight, printer);
  break;
    case 7:
  M5.Lcd.drawBitmap(20, 20, shareWidth, shareHeight, share);
  break;
    case 8:
  M5.Lcd.drawBitmap(20, 20, calcWidth, calcHeight, calc);
  break;
    default:
    M5.Lcd.drawBitmap(20, 20, journalWidth, journalHeight, journal);
  break;
 }
}

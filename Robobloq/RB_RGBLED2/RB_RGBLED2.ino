#include "RB_RGBLED.h"
RB_RGBLed RGBLED(2,2);
void setup() {
 
}

void loop() {
  // put your main code here, to run repeatedly:
 // put your setup code here, to run once:
   RGBLED.setColor(1,50,0,0);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(1,0,50,0);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(1,0,0,50);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(1,0,0,0);
   RGBLED.show();
   delay(100);
   
    RGBLED.setColor(2,50,0,0);
    RGBLED.show();
   delay(100);
   RGBLED.setColor(2,0,50,0);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(2,0,0,50);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(2,0,0,0);
   RGBLED.show();
   delay(100);
   
    RGBLED.setColor(0,50,0,0);
    RGBLED.show();
   delay(100);
   RGBLED.setColor(0,0,50,0);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(0,0,0,50);
   RGBLED.show();
   delay(100);
   RGBLED.setColor(0,0,0,0);
   RGBLED.show();
   delay(100);
}

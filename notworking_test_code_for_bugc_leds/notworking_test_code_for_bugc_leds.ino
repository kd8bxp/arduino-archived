
#include <M5StickC.h>
#include <WiFi.h>

void setup() {
  // put your setup code here, to run once:
 M5.begin();
    M5.update();
  Wire.begin(0,26,10000);
  //Led(6,  0, 0, 150);
Led(0xC4,  0, 0, 100);
delay(5000);
Led(0xC4, 0, 0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void Led(int indexOfLED, int r, int g, int b){
  Wire.beginTransmission(0x38);
  Wire.write(indexOfLED);
  
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();
}

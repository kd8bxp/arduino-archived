#include "DFRobot_HT1632C.h"

#if defined( ESP_PLATFORM ) || defined( ARDUINO_ARCH_FIREBEETLE8266 )  //FireBeetle-ESP32 FireBeetle-ESP8266
#define DATA D6
#define CS D2
#define WR D7
//#define RD D8
#else
#define DATA 6
#define CS 2
#define WR 7
//#define RD 8
#endif

DFRobot_HT1632C ht1632c = DFRobot_HT1632C(DATA, WR,CS);

char str[] = "LeRoy Miller, KD8BXP ";

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
  ht1632c.setFont(FONT8X4);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  ht1632c.print(str,25);
  //delay(100);
}

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

//make a wink effect using just the letter "O" with different font sizes
char str[] = "O";

void setup() {
  // put your setup code here, to run once:
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
	ht1632c.setCursor(6,0);
  ht1632c.print(str);
  ht1632c.setCursor(12,0);
  ht1632c.print(str);
}

void loop() {
  ht1632c.clearScreen();
  ht1632c.setFont(FONT5X4);
  ht1632c.setCursor(6,3);
  ht1632c.print(str);
  ht1632c.setFont(FONT8X4);
  ht1632c.setCursor(12,0);
  ht1632c.print(str);
  delay(250);
  ht1632c.clearScreen();
  ht1632c.setFont(FONT8X4);
  ht1632c.setCursor(6,0);
  ht1632c.print(str);
  ht1632c.setCursor(12,0);
  ht1632c.print(str);
  delay(250);
  
}

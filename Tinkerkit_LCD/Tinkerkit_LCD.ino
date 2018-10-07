/**
* Simple Hello World program. To be run on an Arduino connected
* to a TinkerKit LCD module via Serial.
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <TKLCD.h>

//TKLCD_Serial lcd = TKLCD_Serial(); // when connecting to TKLCD over Serial
TKLCD_Local lcd = TKLCD_Local(); // when programming a TKLCD module itself

int wait = 250;

void setup() {
  lcd.begin();
  
  lcd.clear();
  
  lcd.print("Hello World!");
  
  //lcd.setCursor(0,1);
  
  //lcd.print("Again!");
}

void loop() {

for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
	lcd.scrollDisplayLeft();
	delay(wait);
}

for (int positionCounter =0; positionCounter < 29; positionCounter++) {
	lcd.scrollDisplayRight();
	delay(wait);
	
}
for (int positionCounter = 0; positionCounter <16; positionCounter++) {
	lcd.scrollDisplayLeft();
	delay(wait);
}

}
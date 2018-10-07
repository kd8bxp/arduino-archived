/**
* Simple Hello World program. To be run on an Arduino connected
* to a TinkerKit LCD module via Serial.
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <TKLCD.h>
#include <TinkerKit.h>

//TKLCD_Serial lcd = TKLCD_Serial(); // when connecting to TKLCD over Serial
TKLCD_Local lcd = TKLCD_Local(); // when programming a TKLCD module itself
TKThermistor therm(A0);
TKPotentiometer pot(A1);
TKPotentiometer slide(A2);
TKLed led(D6);

int wait = 250;
float C, F;
int contrast;
int brightness;

void setup() {
  lcd.begin();
  
  lcd.clear();
  
  lcd.print("Hello World!");
  
  //lcd.setCursor(0,1);
  
  //lcd.print("Again!");
}

void loop() {
/*
for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
	lcd.scrollDisplayLeft();
	digitalWrite(6, HIGH);
	delay(wait);
}

for (int positionCounter =0; positionCounter < 29; positionCounter++) {
	lcd.scrollDisplayRight();
	digitalWrite(6, LOW);
	delay(wait);
}

for (int positionCounter = 0; positionCounter <16; positionCounter++) {
	lcd.scrollDisplayLeft();
	digitalWrite(6, HIGH);
	delay(wait);
}
*/
//lcd.clear();

brightness = slide.read();
lcd.setBrightness(brightness/4);

lcd.setCursor(0,0);
lcd.print("Tempature: ");
F = therm.readFahrenheit();
lcd.setCursor(11,0);
lcd.print(F);
digitalWrite(6, HIGH);
delay(1000);
contrast = map(pot.read(),0,1023,175,255);
lcd.setContrast(contrast);
lcd.setCursor(0,1);
lcd.print("Contrast: ");
lcd.setCursor(11,1);
lcd.print(contrast);
digitalWrite(6, LOW);
delay(1000);
}
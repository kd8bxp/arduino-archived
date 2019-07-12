/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

Features of this project:
 .. Prints "Rabbit Laser USA" to the LCD
 .. Shows the seconds elapsed since reset 
 .. Flashing the BackLight
 .. Setting character position for writing to the display
 .. Flashing the light without a time delay.

  The circuit:
 * LCD VSS connected to GND (0vdc)
 * LCD VDD connected to +5VDC
 * LCD VEE connected to gnd (0vdc) .. Prefer to connect to 5K potentiometer as to adjust the contrast of the display
 * LCD RS pin to digital pin 8
 * LCD R/W pin connected to digital GND (0vdc)
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * compatible with keypad shield
 * 
 * Often the Anode of the Backlight is LCD pin 15 and should be connected to +5vdc when backlight is to be ON.
 *    We connect the backlight to Arduino pin 10, but this pin may not deliver enough current to produces a bright background.
 * Often the Cathode of the Backlight is LCD pin 16 and should be connected to the circuit GND.
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified for compatible with keypad shield  .. Ray Scott 9 June 2016 .. RabbitRobots.com


 This example code is in the public domain.
 */

// include the library code:
#include <LiquidCrystal.h>

int BackLight = 10;  // The backlight is set to use PIN 10
int seconds = 0;  // Set the number of counted seconds to Zero
  
  // initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RabbitRobots.com");

  pinMode(BackLight, OUTPUT);
}

void loop() {
  // set the cursor to column 7, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(7, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  
  // check if the seconds is an even number..
   seconds = (millis() / 1000) % 2;  // modulus math to identify an even number
  if (seconds == 1)
  {
    // turn on backlight  
    digitalWrite(BackLight, HIGH);
  }
  else
  {
    // turn off backlight
    digitalWrite(BackLight, LOW);
  }
}


/*
 
Demonstrates the use a 16x2 LCD display using typical HD44780 driver. 



Features of this project:
 .. Prints "RabbitRobots.com" to the LCD
 .. Shows the seconds elapsed since reset 
 .. Setting character position for writing to the display
 .. Uses timed slices to ensure faster process cycles... No "delay" commands
 .. Reads analog value from the IR sensor
 .. Writes the Sensor values to the LCD display
 
* IR Sensor #1 .. analog Pin 8
* The trigger is an IR transmitter LED. 
* IR from sunlight and other ambient light sources can distort readings.
* I recommend to use the standard deviation of all available sensors and then derive which sensor is getting the lower/best readings.
** Value is lower when sees IR light
** The IR transmitter can be power hungry.

  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD BackLight to Pin 10
 * compatible with keypad shield
 
 .. Ray Scott 9 June 2016 .. RabbitRobots.com

 */

// include the library code:
#include <LiquidCrystal.h>
int BackLight = 10;
int seconds = 0;
int sensorValue = 0;
boolean ReadSensorsTS = true;

int AnSensor1 = 8; 

  // initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RabbitRobots.com");

  pinMode(BackLight, OUTPUT);
    // turn off backlight  
    digitalWrite(BackLight, LOW);

}

void loop() {
  // set the cursor to column 7, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(12, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000); // writes to the LCD on EVERY cycle of the "void loop"
// be careful that this item could slow down the loop code's cycle time
 
  // check if the seconds is an even number..
   seconds = (millis() / 250) % 2;  // modulus math to get time slices
  if (seconds == 1 && ReadSensorsTS==true) // only run once per time slice
  {
    ReadSensorsTS = false;
    
    sensorValue = analogRead(AnSensor1);     // read the input on analog pin A0:
    lcd.setCursor(3, 1); // clear the LCD space for 4 characters from int value
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print(sensorValue); // print value to LCD


  }
  else
  {
    ReadSensorsTS = true;
  }

// end of code  
}

 


 

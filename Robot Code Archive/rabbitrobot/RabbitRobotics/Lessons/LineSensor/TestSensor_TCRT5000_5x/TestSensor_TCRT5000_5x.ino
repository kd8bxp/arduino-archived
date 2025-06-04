/*
 
Demonstrates the use a 16x2 LCD display using typical HD44780 driver. 
Intended for testing with Mega 2560


Features of this project:
 .. Prints "5xSens" to the LCD
 .. Shows the seconds elapsed since reset 
 .. Setting character position for writing to the display
 .. Uses timed slices to ensure faster process cycles... No "delay" commands
 .. Reads analog value from the TCRT5000 sensor
 .. Reads the digital signal from the TCRT5000 sensor
 .. Writes the Sensor values to the LCD display
 
* IR Sensor #1 .. analog Pin 8, Digital Pin 44
* IR Sensor #2 .. analog Pin 9, Digital Pin 45
* IR Sensor #3 .. analog Pin 10, Digital Pin 46
* IR Sensor #4 .. analog Pin 11, Digital Pin 47
* IR Sensor #5 .. analog Pin 12, Digital Pin 48
 ... If the analog pin is not actually connected, the value being read will be effected by many factors.
 ... It is recommended to connect the unused Analog Pins to a GND .. 0VDC



* The target should be a white piece of paper with a black line.  
* The thickness of the lines is significant to block reflection of the IR Light. Recommend 3/8" thick line.
* The black line should be due to a Permanent Marker. An ink pen stroke will reflect the IR.
* Using TCRT500 sensors, the adjustment potentiometer only sets the threshold of 
   when the TCRT5000 will activate the digital signal. The Potentiometer does not effect the analog value.
 
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
int BoolSensor1 = 44;
int AnSensor2 = 9; 
int BoolSensor2 = 45;
int AnSensor3 = 10; 
int BoolSensor3 = 46;
int AnSensor4 = 11; 
int BoolSensor4 = 47;
int AnSensor5 = 12; 
int BoolSensor5 = 48;

  // initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("5xSens");

  pinMode(BackLight, OUTPUT);
    // turn on backlight  
    digitalWrite(BackLight, HIGH);
  pinMode(BoolSensor1, INPUT);

}

void loop() {
  // set the cursor to column 7, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(12, 1);
  // print the number of seconds since reset:
//  lcd.print(millis() / 1000); // writes to the LCD on EVERY cycle of the "void loop"

 
  // check if the seconds is an even number..
   seconds = (millis() / 250) % 2;  // modulus math to get time slices
  if (seconds == 1 && ReadSensorsTS==true) // only run once per time slice
  {
    ReadSensorsTS = false;

    sensorValue = analogRead(AnSensor1);     // read the input on analog pin 0:
    lcd.setCursor(3, 1);  // go to LCD location
    lcd.print(" ");  // clear the LCD space for 4 characters from int value
    lcd.setCursor(0, 1); // goto to LCD location
    lcd.print(sensorValue);     // print the value from analog Sensor:

//    lcd.setCursor(5, 1);
    // print the Sensor State:
//    lcd.print(digitalRead(BoolSensor1)); // low value indicates Sensor LED is ON and dark line is not detected.

    sensorValue = analogRead(AnSensor2);     // read the input on analog pin 0:
    lcd.setCursor(7, 1);  // go to LCD location
    lcd.print(" ");  // clear the LCD space for 4 characters from int value
    lcd.setCursor(4, 1); // goto to LCD location
    lcd.print(sensorValue);     // print the value from analog Sensor:

    sensorValue = analogRead(AnSensor3);     // read the input on analog pin 0:
    lcd.setCursor(11, 1);  // go to LCD location
    lcd.print(" ");  // clear the LCD space for 4 characters from int value
    lcd.setCursor(8, 1); // goto to LCD location
    lcd.print(sensorValue);     // print the value from analog Sensor:

    sensorValue = analogRead(AnSensor4);     // read the input on analog pin 0:
    lcd.setCursor(15, 1);  // go to LCD location
    lcd.print(" ");  // clear the LCD space for 4 characters from int value
    lcd.setCursor(12, 1); // goto to LCD location
    lcd.print(sensorValue);     // print the value from analog Sensor:

    sensorValue = analogRead(AnSensor5);     // read the input on analog pin 0:
    lcd.setCursor(15, 0);  // go to LCD location
    lcd.print(" ");  // clear the LCD space for 4 characters from int value
    lcd.setCursor(12, 0); // goto to LCD location
    lcd.print(sensorValue);     // print the value from analog Sensor:


  }
  else
  {
    ReadSensorsTS = true;
  }

// end of code  
}

 


 

/******************************************
 * 
 * This program is meant to test using the TCRT5000 and other line followers with an LCD screen and an Arduino UNO.
 * The LCD screen should display the brightness of the object that the line folllower is looking at. 
 * Higher values represent a darker surface and lower values represent a lighter one.
 * For the TCRT5000, the sensor is effective 0.5-22mm away from the surface and works optimally at 2.5mm away.
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <LiquidCrystal.h> //Library for using lcd displays

/*-----( Define constants )-----*/
#define lcdRows 2 //The number of rows and columns on the lcd screen
#define lcdCols 16

/*-----( Declare objects )-----*/
//Setup lcd display in 4bit parallel mode ( rs, enable, d0, d1, d2, d3).
//If your lcd is not displaying any characters or is printing garbage data,
//check your connections and check with the manufacturer for the correct pins to use on your lcd screen.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

void setup() { /*----( SETUP: RUNS ONCE )----*/

  lcd.begin(lcdRows, lcdCols); //Start using the lcd screen
  lcd.setCursor(0,0); //Set cursor for the LCD in the upper-left corner 
  lcd.print("Darkness"); //Display a label for the output
  pinMode(10, INPUT); //Setup digital pin 10 to recieve the data from the line follower\/* --(end main loop )-- *
  
}
/*--(end setup )---*/

void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  if(millis() % 100 == 0){ //Do the following every 10th of a second:
    lcd.setCursor(0,lcdRows - 1);//Set the lcd cursor to the bottom-left corner
    lcd.print(analogRead(1)); //Print out the analog value receieved from the line follower
    lcd.print("   "); //Print whitepsace next to the analog value to remove extra characters to the right
    lcd.print(digitalRead(10)); //Print out the digital value receieved from the line follower
    lcd.print("   "); //Print whitepsace next to the digital value to remove extra characters to the right
  }
}
/* --(end main loop )-- */

/******************************************
 * 
 * This program is meant to test the functionality of an ultrasonic sensor using an lcd keypad shield and an Arduino UNO.
 * The echo duration in microseconds along with the distance in inches are printed to the lcd
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <LiquidCrystal.h> //Library for using lcd displays\

/*-----( Define constants )-----*/
#define SPEED_OF_SOUND_MPH 776.5 //Used for conversion from the echo delay in microseconds to distance in inches
#define INCHES_PER_MILE  63360
#define MICROS_PER_HOUR 3600000000

#define TRIGGER_PIN 11 //Replace if you use different tigger and echo ports
#define ECHO_PIN 10

#define LCD_ROWS 2 //The number of rows and columns on the lcd screen
#define LCD_COLS 16

/*-----( Declare variables )-----*/
unsigned long echoDuration= 0; //Used to store the delay of the echo in microseconds
float inches = 0; //Used to store the distance from the ultrasonic sensor to the object it's pointing at in inches


/*-----( Declare objects )-----*/
//Setup lcd display in 4bit parallel mode ( rs, enable, d4, d5, d6, d7).
//Digital pin numbers on the audrino should match up to the digital pins on the lcd.
//If your lcd is not displaying any characters or is printing garbage data,
//check your connections and check with the manufacturer for the correct pins to use on your lcd screen.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

void setup() {/*----( SETUP: RUNS ONCE )----*/

lcd.begin(LCD_ROWS, LCD_COLS); //Start using the lcd screen

pinMode(TRIGGER_PIN, OUTPUT); //Setup trigger and echo ports for the ultrasonic sensor to use 
pinMode(ECHO_PIN, INPUT);

}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  //Begin ultrasound pulse to be echoed 
  digitalWrite(TRIGGER_PIN, HIGH);

  //Wait 10 microseconds (Delay the program 10 microseconds to create a 10 millisecond pulse
  delayMicroseconds(10);

  //End ultrasound pulse
  digitalWrite(TRIGGER_PIN, LOW);

  //Get the length of the pulse returning from the echo pin in microseconds
  //The length of this pulse corresponds to the delay from the ultrasound pulse being sent 
  //to the echo being recieved by the sensor
  echoDuration = pulseIn(ECHO_PIN, HIGH);

  inches = (float) echoDuration / MICROS_PER_HOUR;  //Convert the pulse length in microseconds to hours
  inches = inches * SPEED_OF_SOUND_MPH; //Convert the time it took for the pulse to be sent and recieved into distance in miles
  inches = inches * INCHES_PER_MILE; //Convert distance travelled from miles to inches
  inches = inches / 2; //Divide the final distance by two since the pulse originally crossed the space between sensor and object twice 
  
  lcd.setCursor(0, 0);//Set the lcd cursor at the upper-left part of the screen

  lcd.print("Ping: ");//Print out the echo delay to the lcd screen labelled "Ping"
  lcd.print(echoDuration);
  lcd.print("        ");//Clear any extra decimals to the right
  
  lcd.setCursor(0, LCD_ROWS - 1);//Set the lcd cursor at the lower-left corner of the screen
  
  lcd.print("Inches: ");//Print out the calculated distance in inches to the lcd screen labelled "Inches"
  lcd.print(inches);
  lcd.print("   ");//Clear any extra decimals to the right

}

/* --(end main loop )-- */



/* ( End ) */

/******************************************
 * 
 * This program is meant to test 180-degree servo functionality using an LCD keypad shield.
 * The servo's position is incremented and decremented using the left, right, up, and down keys on the LCD keypad.
 * 
 * Uses a LCD Keypad shield, an Arduino UNO, and a simple 180-degree servo motor
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <LiquidCrystal.h> //Library for using lcd displays
#include <Servo.h> //Library for using servo motors

/*-----( Define constants )-----*/
#define KEY_SELECT 0 //Constants representing each readable key on the lcd keypad shield
#define KEY_LEFT 1
#define KEY_UP 2
#define KEY_RIGHT 3
#define KEY_DOWN 4
#define KEY_NONE 5 

#define LCD_ROWS 2 //The number of rows and columns on the lcd screen
#define LCD_COLS 16

/*-----( Declare variables )-----*/
int keyPressed = KEY_NONE; //Stores the key being currently pressed
int lastKeyPressed= KEY_NONE; //Stores the last key that was pressed
int servoPosition = 0; //Used to determine the postion of the servo.

/*-----( Declare objects )-----*/
//Setup lcd display in 4bit parallel mode ( rs, enable, d4, d5, d6, d7).
//Digital pin numbers on the audrino should match up to the digital pins on the lcd.
//If your lcd is not displaying any characters or is printing garbage data,
//check your connections and check with the manufacturer for the correct pins to use on your lcd screen.
//Do not attatch the lcd screen directly to the arduino, as we need a digital pin open for a servo later
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

Servo myServo;//Declare the servo that we will be testing with the program.

void setup() { /*----( SETUP: RUNS ONCE )----*/
  
lcd.begin(LCD_ROWS, LCD_COLS); //Start using the lcd screen

pinMode(3, OUTPUT);//Setup servo on digital port 3
myServo.attach(3);
}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  //Get the current key being pressed on the LCD screen if any every four milliseconds
  if(millis() % 4 == 0){
    keyPressed = currentKey(); 
  }

  if(keyPressed == KEY_UP && lastKeyPressed != keyPressed){ //Do the following if the up key is pressed once
    servoPosition += 10; //Increment the servo position by ten
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  } 

  else if(keyPressed == KEY_DOWN && lastKeyPressed != keyPressed){ //Do the following if the down key is pressed once
    servoPosition -= 10; //Decrement the servo positon by ten
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }
  
  else if(keyPressed == KEY_LEFT && lastKeyPressed != keyPressed){ //Do the following if the left key is pressed once
    servoPosition--; //Decrement the servo positon by one
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }
  
  else if(keyPressed == KEY_RIGHT && lastKeyPressed != keyPressed){ //Do the following if the right key is pressed once
    servoPosition++; //Increment the servo position by one
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }

  else if(lastKeyPressed != keyPressed){
    lastKeyPressed = keyPressed; //If none of the used keys were pressed, but the key pressed still changed, update lastKeyPress
  }

  servoPosition = constrain(servoPosition, 0, 180); //Limit the position to those possible for the servo

  lcd.setCursor(0,0); //Set cursor for the LCD in the upper-left corner 

  //Print out the the servo position to the lcd screen.
  //It is important not to use println() as not all lcd screens understand how to print newline and carriage return
  //The whitepasce at the end clears any remaining digits to the right of servoPosition
  lcd.print("ServoPos: ");
  lcd.print(servoPosition);
  lcd.print("  ");

  //Set the servo to the determined position
  myServo.write(servoPosition);
  
}

/* --(end main loop )-- */



//Function for returning the current key on the keypad being held down.
//The ranges used can vary between lcd screens, but the ones included shoudl work with most lcd screens.
//If they don't, try running LCDTest to determine the proper voltage thesholds
int currentKey(){ 

  int inputVoltage = analogRead(0); //Get the voltage output from the lcd's analog port 0.

  //If the value from the lcd is above 800, return that no button is being pressed.
  if(inputVoltage > 800){
    return KEY_NONE;
  } 

 //If the value from the lcd is at 800 or lower and above 700, return that no button is being pressed.
  else if(inputVoltage <= 800 && inputVoltage > 700){
     return KEY_SELECT;
  } 

  //If the value from the lcd is at 700 or lower and above 500, return that no button is being pressed.
  else if(inputVoltage <= 700 && inputVoltage > 500){
    return KEY_LEFT;
  } 

  //If the value from the lcd is at 500 or lower and above 320, return that no button is being pressed.
  else if(inputVoltage <= 500 && inputVoltage > 320){
    return KEY_DOWN;
  } 

  //If the value from the lcd is at 320 or lower and above 80, return that no button is being pressed.
  else if(inputVoltage <= 320 && inputVoltage > 80){
    return KEY_UP;
  } 

  //If the value from the lcd is at 80 or under, return that no button is being pressed.
  else if(inputVoltage <= 80){
    return KEY_RIGHT;
  } 
  
}


/* ( End ) */

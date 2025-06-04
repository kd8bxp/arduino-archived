/******************************************
 * 
 * This program is meant to test using the LCD keypad shield for hte Arduino UNO.
 * It is reccomeneded to run this test first, as many of other hardware component tests rely on the LCD keypad shield 
 * for input and output. If the buttons presssed on the shield don't correlate with what is displayed, adjust the ranges in
 * the currentKey() function so all accesable keys (Select, Left, Up, Down, and Left) are recognized properly. Be sure to make the same changes
 * to any other instances of currentKey((
 * 
 * Uses a LCD Keypad shield and an Arduino UNO
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <LiquidCrystal.h> //Library for using lcd displays

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
int lastKeyPressed = 5; //Used to help with pressing lcd keypad buttons.

/*-----( Declare objects )-----*/
//Setup lcd display in 4bit parallel mode ( rs, enable, d4, d5, d6, d7).
//Digital pin numbers on the audrino should match up to the digital pins on the lcd.
//If your lcd is not displaying any characters or is printing garbage data,
//check your connections and check with the manufacturer for the correct pins to use on your lcd screen
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 


void setup() { /*----( SETUP: RUNS ONCE )----*/

lcd.begin(LCD_ROWS, LCD_COLS); //Start using the lcd screen

}
/*--(end setup )---*/

void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  lcd.setCursor(0,0); //Set cursor for the LCD in the upper-left corner 

  int keyPressed = currentKey(); //Get the current key being pressed on the LCD screen if any.

  if(keyPressed == KEY_SELECT && lastKeyPressed != keyPressed){ //Do the following if the select key is pressed for the first time
    lcd.print("Select"); //Print "Select" to the lcd screen
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once when pressing the key
  } 
  
  else if(keyPressed == KEY_LEFT && lastKeyPressed != keyPressed){ //Do the following if the left key is pressed for the first time
    lcd.print("Left  "); //Print "Left" to the lcd screen. The extra whitepsace clear out any characters to the right of the word.
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once when pressing the key
  }
  
  else if(keyPressed == KEY_UP && lastKeyPressed != keyPressed){ //Do the following if the up key is pressed for the first time
    lcd.print("Up    "); //Print "Up" to the lcd screen. The extra whitepsace clear out any characters to the right of the word.
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once when pressing the key
  }
  
  else if(keyPressed == KEY_RIGHT && lastKeyPressed != keyPressed){ //Do the following if the right key is pressed for the first time
    lcd.print("Right "); //Print "Right" to the lcd screen. The extra whitepsace clear out any characters to the right of the word.
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once when pressing the key
  }
  
  else if(keyPressed == KEY_DOWN && lastKeyPressed != keyPressed){ //Do the following if the down key is pressed for the first time
    lcd.print("Down  "); //Print "Down" to the lcd screen. The extra whitepsace clear out any characters to the right of the word.
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once when pressing the key   
  }

  else if(keyPressed == KEY_NONE && lastKeyPressed != keyPressed){ //Do the following when a key is released
    lcd.print("None  "); //Print "None" to the lcd screen. The extra whitepsace clear out any characters to the right of the word.
    lastKeyPressed = keyPressed; //Update lastKeyPressed so the conditional only triggers once upon releasing a key 
  }

  lcd.setCursor(0,LCD_ROWS - 1); //Set cursor for the LCD in the bottom-left corner
  lcd.print("Output: "); //Add a label for the raw analog value to be output
  lcd.print(analogRead(0)); //Display the analog value from the shield
  lcd.print("     "); //Remove any extra characters to the right of the analog value

}
/* --(end main loop )-- */



//Function for returning the current key on the keypad being held down.
//The ranges used can vary between lcd screens, but the ones included shoudl work with most lcd screens.
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


 /*
 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.
 
 based on the Autoscroll example, included
 in the LiquidCrystal Arduino Library
 */
 
#include <Wire.h>
#include <LiquidCrystal.h>
#include <TKLCD.h>

const int EXAMPLES_DELAY = 5000;

//TKLCD_Twi lcd = TKLCD_Twi();
//TKLCD_Serial lcd = TKLCD_Serial(); //for Arduino Mega and Mega ADK write
//TKLCD_Serial lcd = TKLCD_Serial(x); where x is the number of the
//serial connector you want to use (x = 1, 2,3 or 4)
TKLCD_Local lcd = TKLCD_Local();

/*If you want to modify the address of I2C communication use the function
 lcd.setActiveAddress(NEW_ADDRESS);
 
 Default address is 0x33*/

void setup() {
  lcd.begin();
  lcd.print("Hello, world!");
  delay(1000);
}

void loop() {
  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // delay at the end of the full loop:
  delay(1000);

 
}


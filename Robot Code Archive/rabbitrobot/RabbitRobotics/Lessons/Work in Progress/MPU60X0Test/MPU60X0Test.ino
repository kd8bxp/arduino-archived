/******************************************
 * 
 * This program is meant to test the MPU60X0 sixaxis gyro servo functionality using a LCD shield.
 * The type of value output to the screen can be changed with the up and down keys and the axis can be changed with the left and right keys where applicable
 * Note that the gyro and accelerometer data being retured are the raw values from the sensor and can fluctuate ±60 units. 
 * 
 * Uses a LCD keypad shield, an Arduino UNO, and a MPU60X0 sixaxis gyro
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <Wire.h> //Library for using I2C and TWI devices
#include <LiquidCrystal.h> //Library for using lcd displays

/*-----( Define constants )-----*/
#define KEY_SELECT 0 //Constants representing each readable key on the lcd keypad shield
#define KEY_LEFT 1
#define KEY_UP 2
#define KEY_RIGHT 3
#define KEY_DOWN 4
#define KEY_NONE 5 

#define DISP_POSITION 0 //Since the sensor outputs so much data, we have 3 different display modes for the lcd
#define DISP_TEMP 1
#define DISP_ROT 2

#define X_VAL 0 //Even with the display modes, we don't have enough characters to dispaly everything at once, so the user has to scroll through x y and z values.
#define Y_VAL 1
#define Z_VAL 2

#define LCD_ROWS 2 //The number of rows and columns on the lcd screen
#define LCD_COLS 16

//Default address for the accelerometer/gyroscope.
//The address can be changed to 0x69 by applying digital HIGH to the ad0 pin on the MPU6000
#define MPU_ADDRESS 0x68  

/*-----( Declare variables )-----*/
//16-bit values for storing the data output by the sensor.
short xPos, yPos, zPos, temp, xRot, yRot, zRot = 0; //Stores X-position, Y-position, Z-position, tempurature, X-rotation, Y-rotation, and Z-rotation
short ixPos, iyPos, izPos, ixRot, iyRot, izRot = 0; //Stores the intial values for the position and rotation, so that the difference from start will be displayed

float realTemp = 0.0; //The tempurature directly given by the sensor isn't attatched to any real-world units, so this will hold the converted tempurature in Celcius

int displayMode = DISP_POSITION; //Stores the current display mode
int displayValue = X_VAL; //Stores what value (x, y, or z) is supposed to be displayed on the lcd
int keyPressed = KEY_NONE; //Stores the key being currently pressed
int lastKeyPressed= KEY_NONE; //Stores the last key that was pressed

/*-----( Declare objects )-----*/
//Setup lcd display in 4bit parallel mode ( rs, enable, d4, d5, d6, d7).
//Digital pin numbers on the audrino should match up to the digital pins on the lcd.
//If your lcd is not displaying any characters or is printing garbage data,
//check your connections and check with the manufacturer for the correct pins to use on your lcd screen.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 


void setup() { /*----( SETUP: RUNS ONCE )----*/

  lcd.begin(LCD_ROWS, LCD_COLS); //Start using the lcd screen

  Wire.begin(); //Initiate i2c as master
  Wire.beginTransmission(MPU_ADDRESS); //Connect to the sensor
  Wire.write(0x6B); //Point to the 1st power management registry (PWR_MGMT_1)
  Wire.write(0x00); //Wake up the sensor
  Wire.endTransmission(); //End connection to the device

  readMPU60X0();//Read data off of the MPU to get starting values
  
  //Store the starting values for the positon and rotation of the sensor.
  ixPos = xPos;
  iyPos = yPos;
  izPos = zPos;
  ixRot = xRot;
  iyRot = yRot;
  izRot = zRot;
}
/*--(end setup )---*/



void loop() {  /*----( LOOP: RUNS CONSTANTLY )----*/

  readMPU60X0(); //Read data off of the MPU to be displayed
  
  //Get the current key being pressed on the LCD screen every four milliseconds
  if(millis() % 4 == 0){
    keyPressed = currentKey(); 
  }
  
  if(keyPressed == KEY_UP && lastKeyPressed != keyPressed){ //Do the following if the up key is pressed once
    if(displayMode < DISP_ROT) displayMode++; //Increment the display mode if it isn't displaying gyro values
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  } 

  else if(keyPressed == KEY_DOWN && lastKeyPressed != keyPressed){ //Do the following if the down key is pressed once
    if(displayMode > DISP_POSITION)displayMode--; //Decrement the display mode if it isn't dispalying accelerometer values
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }

  else if(keyPressed == KEY_LEFT && lastKeyPressed != keyPressed){ //Do the following if the left key is pressed once
    if(displayValue > X_VAL) displayValue--; //Decrement what value will be displayed if it isn't already displaying X 
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }
  
  else if(keyPressed == KEY_RIGHT && lastKeyPressed != keyPressed){ //Do the following if the right key is pressed once
    if(displayValue < Z_VAL) displayValue++; //Increment what value will be displayed if it isn't already displaying Z 
    lastKeyPressed = keyPressed; //Update lastKeyPressed
  }
  
  else if(lastKeyPressed != keyPressed){
    lastKeyPressed = keyPressed; //If none of the used keys were pressed, but the key pressed still changed, update lastKeyPress
  }


  if(millis() % 100 == 0){//Do the following for every 10th of a second

    lcd.setCursor(0,0);//Set the cursor of the lcd at the upper-left corner

    if(displayMode == DISP_POSITION){//do the following if the position is going to be displayed
    
      lcd.print("Position    "); //Print "Position" to the lcd
    
      lcd.setCursor(0,LCD_ROWS - 1); //Set the lcd cursor at the bottom-left corner 
    
      switch(displayValue){ //Complete one of the following blocks of code depending on what value is going to be displayed
      
        case X_VAL: //If x is supposed to be displayed:
        
          lcd.print("X: "); //Print a label for the data
          lcd.print(xPos - ixPos); //Print the change in x-position from start
          lcd.print("      "); //Clear out any extra characters to the right of the x-position
          break; //Exit switch-case statement

        case Y_VAL: //Print a label for the data
          lcd.print("Y: "); //Print a label for the data
          lcd.print(yPos - iyPos); //Print the change in y-position from start
          lcd.print("      "); //Clear out any extra characters to the right of the y-position
          break; //Exit switch-case statement

        case Z_VAL: //Print a label for the data
          lcd.print("Z: "); //Print a label for the data
          lcd.print(zPos - izPos); //Print the change in z-position from start
          lcd.print("      "); //Clear out any extra characters to the right of the z-position
          break; //Exit switch-case statement


        default: //If for some reason some other type of value is trying to be displayed, show an error message
          lcd.print("ERR: Invalid");
    }
    
  }

  else if(displayMode == DISP_TEMP){ //Do the following if the temperature is supposed to be displayed
    lcd.print("Tempurature "); //Print "Tempurature" to the lcd
    lcd.setCursor(0,LCD_ROWS - 1); //Set the lcd cursor in the bottom-left corner of the screen
    realTemp = ( (float) temp/340) + 36.5; //Convert the tempurature returned by the sensor to celcius using the function included in the documentation
    lcd.print("C: "); //Print a label for the data
    lcd.print(realTemp); //Print the tempurature to the lcd
    lcd.print("       "); //Remove any extra characters to the right of the tempurature
  }

  else if(displayMode == DISP_ROT){ //Do the following if the rotation is supposed to be displayed
    lcd.print("Rotation    "); //Print "Rotation" to the lcd
    lcd.setCursor(0,LCD_ROWS - 1); //Set the lcd cursor in the bottom-left corner of the screen
    
    switch(displayValue){ //Complete one of the following blocks of code depending on what value is going to be displayed
      
      case X_VAL: //If x is supposed to be displayed:
        lcd.print("X: "); //Print a label for the data
        lcd.print(xRot - ixRot); //Print the change in x-rotation from start
        lcd.print("      "); //Clear out any extra characters to the right of the x-rotation
        break; //Exit switch-case statement

      case Y_VAL: //If y is supposed to be displayed:
        lcd.print("Y: "); //Print a label for the data
        lcd.print(yRot - iyRot); lcd.print(yPos - iyPos); //Print the change in y-rotation from start
        lcd.print("      "); //Clear out any extra characters to the right of the y-rotation
        break; //Exit switch-case statement

      case Z_VAL: //If z is supposed to be displayed:
        lcd.print("Z: "); //Print a label for the data
        lcd.print(zRot - izRot); //Print the change in z-rotation from start
        lcd.print("      "); //Clear out any extra characters to the right of the z-rotation
        break; //Exit switch-case statement

        default: //If for some reason some other type of value is trying to be displayed, show an error message
          lcd.print("ERR: Invalid");
      }
    
   }

   else{ //If an unknown display is trying to be printed, display an error message
      lcd.print("ERR: Invalid");
   }

  }
  
}
/* --(end main loop )-- */



/* 
 * Function for returning the current key on the keypad being held down.
 * The ranges used can vary between lcd screens, but the ones included shoudl work with most lcd screens.
 * If they don't, try running LCDTest to determine the proper voltage thesholds
 *
 */
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



/* 
 *  Function for reading sensor data off of the MPU60X0.
 *  Should be called each loop.
 */
void readMPU60X0(){
  
  Wire.beginTransmission(MPU_ADDRESS);//re-connect to the sensor
  Wire.write(0x3B); //Point to the start of the registry block containing sensor output values (ACCEL_XOUT_H)
  Wire.endTransmission(false); //End connection the the device, but don't allow it to be used by other devices.
  Wire.requestFrom(MPU_ADDRESS, 14); //Begin requesting data from the block of sensor output registries.

  /* Get the data from the sensor. Since the sensor produces 16 bit data while 12c only transmists 8 bits at a time,
  *  each sensor reading is split into two registries. The first registry represents the high 8 bits of the 16-bit value,
  *  and the 2nd registry provides the low 8 bits. The line below takes the data from the first registry and moves it to the left
  *  or "high" side of the 16-bit value and combines it with the data from the 2nd registry on the "low" side. 
  *  
  *  Example:
  *  1st registry value = 0011 1000
  *  2nd registry value = 0110 0010
  *  short xPos = 0000 0000 0000 0000
  *  
  *  Read 1st registry value:
  *  xPos = 0000 0000 0011 1000
  *  
  *  8 bit left shift (<<):
  *  xPos = 0011 1000 0000 0000
  *  
  *  bitwise OR ( | ) with 2nd registry value
  *  xPos = 0011 1000 0110 0010
  *  
  *  Done!
  */ 
  xPos = (Wire.read() << 8) | Wire.read(); //Get the position on the x-axis (ACCEL_XOUT_H & ACCEL_XOUT_L)
  yPos = (Wire.read() << 8) | Wire.read(); //Get the position on the y-axis (ACCEL_YOUT_H & ACCEL_YOUT_L)
  zPos = (Wire.read() << 8) | Wire.read(); //Get the position on the z-axis (ACCEL_ZOUT_H & ACCEL_ZOUT_L)
  temp = (Wire.read() << 8) | Wire.read(); //Get the tempurature. (TEMP_OUT_H & TEMP_OUT_L)
  xRot = (Wire.read() << 8) | Wire.read(); //Get the rotation on the x-axis (GYRO_XOUT_H & GYRO_XOUT_L)
  yRot = (Wire.read() << 8) | Wire.read(); //Get the rotation on the y-axis (GYRO_YOUT_H & GYRO_YOUT_L)
  zRot = (Wire.read() << 8) | Wire.read(); //Get the rotation on the z-axis (GYRO_ZOUT_H & GYRO_ZOUT_L)
  
  /* ( End reading I2C data) */

}



/* ( End ) */

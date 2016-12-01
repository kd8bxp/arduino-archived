/* Nicomachus for the Freetronics 16x2 LCD shield
 http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9
 http://www.atariarchives.org/basicgames/showpage.php?page=117
 
 This guy from 90AD has got your number (1 - 100).
 
 by eturnerx@gmail.com 201211
 
 */
/*--------------------------------------------------------------------------------------
 Includes
 --------------------------------------------------------------------------------------*/
#include <LiquidCrystal.h>   // include LCD library
/*--------------------------------------------------------------------------------------
 Defines
 --------------------------------------------------------------------------------------*/
// Pins in use
#define BUTTON_ADC_PIN           A0  // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN         3  // D3 controls LCD backlight
// ADC readings expected for the 5 buttons on the ADC input
#define RIGHT_10BIT_ADC          0  // right
#define UP_10BIT_ADC            99  // up
#define DOWN_10BIT_ADC          256  // down
#define LEFT_10BIT_ADC          410  // left
#define SELECT_10BIT_ADC        640  // right
#define BUTTONHYSTERESIS         10  // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE               0  // 
#define BUTTON_RIGHT              1  // 
#define BUTTON_UP                 2  // 
#define BUTTON_DOWN               3  // 
#define BUTTON_LEFT               4  // 
#define BUTTON_SELECT             5  // 

// Define our states
#define STATE_TITLE               00  // A title screen
#define STATE_KEYWAIT             05  // Waiting for a keypress
#define STATE_GAMESTART           20  // The game begins
#define STATE_INPUTNUMBERA        30
#define STATE_INPUTNUMBERB        40
#define STATE_INPUTNUMBERC        50

// Misc. Defines
#define MSGFLASHDELAY           1500  // How long to flash messages for (ms)?

/*--------------------------------------------------------------------------------------
 Variables
 --------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

// Setup the variables that will be used
byte state, nextstate;
byte a, c, b;
int result;

/*--------------------------------------------------------------------------------------
 Init the LCD library with the LCD pins to be used
 --------------------------------------------------------------------------------------*/
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );   //Pins for the freetronics 16x2 LCD shield. LCD: ( RS, E, LCD-D4, LCD-D5, LCD-D6, LCD-D7 )


/*--------------------------------------------------------------------------------------
 setup()
 Called by the Arduino framework once, before the main loop begins
 --------------------------------------------------------------------------------------*/
void setup() {
  // Setup pins. 
  pinMode(BUTTON_ADC_PIN, INPUT );         // Analogue input for the LCD's buttons
  digitalWrite(BUTTON_ADC_PIN, LOW );
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT );     // Backlight control (off/on)
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH );  // Turn the backlight on
  randomSeed(analogRead(1));               // Send the random number generator

  lcd.begin( 16, 2 );                      // Tell LCD library this is 16x2 display
  lcd.setCursor( 0, 0 );
  //          1234567890123456
  lcd.print( "--_NICOMACHUS_--" );
  lcd.setCursor( 0, 1 );
  lcd.print( '.' );
  lcd.print(  '.' );   
  flashbacklight();
  lcd.print(   '.' );  
  flashbacklight();
  lcd.print(    '.' ); 
  flashbacklight();
  a = b = c = 0;
  state = STATE_TITLE;  
}


/*--------------------------------------------------------------------------------------
 loop()
 Arduino main loop
 --------------------------------------------------------------------------------------*/
void loop() {
  byte button;

  button = ReadButtons();
  switch(state) {                  //    Do different things depending on "state"

  case STATE_TITLE:
    lcd.setCursor( 0, 0 );
    //          1234567890123456
    lcd.print( "== NICOMACHUS ==");
    lcd.setCursor( 0, 1 );
    lcd.print( "Press [any] key" );
    a = b = c = 0;
    nextstate = STATE_GAMESTART;
    state = STATE_KEYWAIT;
    break;

  case STATE_KEYWAIT:
    if(buttonJustPressed) state = nextstate;
    break;

  case STATE_GAMESTART:
    lcd.clear();
    //         1234567890123456
    lcd.print("Choose a number"); 
    lcd.setCursor(0,1);
    lcd.print("between 1 & 100");
    delay(MSGFLASHDELAY);
    lcd.clear();
    //         1234567890123456
    lcd.print("Remainder of num"); 
    lcd.setCursor(0,1);
    lcd.print("divided by 3 = 0");
    state = STATE_INPUTNUMBERA;
    break;

  case STATE_INPUTNUMBERA:
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(a == 0) a = 2; 
        else a--;
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(a == 2) a = 0; 
        else a++;
        break;
      case BUTTON_SELECT:
        b = 0;
        lcd.clear();
        lcd.print("Remainder of num"); 
        lcd.setCursor(0,1);
        lcd.print("divided by 5 = 0");
        delay(150);
        state = STATE_INPUTNUMBERB;
        break;
      }
      if(state == STATE_INPUTNUMBERA) {
        lcd.setCursor(15,1);
        lcd.print(a);
      }
    }
    break;

  case STATE_INPUTNUMBERB:
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(b == 0) b = 4; 
        else b--;
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(b == 4) b = 0; 
        else b++;
        break;
      case BUTTON_SELECT:
        c = 0;
        lcd.clear();
        lcd.print("Remainder of num"); 
        lcd.setCursor(0,1);
        lcd.print("divided by 7 = 0");
        delay(150);
        state = STATE_INPUTNUMBERC;
        break;
      }
      if(state == STATE_INPUTNUMBERB) {
        lcd.setCursor(15,1);
        lcd.print(b);
      }
    }
    break;

  case STATE_INPUTNUMBERC:
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(c == 0) c = 6; 
        else c--;
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(c == 6) c = 0; 
        else c++;
        break;
      case BUTTON_SELECT:
        lcd.clear();
        //         1234567890123456
        lcd.print("Let me think");
        delay(MSGFLASHDELAY/2);
        lcd.print('.');
        delay(MSGFLASHDELAY/3);
        lcd.print('.');
        delay(MSGFLASHDELAY/4);
        lcd.print('.');
        delay(MSGFLASHDELAY/5);
        lcd.print('.');
        delay(MSGFLASHDELAY/6);
        result = (70 * a) + (21 * b) + (15 * c);
        while(result > 104) result -= 105;
        lcd.clear();
        //         1234567890123456
        lcd.print("The number = ");
        lcd.print(result);
        lcd.setCursor(0,1); 
        lcd.print( "Press [any] key" );
        nextstate = STATE_TITLE;
        state = STATE_KEYWAIT;
        break;
      }
      if(state == STATE_INPUTNUMBERC) {
        lcd.setCursor(15,1);
        lcd.print(c);
      }
    }
    break;

  default: 
    break;
  }
  //clear the buttonJustPressed & buttonJustReleased flags; they've already done their job
  if(buttonJustPressed)  buttonJustPressed  = false;
  if(buttonJustReleased) buttonJustReleased = false;
  delay(25); // Delay to stop buttons "jittering" false presses. Not as good as a real debounce.
}

void flashbacklight() {
  digitalWrite(LCD_BACKLIGHT_PIN, LOW);  
  delay(150);
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH); 
  delay(150);
}

/*--------------------------------------------------------------------------------------
 ReadButtons()
 Detect the button pressed and return the value
 Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
 --------------------------------------------------------------------------------------*/
byte ReadButtons() {
  unsigned int buttonVoltage;
  byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn

  //read the button ADC pin voltage
  buttonVoltage = analogRead( BUTTON_ADC_PIN );
  //sense if the voltage falls within valid voltage windows
  if( buttonVoltage < ( RIGHT_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_RIGHT;
  }
  else if(   buttonVoltage >= ( UP_10BIT_ADC - BUTTONHYSTERESIS )
    && buttonVoltage <= ( UP_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_UP;
  }
  else if(   buttonVoltage >= ( DOWN_10BIT_ADC - BUTTONHYSTERESIS )
    && buttonVoltage <= ( DOWN_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_DOWN;
  }
  else if(   buttonVoltage >= ( LEFT_10BIT_ADC - BUTTONHYSTERESIS )
    && buttonVoltage <= ( LEFT_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_LEFT;
  }
  else if(   buttonVoltage >= ( SELECT_10BIT_ADC - BUTTONHYSTERESIS )
    && buttonVoltage <= ( SELECT_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_SELECT;
  }
  //handle button flags for just pressed and just released events
  if( ( buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) )
  {
    //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press eve
    //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
    buttonJustPressed  = true;
    buttonJustReleased = false;
  }
  if( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) )
  {
    buttonJustPressed  = false;
    buttonJustReleased = true;
  }

  //save the latest button value, for change event detection next time round
  buttonWas = button;

  return( button );
}

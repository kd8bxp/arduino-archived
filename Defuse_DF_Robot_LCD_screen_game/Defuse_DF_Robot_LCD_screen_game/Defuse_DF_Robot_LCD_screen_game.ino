/* Defuse for the Freetronics 16x2 LCD shield
 http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9
 http://www.atariarchives.org/morebasicgames/showpage.php?page=48
 
 You're in a building of 1 million rooms. There are 100 floor, and 100x100 rooms
 per floor. A bomb is about to go off in 200 seconds. Use your bomb detector to
 find the bomb. All it has is a single signal to read.
 
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
#define RIGHT_10BIT_ADC           0  // right
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
#define STATE_TURNSTART           30
#define STATE_INPUTNUMBER         40

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
byte timer, inputpos, flash;
boolean update;
byte bombpos[6], pos[6], signal[6];
byte ipos[6] = { 
  4,5, 11,12, 14,15 };
int tmp;
unsigned long flashtime;

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
  lcd.print( "-_D E F U S E _-" );
  lcd.setCursor( 0, 1 );
  lcd.print( '.' );
  lcd.print(  '.' );   
  flashbacklight();
  lcd.print(   '.' );  
  flashbacklight();
  lcd.print(    '.' ); 
  flashbacklight();
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
    lcd.print( "= D E F U S E  =");
    lcd.setCursor( 0, 1 );
    lcd.print( "Press [any] key" );
    nextstate = STATE_GAMESTART;
    state = STATE_KEYWAIT;
    break;

  case STATE_KEYWAIT:
    if(buttonJustPressed) state = nextstate;
    break;

  case STATE_GAMESTART:
    //Init signal order
    for(int i = 0; i < 6; i++) signal[i] = i;

    //Shuffle signal order
    for(int cnt = 0; cnt < 500; cnt++) {
      int pos1 = random(6);
      int pos2 = random(6);
      tmp = signal[pos1];
      signal[pos1] = signal[pos2];
      signal[pos2] = tmp;
    }
    //Pick a bomb position. It can't be 00,00,00
    do {
      tmp = 0;
      for(int i = 0; i < 6; i++) {
        pos[i] = 0;
        bombpos[i] = random(9);
        tmp += bombpos[i];
      }
    }
    while(tmp == 0);
    timer = 210;
    lcd.clear();
    //         1234567890123456
    lcd.print("Find the bomb in"); 
    lcd.setCursor(0,1);
    lcd.print("time! Here's the");
    delay(MSGFLASHDELAY); 
    lcd.setCursor(0,0);
    lcd.print("detector. Theres"); 
    lcd.setCursor(0,1);
    lcd.print("1 million rooms.");
    delay(MSGFLASHDELAY); 
    lcd.setCursor(0,0);
    lcd.print("Umm. So how does");
    lcd.setCursor(0,1);
    lcd.print("this thing work?");
    delay(MSGFLASHDELAY); 
    state = STATE_TURNSTART;
    //falls through

  case STATE_TURNSTART:
    //         1234567890123456
    //         SSSSSSsig  t:TTT
    //         Flr:YY  Rm:XX.ZZ
    if(timer == 0) {
      flashbacklight();
      lcd.setCursor(0,0);
      lcd.print("B O O O O O O "); 
      lcd.setCursor(0,1);
      lcd.print(" O O O O O OM!");
      delay(MSGFLASHDELAY); 
      lcd.clear();
      lcd.print("The bombs was"); 
      lcd.setCursor(0,1);
      lcd.print("Flr:"); 
      lcd.print(bombpos[0]); 
      lcd.print(bombpos[1]);
      lcd.print("  Rm:"); 
      lcd.print(bombpos[2]); 
      lcd.print(bombpos[3]);
      lcd.print("."); 
      lcd.print(bombpos[4]); 
      lcd.print(bombpos[5]);
      nextstate = STATE_TITLE;
      state = STATE_KEYWAIT;
      break;
    }
    timer -= 10;
    lcd.clear();
    for(int i = 0; i < 6; i++) {
      lcd.print(9 - abs((int)bombpos[signal[i]] - (int)pos[signal[i]]));
    }
    lcd.print("sig  t:");
    if(timer<100) lcd.print("0");
    lcd.print(timer);
    lcd.setCursor(0,1);
    lcd.print("Flr:YY  Rm:XX.ZZ");
    for(int i = 0; i < 6; i++) {
      lcd.setCursor(ipos[i],1);
      lcd.print(pos[i]);
    }
    inputpos = 0;
    flashtime = millis() + 750; 
    flash = 1;
    state = STATE_INPUTNUMBER;
    //falls through

  case STATE_INPUTNUMBER:
    update = false;
    if(millis() >= flashtime) {
      flashtime = millis() + (750 - (500 * flash));      
      flash = 1 - flash;
      update = true;
    }
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT:
        lcd.setCursor(ipos[inputpos],1);
        lcd.print(pos[inputpos]);
        if(inputpos == 0) inputpos = 5;
        else inputpos--;
        update = true;
        break;

      case BUTTON_RIGHT:
        lcd.setCursor(ipos[inputpos],1);
        lcd.print(pos[inputpos]);
        if(inputpos == 5) inputpos = 0;
        else inputpos++;
        update = true;
        break;

      case BUTTON_DOWN:
        if(pos[inputpos] == 0) pos[inputpos] = 9; 
        else pos[inputpos]--;
        update = true;
        break;

      case BUTTON_UP:
        if(pos[inputpos] == 9) pos[inputpos] = 0; 
        else pos[inputpos]++;
        update = true;
        break;

      case BUTTON_SELECT:
        lcd.clear();
        if(bombpos[0] == pos[0] && bombpos[1] == pos[1] && bombpos[2] == pos[2] &&
        bombpos[3] == pos[3] && bombpos[4] == pos[4] && bombpos[5] == pos[5]) {
          //         1234567890123456
          lcd.print("Bomb deactivated"); lcd.setCursor(0,1);
          lcd.print("You're a hero!");
          nextstate = STATE_TITLE;
          state = STATE_KEYWAIT;
        } else {
          lcd.print("Scanning");
          delay(MSGFLASHDELAY/2);
          lcd.print(".");
          delay(MSGFLASHDELAY/3);
          lcd.print(".");
          delay(MSGFLASHDELAY/4);
          lcd.print(".");
          delay(MSGFLASHDELAY/5);
          lcd.print(".");
          delay(MSGFLASHDELAY/6);
          state = STATE_TURNSTART;
        }
        update = false;
        break;
      }
    }
    if(update == true) {
      lcd.setCursor(ipos[inputpos],1);
      if(flash == 0) lcd.print("_");
      else lcd.print(pos[inputpos]);
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
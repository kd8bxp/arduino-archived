/* Mugwump for the Freetronics 16x2 LCD shield
 http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9
 http://www.atariarchives.org/basicgames/showpage.php?page=115
 
 There are four mugwumps hiding on a 10x10 grid. You enter your guess and then
 are told if you've found a mugwump and how far to the mugwumps.
 The first number is x, the second is y.
 You get ten turns and then you are told where the mugwumps are hiding.
 
 The earliest reference to Mugwump is: http://www.svipx.com/pcc/gameslist.html
 Feb 1973, Bob Albrecht of People's Computing Company and Bud Valenti (and students).
 This Arduino version is based on the listing from Basic Computer Games 1978, David Ahl
 
 by eturnerx@gmail.com 201210
 
 */
/*--------------------------------------------------------------------------------------
 Includes
 --------------------------------------------------------------------------------------*/
#include <LiquidCrystal.h>    // include LCD library
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
#define STATE_TURNSTART           20  // The turn begins
#define STATE_OPTIONX             30  // Getting input for x axis
#define STATE_OPTIONY             40  // Getting input for y axis
#define STATE_GAMEOVER            99

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
byte turn, score, scanx, scany, mugwumpx[4], mugwumpy[4];
boolean mugwumpfound[4];
int distance;

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
  lcd.print( "--_ Mugwump. _--" );
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
    lcd.print( "==  Mugwump   ==");
    lcd.setCursor( 0, 1 );
    lcd.print( "Press [any] key" );
    turn = score = 0;
    for(byte i = 0; i < 4; i++) {
      mugwumpx[i] = random(10);
      mugwumpy[i] = random(10);
      mugwumpfound[i] = false;
    }
    nextstate = STATE_TURNSTART;
    state = STATE_KEYWAIT;
    break;

  case STATE_KEYWAIT:
    if(buttonJustPressed) state = nextstate;
    break;

  case STATE_TURNSTART:
    turn++;
    lcd.clear();
    if(turn > 10) {
      //          1234567890123456
      lcd.print("Sorry, that's 10"); 
      lcd.setCursor(0,1);
      lcd.print("tries.They're at");
      flashbacklight();
      delay(MSGFLASHDELAY);
      score = 0;
      for(int i = 0; i < 4; i++) {
        lcd.clear();
        //        1234567890123456
        lcd.print("Mugwump");
        lcd.print((char)('A' + i));
        lcd.print(" at: ");
        lcd.print(mugwumpx[i]);
        lcd.print(',');
        lcd.print(mugwumpy[i]);
        lcd.setCursor(0,1);
        if(!mugwumpfound[i]) {
          lcd.print("Still hidden.");
        } 
        else {
          lcd.print("You found it!");
          score++;
        }
        delay(MSGFLASHDELAY);
      }
      lcd.clear();
      lcd.print("Final score = ");
      lcd.print(score);
      delay(MSGFLASHDELAY);
      state = STATE_GAMEOVER;
      break;
    }

    //        1234567890123456
    //        Ad   Bd  Cd   Dd
    for(int i = 0; i < 4; i++) {
      lcd.print((char)('A' + i));
      if(mugwumpfound[i]) lcd.print('X');
      else if(turn == 1) lcd.print('?');
      else {
        distance = (int)(sqrt(sq(scanx - mugwumpx[i])+sq(scany - mugwumpy[i])) + 0.5f);
        if(distance > 9) lcd.print('>'); else lcd.print(distance);
      }
      if(i < 3) {
        lcd.print("  ");
        if(i == 0 || i == 2) lcd.print(' ');
      }
    }
    lcd.setCursor(0,1);
    lcd.print("Turn:" );
    lcd.print(turn);
    lcd.setCursor(9,1);
    lcd.print("X:0");
    scanx = 0;
    state = STATE_OPTIONX;
    break;

  case STATE_OPTIONX:
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(scanx == 0) scanx = 9; 
        else scanx--;
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(scanx == 9) scanx = 0; 
        else scanx++;
        break;
      case BUTTON_SELECT:
        scany = 0;
        lcd.setCursor(13,1);
        lcd.print("Y:0");
        state = STATE_OPTIONY;
        break;
      }
    }
    lcd.setCursor(11,1);
    lcd.print(scanx);
    break;

  case STATE_OPTIONY:
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(scany == 0) scany = 9; 
        else scany--;
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(scany == 9) scany = 0; 
        else scany++;
        break;
      case BUTTON_SELECT:
        for(int i = 0; i < 4; i++) {
          if(!mugwumpfound[i] && mugwumpx[i] == scanx && mugwumpy[i] == scany) {
            lcd.clear();
            lcd.print("Found Mugwump");
            lcd.print((char)('A' + i));
            mugwumpfound[i] = true;
            score++;
            flashbacklight();
            delay(MSGFLASHDELAY);
          }
        }
        if(score == 4) {
          lcd.clear();
          //        1234567890123456
          lcd.print("You got them all"); lcd.setCursor(0,1);
          lcd.print("Final score = ");
          lcd.print(15 - turn);
          flashbacklight();
          state = STATE_KEYWAIT;
          nextstate = STATE_GAMEOVER;
        } 
        else
          state = STATE_TURNSTART;
        break;
      }
    }
    if(state == STATE_OPTIONY) {
      lcd.setCursor(15,1);
      scany = constrain(scany, 0, 9);
      lcd.print(scany);
    }
    break;

  case STATE_GAMEOVER:
    lcd.clear();
    //          1234567890123456
    lcd.print( "== GAME OVER  ==");
    lcd.setCursor( 0, 1 );
    lcd.print( "Press [any] key" );
    nextstate = STATE_TITLE;
    state = STATE_KEYWAIT;
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

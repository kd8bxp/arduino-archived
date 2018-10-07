/* Camel Converted/Reimagined for the Freetronics 16x2 LCD shield
by eturnerx@gmail.com 201211 http://eturnerx.com

Based info found here: http://www.atariarchives.org/morebasicgames/showpage.php?page=24
Original game by Heath Users Group

http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9

*/
/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <LiquidCrystal.h>   // include LCD library
#include <avr/pgmspace.h>    // put the strings into flash RAM - SRAM is filling up!
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
#define STATE_TITLE               00    // A title screen
#define STATE_KEYWAIT             10    // Waiting for a keypress
#define STATE_NEWGAME             30
#define STATE_NEWTURN             40
#define STATE_PLAYERMOVE          50
#define STATE_DRINK               60
#define STATE_WALK                70
#define STATE_RUN                 80
#define STATE_STOP                90
#define STATE_STATUS             100
#define STATE_HOPE               110
#define STATE_CAPTUREDMOVE       120
#define STATE_GAMEOVERWON        250
#define STATE_GAMEOVERLOST       251

// Misc. Defines
#define MSGFLASHDELAY           1250  // How long to flash messages for (ms)?

/*--------------------------------------------------------------------------------------
  String Table
--------------------------------------------------------------------------------------*/
#define STR_CAMEL                 6
//                               1234567890123456
prog_char string_0[]  PROGMEM = "hope DRINK_ walk";
prog_char string_1[]  PROGMEM = "drink _WALK_ run";
prog_char string_2[]  PROGMEM = "walk _RUN__ stop";
prog_char string_3[]  PROGMEM = "run _STOP_ stats";
prog_char string_4[]  PROGMEM = "stop STATS_ hope";
prog_char string_5[]  PROGMEM = "stats HOPE drink";
prog_char string_6[]  PROGMEM = "CAMEL - HEATH UG";
prog_char string_7[]  PROGMEM = "-- GAME  OVER --";
prog_char string_8[]  PROGMEM = "YOU WIN - PARTY!";
prog_char string_9[]  PROGMEM = "Pygmies come too";
//                               1234567890123456
prog_char string_10[] PROGMEM = "Sandstorm! Lost.";
prog_char string_11[] PROGMEM = "Camel hurt hump";
prog_char string_12[] PROGMEM = "Died  in  desert";
prog_char string_13[] PROGMEM = "CamelUnion pro-";
prog_char string_14[] PROGMEM = "test at funeral";
prog_char string_15[] PROGMEM = "Eaten by vulture";
prog_char string_16[] PROGMEM = "and cannibals";
prog_char string_17[] PROGMEM = "Your skull's now";
prog_char string_18[] PROGMEM = "used as a purse";
prog_char string_19[] PROGMEM = "Dumb people:";
//                               1234567890123456
prog_char string_20[] PROGMEM = "stay off camels";
prog_char string_21[] PROGMEM = "Turkeys fly not";
prog_char string_22[] PROGMEM = "ride camels.";
prog_char string_23[] PROGMEM = "New camel&game?";
prog_char string_24[] PROGMEM = "Ran out of water";
//prog_char string_25[] PROGMEM = "Gone XXX miles.";
prog_char string_25[] PROGMEM = "";
prog_char string_26[] PROGMEM = "At an oasis.";
prog_char string_27[] PROGMEM = "Water refilled";
prog_char string_28[] PROGMEM = "You are Captured";
prog_char string_29[] PROGMEM = "held for ransom";
//                               1234567890123456
prog_char string_30[] PROGMEM = "ESCAPE_ or  wait";
prog_char string_31[] PROGMEM = "escape  or _WAIT";
prog_char string_32[] PROGMEM = "Escape succeeded";
prog_char string_33[] PROGMEM = "Escape fail. You";
prog_char string_34[] PROGMEM = "were stabbed";
prog_char string_35[] PROGMEM = "Ransom paid!";
prog_char string_36[] PROGMEM = "Still waiting...";
prog_char string_37[] PROGMEM = "Resting...";
prog_char string_38[] PROGMEM = "Camel thanks you";
prog_char string_39[] PROGMEM = "Camel burns sand";
//                               1234567890123456
prog_char string_40[] PROGMEM = "You ran poor old";
prog_char string_41[] PROGMEM = "camel to death";
prog_char string_42[] PROGMEM = "Camel likes walk";
prog_char string_43[] PROGMEM = "Hope finds you";
prog_char string_44[] PROGMEM = "Gives you water";
prog_char string_45[] PROGMEM = "Pygmies got you";
prog_char string_46[] PROGMEM = "Camel & You soup";
prog_char string_47[] PROGMEM = "-W A R N I N G -";
prog_char string_48[] PROGMEM = "   Get a drink";
prog_char string_49[] PROGMEM = "You're at oasis";
//                               1234567890123456
prog_char string_50[] PROGMEM = "desert allround";
prog_char string_51[] PROGMEM = "Good luck and";
prog_char string_52[] PROGMEM = "   good cameling";
prog_char string_53[] PROGMEM = "You take a drink";

PROGMEM const char *string_table[] = {
  string_0,  string_1,  string_2,  string_3,  string_4,  string_5,
  string_6,  string_7,  string_8,  string_9,  string_10, string_11,
  string_12, string_13, string_14, string_15, string_16, string_17,
  string_18, string_19, string_20, string_21, string_22, string_23,
  string_24, string_25, string_26, string_27, string_28, string_29,
  string_30, string_31, string_32, string_33, string_34, string_35,
  string_36, string_37, string_38, string_39, string_40, string_41,
  string_42, string_43, string_44, string_45, string_46, string_47,
  string_48, string_49, string_50, string_51, string_52, string_53
};


/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

// Setup the variables that will be used
byte state, nextstate;

int z;  // thirst level
int s;  // drinks left in canteen
byte c;  // player distance travelled
byte c1; // pygmies distance travelled
byte q;  // 0 if free, higher if captured for ransom
byte f;  // condition of the camel
byte p;  // pygmies can't capture in the first three turns
byte choice;   // Your move
char stringbuffer[17];

int choicetostate[6] = { 
  STATE_DRINK, STATE_WALK, STATE_RUN,
  STATE_STOP, STATE_STATUS, STATE_HOPE
};

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
  lcd.print(getString(STR_CAMEL));
  lcd.setCursor( 0, 1 );
  lcd.print( '.' );
  lcd.print(  '.' );   flashbacklight();
  lcd.print(   '.' );  flashbacklight();
  lcd.print(    '.' ); flashbacklight();
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
      lcd.print(getString(STR_CAMEL));
      pressanykey(STATE_NEWGAME);
      break;

    case STATE_KEYWAIT:
      if(buttonJustPressed) state = nextstate;
      break;

    case STATE_NEWGAME:
      z = 4; s = 6; c = c1 = q = f = p = 0; 
      printscreen(49,50);
      printscreen(51,52);
      state = STATE_NEWTURN;
      // falls through

    case STATE_NEWTURN:
      if(c > 199) { state = STATE_GAMEOVERWON; break; }
      z--; p++; choice = 0;
      if(z == 1) printscreen(47,48);
      else if(z < 0) { printscreen(7,24); delay(MSGFLASHDELAY); state = STATE_GAMEOVERLOST; break; }
      if(q > 0) { 
        lcd.clear(); 
        lcd.print(getString(28)); lcd.setCursor(0,1); 
        lcd.print(getString(30)); 
        state = STATE_CAPTUREDMOVE; break; 
      }
      if(p > 3) {
        c1 += random(2,13);
        if(c1 >= c) { printscreen(45,46); delay(MSGFLASHDELAY); state = STATE_GAMEOVERLOST; break; }
      }
      lcd.clear();
      lcd.print("DIST:");
      if(c < 100) lcd.print(' ');
      if(c < 10) lcd.print(' ');
      lcd.print(c);
      if(p > 3) {
        lcd.print("   P:");
        if(c-c1 < 100) lcd.print(' ');
        if(c-c1 < 10) lcd.print(' ');
        lcd.print(c-c1);
      }
      lcd.setCursor(0,1);
      lcd.print(getString(choice));
      state = STATE_PLAYERMOVE;
      // falls through...

    case STATE_PLAYERMOVE:
      if(buttonJustPressed) {
        switch(button) {
          case BUTTON_LEFT:
            if(choice == 0) choice = 5; else choice = choice - 1;
            break;
          case BUTTON_RIGHT:
            if(choice == 5) choice = 0; else choice = choice + 1;
            break;
          case BUTTON_SELECT: 
            state = choicetostate[choice];
            break;
        }
      }
      if(state == STATE_PLAYERMOVE) {
        lcd.setCursor(0,1);
        lcd.print(getString(choice));
      }
      break;

    case STATE_DRINK:
      s = s - 1; z = 4;
      if(s < 0) { printscreen(7,24); delay(MSGFLASHDELAY); state = STATE_GAMEOVERLOST; break; }
      lcd.setCursor(0,1); 
      lcd.print(getString(53)); 
      delay(MSGFLASHDELAY);
      lcd.setCursor(0,1);
      state = STATE_PLAYERMOVE;
      break;
      
    case STATE_WALK:
      f++;
      if(f > 7) { printscreen(40,41); delay(MSGFLASHDELAY); state = STATE_GAMEOVERLOST; break; }
      movementevents();
      if(q == 0) {
        lcd.clear();
        lcd.print(getString(42));
        c = c + random(10);
        delay(MSGFLASHDELAY);
      }
      state = STATE_NEWTURN;
      break;
      
    case STATE_RUN:
      f+=3;
      if(f > 7) { printscreen(40,41); delay(MSGFLASHDELAY); state = STATE_GAMEOVERLOST; break; }
      movementevents();
      if(q == 0) {
        lcd.clear();
        lcd.print(getString(39));
        c = c + random(20);
        delay(MSGFLASHDELAY);
      }
      state = STATE_NEWTURN;
      break;

    case STATE_STOP:
      f = 0;
      printscreen(37,38);
      state = STATE_NEWTURN;
      break;

    case STATE_STATUS:
      lcd.setCursor(0,1); 
      lcd.print("Camel ");
      lcd.print(f);
      lcd.print(" day left"); 
      delay(MSGFLASHDELAY);
      lcd.setCursor(0,1);
      lcd.print("Drinks:");
      lcd.print(s);
      lcd.print(" left   ");
      delay(MSGFLASHDELAY);
      lcd.setCursor(0,1);
      lcd.print("Thirst:");
      lcd.print(s);
      lcd.print(" actions");
      delay(MSGFLASHDELAY);
      state = STATE_PLAYERMOVE;
      break;
      
    case STATE_HOPE:
      if(random(10)==4) {
        printscreen(43,44);
        s = 3, z = 0;
        state = STATE_NEWTURN;
      } else {
        printscreen(7,12); 
        delay(MSGFLASHDELAY);
        state = STATE_GAMEOVERLOST;
      }
      break;

   case STATE_CAPTUREDMOVE:
     if(buttonJustPressed) {
        z = 4; // Captors provide drinking water
        switch(button) {
          case BUTTON_LEFT:
          case BUTTON_RIGHT:
            choice = 1 - choice;
            lcd.setCursor(0,1);
            lcd.print(getString(30 + choice));
            break;
          case BUTTON_SELECT:
            if(choice == 0) {
              if(random(100) < 50) {
                lcd.clear(); 
                lcd.print(getString(32));
                delay(MSGFLASHDELAY);
                q = 0;
                state = STATE_NEWTURN;
              } else {
                printscreen(33,34);
                state = STATE_GAMEOVERLOST;
              }
            } else {
              if(random(100) < 75) {
                lcd.clear(); 
                lcd.print(getString(35));
                delay(MSGFLASHDELAY);
                q = 0;
                state = STATE_NEWTURN;
              } else {
                lcd.clear(); 
                lcd.print(getString(36));
                delay(MSGFLASHDELAY);
                state = STATE_NEWTURN;
              }
            }
            break;
        }
      }
      break;

    case STATE_GAMEOVERWON:
      printscreen(7,8);
      lcd.setCursor(0,1);
      lcd.print(getString(9));
      nextstate = STATE_TITLE;
      state = STATE_KEYWAIT;
      break;

    case STATE_GAMEOVERLOST:
      choice = (byte)((random(0,5) * 2) + 13);
      printscreen(choice, choice + 1);
      lcd.clear();
      lcd.print(getString(7)); lcd.setCursor(0,1);
      lcd.print(getString(23));
      nextstate = STATE_TITLE;
      state = STATE_KEYWAIT;
      break;

    default: break;
  }
  //clear the buttonJustPressed & buttonJustReleased flags; they've already done their job
  if(buttonJustPressed)  buttonJustPressed  = false;
  if(buttonJustReleased) buttonJustReleased = false;
  delay(50);  // Delay to stop buttons "jittering" false presses
}

void movementevents() {
  if(random(100)<5) {
    q = 1;
    printscreen(28,29);
    lcd.clear();
    lcd.print(getString(29)); lcd.setCursor(0,1);
    lcd.print(getString(30));
    state = STATE_NEWTURN;
  } else if(random(100)<20) {
    z = 4; s = 6;
    printscreen(26,27);
  } else if(random(100)<5) {
    lcd.clear();
    lcd.print(getString(10));
    delay(MSGFLASHDELAY);
    c += random(21 - 10);
  } else if(random(100)<5) {
    lcd.clear();
    lcd.print(getString(11));
    delay(MSGFLASHDELAY);
    c1++;
  }
}

void flashbacklight() {
  digitalWrite(LCD_BACKLIGHT_PIN, LOW);  delay(150);
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH); delay(150);
}

void pressanykey(byte _nextstate) {
  lcd.setCursor( 0, 1 );
  //          1234567890123456
  lcd.print( "Press [any] key " );
  nextstate = _nextstate;
  state = STATE_KEYWAIT;
}

void printscreen(byte line1, byte line2) {
  lcd.clear();
  lcd.print(getString(line1)); lcd.setCursor(0,1);
  lcd.print(getString(line2));
  delay(MSGFLASHDELAY);
}

char *getString(int stringindex) {
  strcpy_P(stringbuffer, (char*)pgm_read_word(&(string_table[stringindex])));
  return stringbuffer;
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
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
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

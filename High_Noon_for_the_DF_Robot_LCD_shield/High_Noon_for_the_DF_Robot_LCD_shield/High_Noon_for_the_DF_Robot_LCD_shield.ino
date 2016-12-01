/* High Noon Converted/Reimagined for the Freetronics 16x2 LCD shield
by eturnerx@gmail.com 201209

Based info found here: http://www.mybitbox.com/highnoon-1970/
Original game by Chris Gaylo, 1970

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
#define STATE_TITLE               20
#define STATE_NEWGAME             30    // Flavour intro
#define STATE_NEWTURN             40
#define STATE_PLAYERMOVE          50
#define STATE_ADVANCING           60
#define STATE_ADVANCINGINPUT      65    // How many paces?
#define STATE_STANDING            70
#define STATE_FIRING              80
#define STATE_DUCK                90
#define STATE_NOTROUGHS           100
#define STATE_GIVEUP              110
#define STATE_RUN                 120
#define STATE_RUNINPUT            125
#define STATE_NOAMMO              130
#define STATE_BARTMOVES           200
#define STATE_BARTNOAMMO          210
#define STATE_RELOAD              190   // Not in original... but
#define STATE_GAMEOVERWON         250
#define STATE_GAMEOVERLOST        251

/*--------------------------------------------------------------------------------------
  String Table
--------------------------------------------------------------------------------------*/
#define STR_HIGHNOON         6
#define STR_GAMEOVER         7
#define STR_GAMEOVERWIN      8
#define STR_GAMEOVERLOST     9
#define STR_BARTNOAMMO      10
#define STR_YOUNOAMMO       11
#define STR_HOWFARTOWALK    12
#define STR_YOUGITRIGHTIN   13
#define STR_BARTSUGLYFACE   14
#define STR_YOUADVANCEHERO  15
#define STR_WAIT1           16
#define STR_WAIT2           17
#define STR_YOUFIREAND      18
#define STR_NICKHIMDRAT     19
#define STR_KILLHIMYEEHAW   20
#define STR_MISSBYAMILE     21
#define STR_CLICKNOAMMO     22
#define STR_NOMORETROUGHS   23
#define STR_YOULOOKSILLY    24
#define STR_YOUDUCKBEHIND   25
#define STR_AWATERTROUGH    26
#define STR_THEREAINTMORE   27
#define STR_COWARDLYTROUGHS 28
#define STR_BARTACCEPTSAND  29
#define STR_YOULEAVEALIVE   30
#define STR_HOWFARTORUN     31
#define STR_YOURLEGSCARRY   32
#define STR_YOUOUTTATOWN    33
#define STR_YOUTURNANDRUN   34
#define STR_BARTGETSRIGHT   35
#define STR_UPINYOURFACE    36
#define STR_BARTCOMESONUP   37
#define STR_BARTSHOOTSBUT   38
#define STR_TROUGHHIDESYOU  39
#define STR_BARTSHOOTSAND   40
#define STR_NICKSYOUOUCH    41
#define STR_KILLSYOURIP     42
#define STR_MISSESBYAMILE   43
#define STR_BARTHIGHTAILS   44
#define STR_FOLKSWANTBLOOD  45
#define STR_SOGIVENEWAMMO   46
//                               1234567890123456
prog_char string_0[]  PROGMEM = "run  _WALK_ wait";
prog_char string_1[]  PROGMEM = "walk _WAIT_ fire";
prog_char string_2[]  PROGMEM = "wait _FIRE_ duck";
prog_char string_3[]  PROGMEM = "fire _DUCK_ quit";
prog_char string_4[]  PROGMEM = "duck _QUIT_ run ";
prog_char string_5[]  PROGMEM = "quit _RUN _ walk";
prog_char string_6[]  PROGMEM = "H I G H  N O O N";
prog_char string_7[]  PROGMEM = "-- GAME  OVER --";
prog_char string_8[]  PROGMEM = "YOU WIN $20,000!";
prog_char string_9[]  PROGMEM = "--  YOU LOSE  --";
prog_char string_10[] PROGMEM = "Bart has no ammo";
prog_char string_11[] PROGMEM = "You have no ammo";
prog_char string_12[] PROGMEM = "How far to walk?";
prog_char string_13[] PROGMEM = "You git right in";
prog_char string_14[] PROGMEM = "Bart's ugly face";
prog_char string_15[] PROGMEM = "You advance hero";
prog_char string_16[] PROGMEM = "Wait=Easy Target";
prog_char string_17[] PROGMEM = "Brave & stupid";
prog_char string_18[] PROGMEM = "You fire and ...";
prog_char string_19[] PROGMEM = "nick him. Drat!";
prog_char string_20[] PROGMEM = "kill him! Yeehaw";
prog_char string_21[] PROGMEM = "miss by a mile";
prog_char string_22[] PROGMEM = "Click! No ammo";
prog_char string_23[] PROGMEM = "No more troughs";
prog_char string_24[] PROGMEM = "You look silly";
prog_char string_25[] PROGMEM = "You duck behind";
prog_char string_26[] PROGMEM = "a water trough";
prog_char string_27[] PROGMEM = "There ain't more";
prog_char string_28[] PROGMEM = "cowardly troughs";
prog_char string_29[] PROGMEM = "Bart accepts and";
prog_char string_30[] PROGMEM = "you leave alive";
prog_char string_31[] PROGMEM = "How far to run?";
prog_char string_32[] PROGMEM = "Your legs carry";
prog_char string_33[] PROGMEM = "you outta town";
prog_char string_34[] PROGMEM = "You turn and run";
prog_char string_35[] PROGMEM = "Bart gets right";
prog_char string_36[] PROGMEM = "up in your face";
prog_char string_37[] PROGMEM = "Bart comes on up";
prog_char string_38[] PROGMEM = "Bart shoots but";
prog_char string_39[] PROGMEM = "trough hides you";
prog_char string_40[] PROGMEM = "Bart shoots and";
prog_char string_41[] PROGMEM = "nicks you. Ouch!";
prog_char string_42[] PROGMEM = "kills you. R.I.P";
prog_char string_43[] PROGMEM = "misses by a mile";
prog_char string_44[] PROGMEM = "Bart high tails";
prog_char string_45[] PROGMEM = "Folks want blood";
prog_char string_46[] PROGMEM = "so give new ammo";
PROGMEM const char *string_table[] = {
  string_0,  string_1,  string_2,  string_3,  string_4,  string_5,
  string_6,  string_7,  string_8,  string_9,  string_10, string_11,
  string_12, string_13, string_14, string_15, string_16, string_17,
  string_18, string_19, string_20, string_21, string_22, string_23,
  string_24, string_25, string_26, string_27, string_28, string_29,
  string_30, string_31, string_32, string_33, string_34, string_35,
  string_36, string_37, string_38, string_39, string_40, string_41,
  string_42, string_43, string_44, string_45, string_46
};


/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

// Setup the variables that will be used
byte state, nextstate;
byte bulletsyou, bulletsbart;
byte choice, inputnumber;
int turns, distance, troughs, hitchance;
byte bartmoves;
char stringbuffer[17];
int choicetostate[6] = { 
  STATE_ADVANCING, STATE_STANDING, STATE_FIRING,
  STATE_DUCK, STATE_GIVEUP, STATE_RUN
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
  lcd.print(getString(STR_HIGHNOON));
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
      lcd.print(getString(STR_HIGHNOON));
      pressanykey(STATE_NEWGAME);
      break;

    case STATE_KEYWAIT:
      if(buttonJustPressed) state = nextstate;
      break;

    case STATE_NEWGAME:
      bulletsyou = bulletsbart = 4;
      troughs = random(2,5);
      distance = 90;
      turns = 0;
      state = STATE_NEWTURN;
      // falls through

    case STATE_NEWTURN:
      turns++;
      lcd.clear();
      lcd.print('B');
      for(byte i=0;i<bulletsbart;i++) lcd.print('|');
      lcd.setCursor(15-bulletsyou,0);
      for(byte i=0;i<bulletsyou;i++) lcd.print('|');
      lcd.print('U');
      choice = 0;
      inputnumber = 1;
      lcd.setCursor(0,1);
      lcd.print(getString(choice));
      lcd.setCursor(6,0);
      if(distance < 100) lcd.print(' ');
      if(distance < 10)  lcd.print(' ');
      lcd.print(distance);
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

    case STATE_ADVANCING:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_HOWFARTOWALK));
      state = STATE_ADVANCINGINPUT;
      inputnumber = 1;
      printinputnumber();  
      break;
      
    case STATE_ADVANCINGINPUT:
      if(doinputnumber(0,10,button)) {
        if(inputnumber == 0) {
          choice = 1;
          state = STATE_STANDING;
        } else if(distance - inputnumber <= 0) {
          lcd.clear();
          //         1234567890123456
          lcd.print(getString(STR_YOUGITRIGHTIN)); lcd.setCursor(0,1);
          lcd.print(getString(STR_BARTSUGLYFACE));
          distance = 0;
          nextstate = STATE_BARTMOVES;
          state = STATE_KEYWAIT;
        } else {
          distance = distance - inputnumber;
          lcd.clear();
          //         1234567890123456
          lcd.print(getString(STR_YOUADVANCEHERO));
          pressanykey(STATE_BARTMOVES);
        }
      }
      break;
      
    case STATE_STANDING:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_WAIT1)); lcd.setCursor(0,1);
      lcd.print(getString(STR_WAIT2));
      nextstate = STATE_BARTMOVES;
      state = STATE_KEYWAIT;
      break;

    case STATE_FIRING:
      lcd.clear();
      if(bulletsyou > 0) {
        bulletsyou--;
        //         1234567890123456
        lcd.print(getString(STR_YOUFIREAND)); lcd.setCursor(0,1);
        if(random(1,100) >= distance) {
          if(random(1,4) == 1) {
            //         1234567890123456
            lcd.print(getString(STR_NICKHIMDRAT));
            nextstate = STATE_BARTMOVES;
          } else {
            lcd.print(getString(STR_KILLHIMYEEHAW));
            nextstate = STATE_GAMEOVERWON;
          }
        } else {
          //         1234567890123456
          lcd.print(getString(STR_MISSBYAMILE));
          nextstate = STATE_BARTMOVES;
        }
        //check bullets etc if we're still live
        if(nextstate == STATE_BARTMOVES && bulletsyou == 0) {
          if(bulletsbart == 0) nextstate = STATE_RELOAD;
          else nextstate = STATE_NOAMMO;
        }
      } else {
        //         1234567890123456
        lcd.print(getString(STR_CLICKNOAMMO));
        choice = 1;
        pressanykey(STATE_BARTMOVES);
      }
      state = STATE_KEYWAIT;
      break;

    case STATE_DUCK:
      lcd.clear();
      troughs = troughs - 1;
      if(troughs < 0) {
        //         1234567890123456
        lcd.print(getString(STR_NOMORETROUGHS)); lcd.setCursor(0,1);
        lcd.print(getString(STR_YOULOOKSILLY));
        choice = 1;
      } else {
        lcd.print(getString(STR_YOUDUCKBEHIND)); lcd.setCursor(0,1);
        lcd.print(getString(STR_AWATERTROUGH));
      }
      if(troughs != 0) nextstate = STATE_BARTMOVES;
      else nextstate = STATE_NOTROUGHS;
      state = STATE_KEYWAIT;
      break;
      
    case STATE_NOTROUGHS:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_THEREAINTMORE)); lcd.setCursor(0,1);
      lcd.print(getString(STR_COWARDLYTROUGHS));
      nextstate = STATE_BARTMOVES;
      state = STATE_KEYWAIT;
      break;

    case STATE_GIVEUP:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_BARTACCEPTSAND)); lcd.setCursor(0,1);
      lcd.print(getString(STR_YOULEAVEALIVE));
      nextstate = STATE_GAMEOVERLOST;
      state = STATE_KEYWAIT;
      break;

    case STATE_RUN:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_HOWFARTORUN));
      inputnumber = 1;
      printinputnumber();  
      state = STATE_RUNINPUT;
      break;
      
    case STATE_RUNINPUT:
      if(doinputnumber(0,10,button)) {
        if(inputnumber == 0) {
          state = STATE_STANDING;
        } else if(distance + inputnumber >= 120) {
          lcd.clear();
          //         1234567890123456
          lcd.print(getString(STR_YOURLEGSCARRY)); lcd.setCursor(0,1);
          lcd.print(getString(STR_YOUOUTTATOWN));
          nextstate = STATE_GAMEOVERLOST;
          state = STATE_KEYWAIT;
        } else {
          lcd.clear();
          distance = distance + inputnumber;
          //         1234567890123456
          lcd.print(getString(STR_YOUTURNANDRUN));
          pressanykey(STATE_BARTMOVES);
        }
      }
      break;

    case STATE_NOAMMO:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_YOUNOAMMO));
      pressanykey(STATE_BARTMOVES);
      break;

    case STATE_BARTMOVES:
      lcd.clear();
      
      //Simple AI - a little different to the original, maybe smarter a tad?
      hitchance = distance;
      if(choice == 1) hitchance -= 20;       // Easier to hit you when you stand or miss a duck or fire with no ammo
      else if(choice == 4) hitchance += 10;  // Cannot hit behind a trough, but Bart doesn't fully get this
      else if(choice == 5) hitchance -= 30;  // Easier to hit you when you run

      if(bulletsbart == 0) bartmoves = 2;      // If he's outta ammo he runs a bit
      else if(distance < 20) bartmoves = 1;    // If we're close Bart shoots
      else if(bulletsyou == 0) bartmoves = 0;  // Otherwise if we've no more bullets, Bart moves closer for the shot
      else if(distance > 80) bartmoves = 0;    // Otherwise if we're too far apart he moves up
      else {
        if(random(1,100) >= hitchance)     // Otherwise, his move depends on the chance of hitting you
          bartmoves = 1;                       // Shoots!
        else
          bartmoves = 0;                       // Moves
      }
      
      switch(bartmoves) {
        case 0:
          bartmoves = random(1,11);
          distance = distance - bartmoves;
          if(distance < 0) {
            distance = 0;
            //         1234567890123456
            lcd.print(getString(STR_BARTGETSRIGHT)); lcd.setCursor(0,1);
            lcd.print(getString(STR_UPINYOURFACE));
          } else {
            //         1234567890123456
            lcd.print(getString(STR_BARTCOMESONUP));
            lcd.setCursor(0,1);
            lcd.print("about ");
            lcd.print(bartmoves);
            lcd.print(" paces.");
          }
          nextstate = STATE_NEWTURN;
          break;
          
        case 1:
          bulletsbart--;
          if(choice == 3) {
            //         1234567890123456
            lcd.print(getString(STR_BARTSHOOTSBUT)); lcd.setCursor(0,1);
            lcd.print(getString(STR_TROUGHHIDESYOU));
            nextstate = STATE_NEWTURN;
          } else {
            lcd.print(getString(STR_BARTSHOOTSAND));
            lcd.setCursor(0,1);
            if(random(1,100) >= hitchance) {
              if(random(1,4) == 1) {
                //         1234567890123456
                lcd.print(getString(STR_NICKSYOUOUCH));
                nextstate = STATE_NEWTURN;
              } else {
                lcd.print(getString(STR_KILLSYOURIP));
                nextstate = STATE_GAMEOVERLOST;
              }
            } else {
              //         1234567890123456
              lcd.print(getString(STR_MISSESBYAMILE));
              nextstate = STATE_NEWTURN;
            }
          }
          //check bullets etc if we're still live
          if(nextstate == STATE_NEWTURN && bulletsbart == 0) {
           if(bulletsyou == 0) nextstate = STATE_RELOAD;
           else nextstate = STATE_BARTNOAMMO;
          }
          break;
          
        case 2:
          bartmoves = random(1,6);
          distance = distance + bartmoves;
          //         1234567890123456
          lcd.print(getString(STR_BARTHIGHTAILS));
          lcd.setCursor(0,1);
          lcd.print("it back ");
          lcd.print(bartmoves);
          lcd.print(" paces");
          nextstate = STATE_NEWTURN;
          break;
      }
      state = STATE_KEYWAIT;
      break;
      
    case STATE_BARTNOAMMO:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_BARTNOAMMO));
      pressanykey(STATE_NEWTURN);
      break;

    case STATE_RELOAD:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_FOLKSWANTBLOOD)); lcd.setCursor(0,1);
      lcd.print(getString(STR_SOGIVENEWAMMO));
      bulletsyou = bulletsbart = random(1,4);
      if(distance < 30) distance = 30;
      if(distance > 50) distance = 50;
      nextstate = STATE_NEWTURN;
      state = STATE_KEYWAIT;
      break;

    case STATE_GAMEOVERWON:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_GAMEOVER)); lcd.setCursor(0,1);
      lcd.print(getString(STR_GAMEOVERWIN));
      nextstate = STATE_TITLE;
      state = STATE_KEYWAIT;
      break;

    case STATE_GAMEOVERLOST:
      lcd.clear();
      //         1234567890123456
      lcd.print(getString(STR_GAMEOVER)); lcd.setCursor(0,1);
      lcd.print(getString(STR_GAMEOVERLOST));
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

boolean doinputnumber(int minimum, int maximum, byte button) {
  boolean retval = false;
  if(buttonJustPressed) {
    switch(button) {
      case BUTTON_LEFT:
        if(inputnumber <= minimum) inputnumber = maximum; else inputnumber = inputnumber - 1;
        printinputnumber();  
        break;
      case BUTTON_RIGHT:
        if(inputnumber >= maximum) inputnumber = minimum; else inputnumber = inputnumber + 1;
        printinputnumber();  
        break;
      case BUTTON_SELECT:
        retval = true;
        break;
    }
  }
  return retval;
}

void printinputnumber() {
  lcd.setCursor(4,1);
  if(inputnumber <= 0) lcd.print("   "); else lcd.print("<  ");
  if(inputnumber <= 9) lcd.print(' ');
  lcd.print(inputnumber);
  if(inputnumber <= 99) lcd.print("  >");
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

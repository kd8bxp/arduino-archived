
/* Wumpus Conversion for the Freetronics 16x2 LCD shield
http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9
http://www.atariarchives.org/bcc1/showpage.php?page=247

by eturnerx@gmail.com 201208

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

#define BUTTON_NONE               0  // 
#define BUTTON_RIGHT              1  // 
#define BUTTON_UP                 2  // 
#define BUTTON_DOWN               3  // 
#define BUTTON_LEFT               4  // 
#define BUTTON_SELECT             5  // 

// Define our states
#define STATE_TITLE               00  // A title screen
#define STATE_KEYWAIT             05  // Waiting for a keypress
#define STATE_GO1                 10  // Flavour intro
#define STATE_SENSINGWUMPUS       20  // Situational Awareness: Wumpus
#define STATE_SENSINGBATS         22  // Situational Awareness: Bats
#define STATE_SENSINGPIT          24  // Situational Awareness: Pit
#define STATE_NEWROOM             30  // Just arrived -> print the goodies
#define STATE_HUNTING             31  // Wait for instructions
#define STATE_SHOOT               40  // Setting up the shot
#define STATE_SHOOTREPORT         50  // Doing the shot
#define STATE_BATS                60  // Bat rapid teleport system
#define STATE_PIT                 70  // Oh dear, fell in a pit!
#define STATE_WUMPUSCHOMP         80  // Oh dear, eaten by the wumpus
#define STATE_SUICIDE             85  // Whoops! you shot yourself
#define STATE_GAMEWIN             90  // You won! Yay!
#define STATE_GAMEOVER            98  // Gameover. Option to Shuffle and restart or just restart
#define STATE_GAMEOVERWAIT        99

// Misc. Defines
#define MAXSHOTDISTANCE            6  // How far can a shot travel?

// A datatype that describes a room
typedef struct {
  byte dir1, dir2, dir3;
  char id;
  byte visited;
  byte known;
} room_t;

/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

// Setup the variables that will be used
byte state, nextstate;
byte playerroom, wumpusroom, batsroom, pitroom;
byte startplayerroom, startwumpusroom, startbatsroom, startpitroom;
byte shot[MAXSHOTDISTANCE], shots;
room_t caves[20];

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
  lcd.print( "Hunt the Wumpus!" );
  lcd.setCursor( 0, 1 );
  lcd.print( '.' );
  shuffle();
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
      lcd.print( "Hunt the Wumpus!" );
      lcd.setCursor( 0, 1 );
      lcd.print( "Press [any] key" );
      newgame();
      nextstate = STATE_GO1;
      state = STATE_KEYWAIT;
      break;


    case STATE_KEYWAIT:
      if(buttonJustPressed) state = nextstate;
      break;


    case STATE_GO1:
      if(wumpusroom == playerroom) {
        state = STATE_WUMPUSCHOMP;
      } else if(batsroom == playerroom) {
        state = STATE_BATS;
      } else if(pitroom == playerroom) {
        state = STATE_PIT;
      } else {
        state = STATE_SENSINGWUMPUS;
      }
      break;


    case STATE_SENSINGWUMPUS:
      if(wumpusroom == caves[playerroom].dir1 || wumpusroom == caves[playerroom].dir2 || wumpusroom == caves[playerroom].dir3) {
        outputplayerroom();
        //          1234567890123456
        lcd.print( "You smell wumpus" );
        nextstate = STATE_SENSINGBATS;
        state = STATE_KEYWAIT;
        break;
      } else state = STATE_SENSINGBATS; // and fall through...


    case STATE_SENSINGBATS:
      if(batsroom == caves[playerroom].dir1 || batsroom == caves[playerroom].dir2 || batsroom == caves[playerroom].dir3) {
        outputplayerroom();
        //          1234567890123456
        lcd.print( "You hear wings" );
        nextstate = STATE_SENSINGPIT;
        state = STATE_KEYWAIT;
        break;
      } else state = STATE_SENSINGPIT; // and fall through...


    case STATE_SENSINGPIT:
      if(pitroom == caves[playerroom].dir1 || pitroom == caves[playerroom].dir2 || pitroom == caves[playerroom].dir3) {
        outputplayerroom();
        //          1234567890123456
        lcd.print( "You feel breeze" );
        nextstate = STATE_NEWROOM;
        state = STATE_KEYWAIT;
        break;
      } else state = STATE_NEWROOM; // and fall through...


    case STATE_NEWROOM:
      caves[playerroom].visited = true;
      caves[caves[playerroom].dir1].known = true;
      caves[caves[playerroom].dir2].known = true;
      caves[caves[playerroom].dir3].known = true;
      outputplayerroom();
      //         1234567890123456
      lcd.print("  <");
      lcd.print(    caves[caves[playerroom].dir1].id);
      lcd.print(     "  ^");
      lcd.print(         caves[caves[playerroom].dir2].id);
      lcd.print(         "  ");
      lcd.print(            caves[caves[playerroom].dir3].id);
      lcd.print(             ">  *");
      state = STATE_HUNTING;
      break;


    case STATE_HUNTING:
      if(buttonJustPressed) {
        switch(button) {
          case BUTTON_LEFT:
            playerroom = caves[playerroom].dir1;
            state = STATE_GO1;
            break;
          case BUTTON_UP:
            playerroom = caves[playerroom].dir2;
            state = STATE_GO1;
            break;
          case BUTTON_RIGHT:
            playerroom = caves[playerroom].dir3;
            state = STATE_GO1;
            break;
          case BUTTON_SELECT:
            for(int i = 1; i < MAXSHOTDISTANCE; i++) shot[i] = 255;
            shots = 0;
            shot[0] = playerroom;
            outputshotui();
            state = STATE_SHOOT;
            break;
          default:
            flashbacklight();
            break;
        }
      }
      break;


    case STATE_SHOOT:
      if(buttonJustPressed) {
        room_t r = caves[shot[shots]];
        switch(button) {
          case BUTTON_LEFT:
            if(shots < MAXSHOTDISTANCE && r.visited && caves[r.dir1].known) {
              shots++;
              shot[shots] = r.dir1;
              outputshotui();
            } else flashbacklight();
            break;
          case BUTTON_UP:
            if(shots < MAXSHOTDISTANCE && r.visited && caves[r.dir2].known) {
              shots++;
              shot[shots] = r.dir2;
              outputshotui();
          } else flashbacklight();
            break;
          case BUTTON_RIGHT:
            if(shots < MAXSHOTDISTANCE && r.visited && caves[r.dir3].known) {
              shots++;
              shot[shots] = r.dir3;
              outputshotui();
            } else flashbacklight();
            break;
          case BUTTON_SELECT:
            state = STATE_SHOOTREPORT;
            break;
          default:
            flashbacklight();
            break;
        }
      }
      break;


    case STATE_SHOOTREPORT:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("--Arrow Report--");
      lcd.setCursor(0,1);
      lcd.print(">>--> ");
      nextstate = STATE_GO1;
      state = STATE_KEYWAIT;
      for(int i = 1; i <= shots; i++) {
        room_t r = caves[shot[i]];
        lcd.print(r.id);
        if(shot[i] == playerroom) {
          lcd.print(" HIT");
          nextstate = STATE_SUICIDE;
          state = STATE_KEYWAIT;
          break;
        } else if(shot[i] == wumpusroom) {
          lcd.print(" HIT");
          nextstate = STATE_GAMEWIN;
          state = STATE_KEYWAIT;
          break;
        }
      }
      if(nextstate == STATE_GO1) { // Missed - the Wumpus might move
        switch(random(4)) {
          case 0: break; // Does not move
          case 1: wumpusroom = caves[wumpusroom].dir1; break;
          case 2: wumpusroom = caves[wumpusroom].dir2; break;
          case 3: wumpusroom = caves[wumpusroom].dir3; break;
        }
      }
      break;


    case STATE_BATS:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("Bats fly you to");
      lcd.setCursor(0,1);
      lcd.print("a random room.");
      playerroom = random(20);
      nextstate = STATE_GO1;
      state = STATE_KEYWAIT;
      break;


    case STATE_PIT:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("AAIIIEEEeeee.e..");
      lcd.setCursor(0,1);
      lcd.print("You fell in  pit");
      nextstate = STATE_GAMEOVER;
      state = STATE_KEYWAIT;
      break;


    case STATE_WUMPUSCHOMP:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("Omnomnomnomnomn!");
      lcd.setCursor(0,1);
      lcd.print("Wumpus ate you.");
      nextstate = STATE_GAMEOVER;
      state = STATE_KEYWAIT;
      break;


    case STATE_SUICIDE:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("BOOOOM HEADSHOT!");
      lcd.setCursor(0,1);
      lcd.print("Shot yourself ><");
      nextstate = STATE_GAMEOVER;
      state = STATE_KEYWAIT;
      break;


    case STATE_GAMEWIN:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("Great shot Robin");
      lcd.setCursor(0,1);
      lcd.print("Wumpus is dead");
      nextstate = STATE_GAMEOVER;
      state = STATE_KEYWAIT;
      break;


    case STATE_GAMEOVER:
      lcd.clear();
      lcd.setCursor(0,0);
      //         1234567890123456
      lcd.print("-- GAME  OVER --");
      lcd.setCursor(0,1);
      lcd.print("");
      state = STATE_GAMEOVERWAIT;
      break;


    case STATE_GAMEOVERWAIT:
      if(buttonJustPressed) {
        switch(button) {
          case BUTTON_LEFT:
            shuffle(); // Fall through .....
          case BUTTON_RIGHT:
            newgame();
            break;
          default:
            flashbacklight();
            break;
        }
      }
      break;


    default: break;
  }
  //clear the buttonJustPressed & buttonJustReleased flags; they've already done their job
 if(buttonJustPressed)  buttonJustPressed  = false;
 if(buttonJustReleased) buttonJustReleased = false;
 delay(25);                           // Delay to stop buttons "jittering" false presses
}

void flashbacklight() {
  digitalWrite(LCD_BACKLIGHT_PIN, LOW);  delay(150);
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH); delay(150);
}

void outputplayerroom() {
  lcd.clear();
  lcd.setCursor(0,0);
  //          1234567890123456
  lcd.print( "Room:" );
  lcd.print(       caves[playerroom].id );
  lcd.print(       " Exits:" );
  lcd.print(               caves[caves[playerroom].dir1].id );
  lcd.print(                caves[caves[playerroom].dir2].id );
  lcd.print(                 caves[caves[playerroom].dir3].id );
  lcd.setCursor(0,1);
}

void outputshotui() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SHOOT: ");
  for(byte i = 0; i < MAXSHOTDISTANCE; i++) {
    if(shot[i] == 255) lcd.print('.');
    else lcd.print(caves[shot[i]].id);
  }
  if(shots < MAXSHOTDISTANCE) {
    room_t r = caves[shot[shots]];
    if(r.visited && caves[r.dir1].known) {
      lcd.setCursor(2,1);
      lcd.print('<');
      lcd.print(caves[r.dir1].id);
    }
    if(r.visited && caves[r.dir2].known) {
      lcd.setCursor(6,1);
      lcd.print('^');
      lcd.print(caves[r.dir2].id);
    }
    if(r.visited && caves[r.dir3].known) {
      lcd.setCursor(10,1);
      lcd.print(caves[r.dir3].id);
      lcd.print('>');
    }
  }
  lcd.setCursor(14,1);
  lcd.print('*');
}

void newgame() {
  state = STATE_GO1;
  playerroom = startplayerroom;
  wumpusroom = startwumpusroom;
  batsroom = startbatsroom;
  pitroom = startpitroom;
  for(int i = 0; i < 20; i++) caves[i].visited = caves[i].known = false;
}

void shuffle() {
  int i, j, k;
  char l;
  // Reset the map
  caves[0] = (room_t) { 1,4,5,'A',false,false };
  caves[1] = (room_t) { 0,2,7,'B',false,false };
  caves[2] = (room_t) { 1,3,9,'C',false,false };
  caves[3] = (room_t) { 2,4,11,'D',false,false };
  caves[4] = (room_t) { 0,3,13,'E',false,false };
  caves[5] = (room_t) { 0,6,14,'F',false,false };
  caves[6] = (room_t) { 5,7,15,'G',false,false };
  caves[7] = (room_t) { 1,6,8,'H',false,false };
  caves[8] = (room_t) { 7,9,16,'I',false,false };
  caves[9] = (room_t) { 2,8,10,'J',false,false };
  caves[10] = (room_t) { 9,11,17,'K',false,false };
  caves[11] = (room_t) { 3,10,12,'L',false,false };
  caves[12] = (room_t) { 11,13,18,'M',false,false };
  caves[13] = (room_t) { 4,12,14,'N',false,false };
  caves[14] = (room_t) { 5,13,19,'O',false,false };
  caves[15] = (room_t) { 6,16,19,'P',false,false };
  caves[16] = (room_t) { 8,15,17,'Q',false,false };
  caves[17] = (room_t) { 10,16,18,'R',false,false };
  caves[18] = (room_t) { 12,17,19,'S',false,false };
  caves[19] = (room_t) { 14,15,18,'T',false,false };
  // Place the player, wumpus, bats and the pit
  startplayerroom = random(20);
  do { startwumpusroom = random(20); } while (startwumpusroom == startplayerroom);
  do { startbatsroom = random(20); } while (startbatsroom == startwumpusroom || startbatsroom == startplayerroom);
  do { startpitroom = random(20); } while (startpitroom == startbatsroom || startpitroom == startwumpusroom || startpitroom == startplayerroom);
  // Mix up the room IDs
  for(i = 0; i < 500; i++) {
    j = random(20);
    do { k = random(20); } while( k == j );
    l = caves[j].id;
    caves[j].id = caves[k].id;
    caves[k].id = l;
  }
  // Re-alphabetasize the room IDs
  for(i = 0; i < 20; i++) {
    if(caves[caves[i].dir3].id < caves[caves[i].dir2].id) { j = caves[i].dir2; caves[i].dir2 = caves[i].dir3; caves[i].dir3 = j; }
    if(caves[caves[i].dir2].id < caves[caves[i].dir1].id) { j = caves[i].dir1; caves[i].dir1 = caves[i].dir2; caves[i].dir2 = j; }
    if(caves[caves[i].dir3].id < caves[caves[i].dir2].id) { j = caves[i].dir2; caves[i].dir2 = caves[i].dir3; caves[i].dir3 = j; }
  }
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
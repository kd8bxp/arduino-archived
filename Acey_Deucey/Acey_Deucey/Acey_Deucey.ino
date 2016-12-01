/* Acecy Deucey for the Freetronics 16x2 LCD shield
 http://www.freetronics.com/pages/16x2-lcd-shield-quickstart-guide#.UBKMm7T9OK9
 http://www.atariarchives.org/basicgames/showpage.php?page=2
 
 Loosely based on the original game, though changed for a heads up format.
 Similar to Red Dog Poker, but played heads rather than vs the house
 
 by eturnerx@gmail.com 201210
 
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
#define STATE_TURNSTART           20  // The turn begins
#define STATE_OPTION              30  // Getting input
#define STATE_AITURN              40  // The AI gives it a go
#define STATE_GAMEOVER            99

// Misc. Defines
#define DECKRESHUFFLE             38  // How many cards dealt (more than or equal) triggers a reshuffle
#define MSGFLASHDELAY           1300  // How long to flash messages for (ms)?

/*--------------------------------------------------------------------------------------
 Variables
 --------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

// Setup the variables that will be used
byte state, nextstate;
byte deck[52], card1, card2, card3;
const char ranklabels[13] = { 
  '2','3','4','5','6','7','8','9','T','J','Q','K','A' };
int pot, ante, turn, cashplayer, cashai, deckpos, bet;
byte betdregs;                           //for tracking "sub ante" bets caused by pot or player stack being too small

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
  lcd.print( "_ Acey  Deucey _" );
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
    lcd.print( "= Acey  Deucey =" );
    shuffle();
    pot = turn = 0;
    ante = 1;
    cashplayer = cashai = 50;
    lcd.setCursor( 0, 1 );
    lcd.print( "Press [any] key" );
    nextstate = STATE_TURNSTART;
    state = STATE_KEYWAIT;
    break;

  case STATE_KEYWAIT:
    if(buttonJustPressed) state = nextstate;
    break;

  case STATE_TURNSTART:
    turn++; 
    bet = betdregs = 0;
    printStatusLine();

    if(turn % 6 == 5 && ante < 20) {
      ante++;
      lcd.clear();
      lcd.print("ANTE INCREASE");
      lcd.setCursor(0,1);
      lcd.print("Ante is now: ");
      lcd.print(ante);
      flashbacklight();
      delay(MSGFLASHDELAY);
    }

    if(deckpos >= DECKRESHUFFLE) {
      shuffle();
      lcd.clear();
      lcd.print("*DECK  SHUFFLED*");
      flashbacklight();
      delay(MSGFLASHDELAY);
    }      

    if(cashplayer == 0) {
      lcd.clear();
      lcd.print("You have no ANTE");
      lcd.setCursor(0,1);
      lcd.print(">>  YOU LOSE  <<");
      state = STATE_GAMEOVER;
      return;
    } 
    else if(cashplayer <= ante) {
      printStatusLine();
      lcd.print("*You are ALL-IN*");
      pot += cashplayer;
      cashplayer = 0;
      flashbacklight();
      delay(MSGFLASHDELAY);
    } 
    else {
      printStatusLine();
      lcd.print("ANTE ");
      lcd.print(ante);
      lcd.print("  ");
      lcd.print("ANTE ");
      lcd.print(ante);
      pot += ante;
      cashplayer -= ante;
    }
    printStatusLine();
    delay(MSGFLASHDELAY);

    card1 = deck[deckpos]; 
    deckpos++;
    card2 = deck[deckpos]; 
    deckpos++;
    if(card1 > card2) {
      byte tmp = card1;
      card1 = card2;
      card2 = tmp;
    }

    //         1234567890123456
    //         Cards:TQ  Bet:00
    lcd.print("Cards:");
    lcd.print(ranklabels[card1]);
    lcd.print(ranklabels[card2]);
    lcd.print("  Bet:");
    if(bet < 10) lcd.print('0');
    lcd.print(bet);
    state = STATE_OPTION;
    break;

  case STATE_OPTION:
    if(cashplayer == 0) {
      buttonJustPressed = true;
      button = BUTTON_SELECT;
    }
    if(buttonJustPressed) {
      switch(button) {
      case BUTTON_LEFT: 
      case BUTTON_DOWN:
        if(bet > 0) {
          if(betdregs > 0) {
            bet -= betdregs;
            betdregs = 0;
          } 
          else bet -= ante;
        }
        break;
      case BUTTON_RIGHT: 
      case BUTTON_UP:
        if(bet < cashplayer && bet < pot) {
          bet += ante;
          if(bet > cashplayer) {
            betdregs = (cashplayer + ante) - bet;
            bet = cashplayer;
          }
          if(bet > pot) {
            betdregs = (pot + ante) - bet;
            bet = pot;
          }
        }
        break;
      case BUTTON_SELECT:
        lcd.clear();
        if(bet == 0 && cashplayer > 0) {
          lcd.print("You pass....");
          delay(MSGFLASHDELAY*1.5);
          state = STATE_AITURN;
        }  
        else {
          card3 = deck[deckpos]; 
          deckpos++;
          lcd.print("Drawing a card");
          lcd.setCursor(0,1);
          lcd.print(ranklabels[card1]);
          lcd.print("  -  ");
          lcd.print(ranklabels[card2]);
          delay(MSGFLASHDELAY/2);
          lcd.setCursor(3,1);
          lcd.print('/');
          delay(MSGFLASHDELAY/2);
          lcd.setCursor(3,1);
          lcd.print('-');
          delay(MSGFLASHDELAY/2);
          lcd.setCursor(3,1);
          lcd.print(ranklabels[card3]);
          lcd.setCursor(9,1);
          if(card1 < card3 && card2 > card3) {
            pot -= bet;
            cashplayer += bet;
            lcd.print("Win: ");
            lcd.print(bet);
            lcd.setCursor(2,1);
            lcd.print('>');
            lcd.setCursor(4,1);
            lcd.print('<');
          } 
          else {
            pot += bet;
            cashplayer -= bet;
            lcd.print("Lose:");
            if(bet<10) lcd.print(' ');
            lcd.print(bet);
            lcd.setCursor(2,1);
            if(card3 > card1) lcd.print('<'); 
            else if(card3 < card1) lcd.print('>'); 
            else lcd.print('=');
            lcd.setCursor(4,1);
            if(card3 > card2) lcd.print('>'); 
            else if(card3 < card2) lcd.print('<'); 
            else lcd.print('=');
          }
          delay(MSGFLASHDELAY);
          state = STATE_AITURN;
        }
        break;
      }
    }
    if(state == STATE_OPTION) {
      lcd.setCursor(14,1);
      if(bet < 10) lcd.print(' ');
      lcd.print(bet);
    }
    break;

  case STATE_AITURN:
    if(deckpos >= DECKRESHUFFLE) {
      shuffle();
      lcd.clear();
      lcd.print("*DECK  SHUFFLED*");
      flashbacklight();
      delay(MSGFLASHDELAY);
    }      

    if(cashai == 0) {
      lcd.clear();
      lcd.print("Arduino is BROKE");
      delay(MSGFLASHDELAY);
      lcd.setCursor(0,1);
      lcd.print(">>  YOU WIN!  <<");
      state = STATE_GAMEOVER;
      return;
    } 
    else if(cashai <= ante) {
      lcd.clear();
      lcd.print("Arduino's ALL-IN");
      pot += cashai;
      cashai = 0;
      flashbacklight();
      delay(MSGFLASHDELAY);
    } 
    else {
      pot += ante;
      cashai -= ante;
    }
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);

    card1 = deck[deckpos]; 
    deckpos++;
    card2 = deck[deckpos]; 
    deckpos++;
    if(card1 > card2) {
      byte tmp = card1;
      card1 = card2;
      card2 = tmp;
    }

    // Okay now what'll the AI do?
    // In a clean deck, bet when the spread is seven or more
    // Bet more when the spread is larger
    lcd.setCursor(0,0);
    lcd.print("Arduino's Turn  ");
    lcd.setCursor(0,1);
    lcd.print(ranklabels[card1]);
    lcd.print("  -  ");
    lcd.print(ranklabels[card2]);
    lcd.setCursor(9,1);
    if(card2 - card1 < 7 && cashai > 0) {
      lcd.print("PASS!");
    } 
    else {
      // How much to bet?
      bet = (card2 - card1) * ante;
      if(bet > int(cashai/2) && cashai > (ante * 2)) {
        bet = int(cashai/2);
        bet -= (bet % ante);
      }
      if(bet > cashai) bet = cashai;
      if(bet > pot) bet = pot;
      lcd.print("BET:");
      lcd.print(bet);
      lcd.setCursor(0,0);
      lcd.print("Drawing a card  ");
      lcd.setCursor(0,1);
      card3 = deck[deckpos]; 
      deckpos++;
      delay(MSGFLASHDELAY/2);
      lcd.setCursor(3,1);
      lcd.print('/');
      delay(MSGFLASHDELAY/2);
      lcd.setCursor(3,1);
      lcd.print('-');
      delay(MSGFLASHDELAY/2);
      lcd.setCursor(3,1);
      lcd.print(ranklabels[card3]);
      lcd.setCursor(9,1);
      if(card1 < card3 && card2 > card3) {
        pot -= bet;
        cashai += bet;
        lcd.print("Win: ");
        lcd.print(bet);
        lcd.setCursor(2,1);
        lcd.print('>');
        lcd.setCursor(4,1);
        lcd.print('<');
      } 
      else {
        pot += bet;
        cashai -= bet;
        lcd.print("Lose:");
        if(bet<10) lcd.print(' ');
        lcd.print(bet);
        lcd.setCursor(2,1);
        if(card3 > card1) lcd.print('<'); 
        else if(card3 < card1) lcd.print('>'); 
        else lcd.print('=');
        lcd.setCursor(4,1);
        if(card3 > card2) lcd.print('>'); 
        else if(card3 < card2) lcd.print('<'); 
        else lcd.print('=');
      }
    }
    delay(MSGFLASHDELAY);
    state = STATE_TURNSTART;
    break;

  case STATE_GAMEOVER:
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

void printStatusLine() {
  lcd.clear();
  //         1234567890123456
  if(cashai < 10) lcd.print('0');
  lcd.print(cashai);
  lcd.print(":A");
  if(pot < 10) lcd.print("   0");
  else lcd.print("   ");
  lcd.print(pot);
  lcd.print("   U:");
  if(cashplayer < 10) lcd.print('0');
  lcd.print(cashplayer);
  lcd.setCursor(0,1);
}

void shuffle() {
  // Setup a clean deck
  for(int suit = 0; suit < 4; suit++) {
    for(byte rank = 0; rank < 13; rank++) {
      deck[(suit * 13) + rank] = rank;
    }
  }

  // Shuffle  
  //  First pass ... each position is shuffled
  for(int pos1 = 0; pos1 < 52; pos1++) {
    int pos2 = random(52);
    byte tmp = deck[pos1];
    deck[pos1] = deck[pos2];
    deck[pos2] = tmp;
  }
  //  Second pass is a series of random swaps
  for(int cnt = 0; cnt < 1000; cnt++) {
    int pos1 = random(52);
    int pos2 = random(52);
    byte tmp = deck[pos1];
    deck[pos1] = deck[pos2];
    deck[pos2] = tmp;
  }
  deckpos = 0;
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

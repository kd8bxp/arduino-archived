#include "keyboard.h"
#define F_CPU 8000000UL // 8MHz with internal RC-Osciallator
#include <util/delay.h>
#include "display.h"

int blitzMode=0;  // *0 toggles to 1, to speed up chess moves (and dumb down play)
/**
*
* @file keyboard.c
*
* @brief keyboard test
*
* @author ChrisMicro
* @copyright (c) 2014 ChrisMicro
*
* The CH2-Computer can be used as a shield for Ardduinos with an Atmega168 or Atmega328
* microcontroller ( mainly Arduino Uno ).
*
	keyboard pin connection

	pin1: PB2
	pin2: PD3
	pin3: PB3
	pin4: PD6
	pin5: PB0
	pin6: PD5
	pin7: PD4


	KEYBOARDTYPE1:
	keyboard pin layout for mulicomp keypad ( used in v0.1 )

	key line driver active high PD3, PD4, PD5, PD6
	key line inputs PB0, PB2, PB3,

	keys 1 ,2 ,3 : PD3, row 0
	Keys 4 ,5 ,6 : PD4, row 1
	keys 7 ,8 ,9 : PD5, row 2
	keys * ,0 ,# : PD6, row 3
	PB3 PB2 PB0

	KEYBOARDTYPE2:
	keyboard pin layout for for S512J-BF7KUV

	keys 1 ,2 ,3 : PD6, row 0, display cathode
	Keys 4 ,5 ,6 : PB0, row 1, display anode
	keys 7 ,8 ,9 : PD5, row 2, display cathode
	keys * ,0 ,# : PD4, row 3, display cathode
	PB2 PD2 PB3
	A C A

	meaning of second key level
	char KeyMatrix2[4][3]={
	  'A','B','C',
	  'D','E','F',
	  'r','g','s', // r: reset, g: go, s: set address
	  '*','0','#'
	};


	! The keyboard type is selected in "keyboard.h" !

*/
// row 0..3
void setKeyRow(uint8_t row)
{
  if(row<4)
  {
	  // display lines
	  DDRC|=(1<<3); // set line as output
	  PORTC&=~(1<<3); // discharge
	  DDRD|=(1<<2); // set line as output
	  PORTD&=~(1<<2); // discharge
	  DDRB|=((1<<4)|(1<<5)); // set line as output
	  PORTB&=~((1<<4)|(1<<5)); // discharge

	  // key lines
	  PORTB&=~((1<<0)|(1<<2)|(1<<3)); // discharge
	  DDRB=((1<<0)|(1<<2)|(1<<3)); // set all key lines as output

	  PORTD&=~((1<<3)|(1<<4)|(1<<5)|(1<<6)); // discharge and make sure pull ups are turned off
	  DDRD=((1<<3)|(1<<4)|(1<<5)|(1<<6)); // set all selector lines as output

	  DDRD&=~((1<<3)|(1<<4)|(1<<5)|(1<<6)); // set all selector lines to input
	  DDRB&=~((1<<0)|(1<<2)|(1<<3)); // set all key lines as input

	  // display lines
	  DDRC&=~(1<<3); // line as input
	  DDRD&=~(1<<2); // line as input
	  DDRB&=~((1<<4)|(1<<5)); // set line as input

	  // key row selection
	  DDRD|=(1<<(3+row)); // set the row key line to output
	  PORTD|=(1<<(3+row)); // set row key pin to high
  }
}

#define KEYSCANDELAY 2 //1 got me a distorted LED matrix.
void delayUs(uint8_t us)
{
	uint8_t n;
	for(n=0;n<us;n++) _delay_us(KEYSCANDELAY);
}

uint8_t getKeyColumnPattern()
{
  uint8_t result=0;

  PORTB&=~((1<<3)|(1<<2)|(1<<0)); // set not used multiplexer lines to 0

  DDRB|=((1<<3)|(1<<2)); // set multplexer lines as outputs
  DDRB&=~(1<<0); // set the selected line as input
  delayUs(KEYSCANDELAY);
  if(PINB&(1<<0)) result|=(1<<0) ;


  PORTB&=~((1<<3)|(1<<2)|(1<<0)); // set not used multiplexer lines to 0

  DDRB|=((1<<3)|(1<<0)); // set multplexer lines as outputs
  DDRB&=~(1<<2); // set the selected line as input
  delayUs(KEYSCANDELAY);
  if(PINB&(1<<2)) result|=(1<<1) ;


  PORTB&=~((1<<3)|(1<<2)|(1<<0)); // set not used multiplexer lines to 0

  DDRB|=((1<<2)|(1<<0)); // set multplexer lines as outputs
  DDRB&=~(1<<3); // set the selected line as input
  delayUs(KEYSCANDELAY);
  if(PINB&(1<<3)) result|=(1<<2) ;

  return result;
}

void highImpedance()
{
	  // display lines
	  DDRC&=~(1<<3);
	  PORTC&=~(1<<3);
	  DDRD&=~(1<<2);
	  PORTD&=~(1<<2);

	  DDRB&=~((1<<4)|(1<<5)|(1<<7));
	  PORTB&=~((1<<4)|(1<<5)|(1<<7));

	  // keyboard lines
	  DDRD&=~((1<<3)|(1<<4)|(1<<5)|(1<<6)); // set all selector lines to input
	  PORTD&=~((1<<3)|(1<<4)|(1<<5)|(1<<6)); // discharge and make sure pull ups are turned off

	  DDRB&=~((1<<0)|(1<<2)|(1<<3)); // set all key lines as input
	  PORTB&=~((1<<0)|(1<<2)|(1<<3)); // discharge
}

// return the number of the key pressed
uint8_t scanKey1()
{
	uint8_t n,value,result=0,tmp;
	n=4;

	do
	{
		n--;
		setKeyRow(n);
		value=getKeyColumnPattern();
		if(value>0) result=3;
		if(value>1) result=2;
		if(value>2) result=1;

		if(result>0)break;

	}while(n>0);

	tmp=result+n*3;
	#ifdef KEYBOARDTYPE1
		if(tmp==0)tmp=0xFF;
		if(tmp==11)tmp=0;
	#endif

	return tmp;
}
//check keys 4,6 ( 5 reacts also like key2 )
uint8_t checkKey46()
{
	uint8_t result=0xFF; // no key

	highImpedance();
	DDRB|=(1<<0); //  row 1 as output
	// if the key is pressed, column 1 and 2 should be discharged
	PORTB|=(1<<2)|(1<<3); // set pull up column 1,3
	delayUs(5);
	if((PINB&(1<<2))==0)result=4;
	if((PINB&(1<<3))==0)result=6;

	highImpedance();
	return result;
}

// check keys 0,2,8
uint8_t checkKey028()
{
	uint8_t result=0xFF; // no key
	highImpedance();

	DDRD|=(1<<3);  // PD3 as output
	PORTD|=(1<<3); // set to high

	PORTD&=~((1<<4)|(1<<5)|(1<<6)); // set all lines to zero
	DDRD|=((1<<4)|(1<<5)|(1<<6));   // output, discharge
	DDRD&=~((1<<4)|(1<<5)|(1<<6));  // set to input
	delayUs(5);
	if(PIND&(1<<4)) result=0;
	if(PIND&(1<<5)) result=8;
	if(PIND&(1<<6)) result=2;

	highImpedance();
	return result;
}

// driver for KEYBOARDTYPE2
uint8_t scanKey2()
{
	uint8_t tmp;
	static uint8_t state=0,value=0xFF;
/*
	value=scanKey1();
	uint8_t kcode[]={0xFF,0xFF,0xFF,5,12,10,0xFF,9,7,0xFF,3,1 };
	value=kcode[value];
	if(value==0xFF) value=checkKey028();
	if(value==0xFF) value=checkKey46();
*/


	switch(state)
	{
		case 0:{
			tmp=scanKey1();
			uint8_t kcode[]={0xFF,0xFF,0xFF,5,12,10,0xFF,9,7,0xFF,3,1 };
			tmp=kcode[tmp];
			if(tmp==0xFF)
			{
				state=1;
			}else
			{
				value=tmp;
			}
		}break;
		case 1:{
			value=checkKey028();
			if(value==0xFF)	value=checkKey46();
			state=0;
		}break;
	}

	return value;
}

/**************************************************************************************

   Scan the matrix keyboard and detect keys. This routine is none blocking.

   return value:
   uint8_t =   NOKEY, STARKEY, HASHKEY, 0..9

**************************************************************************************/
uint8_t scanKey()
{
  uint8_t ddrc;
  uint8_t ddrd;
  uint8_t ddrb;
  uint8_t temp=NOKEY;
  static uint8_t oldKey;


  // save port configuration
  ddrb=DDRB;
  ddrc=DDRC;
  ddrd=DDRD;

	#ifdef KEYBOARDTYPE1
	  temp=scanKey1();
	#endif

	#ifdef KEYBOARDTYPE2

		oldKey=scanKey2();
		// key filter
		if(oldKey!=NOKEY)
		{
			temp=scanKey2();
			if(temp!=oldKey)
			{
				oldKey=temp;
				temp=scanKey2();
			}
			if(temp!=oldKey)
			{
				oldKey=temp;
				temp=scanKey2();
			}
			if(temp!=oldKey)
			{
				oldKey=temp;
				temp=scanKey2();
			}
		}
		if((temp>12)&&(temp<0xF0))temp=2; 	// dirty fix, remove this line later !!!
	#endif

  // restore port configuration
  DDRB=ddrb;
  DDRC=ddrc;
  DDRD=ddrd;

  if(temp==10) return STARKEY; // '*'
  //if(temp==11) return 0; // zero
  if(temp==12) return HASHKEY; // '#'
  return temp;

}
/**************************************************************************************

  Check if a key is pressed on keyboard.
  Copy the key value after the rising edge of the key pressing to the global
  variable "KeyValue".

  output:
    setting global variable "KeyValue" on rising edge

  return value:
    uint8_t 0 if no key pressed
            1 if key pressed

**************************************************************************************/
#define KEYRELEASED 0
#define KEYPRESSED 1

static uint8_t KeyState=KEYRELEASED;
static uint8_t KeyValue=NOKEY;

uint8_t _keyPressed()
{
  uint8_t temp=0;
  static uint8_t oldKey=0;
  temp=scanKey();
  switch(KeyState)
  {
    case KEYRELEASED:{
      oldKey=NOKEY;
      if(temp!=NOKEY) {
        KeyValue=temp;
        KeyState=KEYPRESSED;  // ==> KEYPRESSED
      }
    }break;

    case KEYPRESSED:{
      if(temp==NOKEY) KeyState=KEYRELEASED;  // ==> KEYPRESSED
      else if(temp!=oldKey){
        oldKey=temp;
      }
    }break;
  }

  if(temp==NOKEY)return 0;
  else return 1;
}
static uint8_t LastKey=NOKEY;

uint8_t keyHit()
{
  _keyPressed();
  LastKey=KeyValue;
  if(KeyValue!=NOKEY)
  {
    KeyValue=NOKEY;
    return 1;
  }else return 0;
}

uint8_t _keyHit()
{
  if(keyHit())
  {
    if(LastKey==STARKEY)return 0; // ignore shift key
    return 1;
  }else  return 0;
}
uint8_t _getKey()
{
  return LastKey;
}
/**************************************************************************************

  uint8_t __keyPressed()

  get a key and return it's code

  the shift key is used to switch to the next layer.

  return:
    0..16       ( 0-0xF )
    17: shift 7
    18: shift 8
    19: shift 9
    0xFD: HASHKEY

**************************************************************************************/
#define START_STATE 0
#define KEYPRESSED 1
#define SHIFTPRESSED 2
uint8_t __keyPressed()
{
  uint8_t temp;
  static uint8_t state=START_STATE;
  //keyHit();
  temp=_getKey();
  //Serial.println(temp);

  switch(state)
  {
    case START_STATE:{
      //Serial.println("START_STATE");
      if(temp==STARKEY) {
        state=SHIFTPRESSED;                     //==> SHIFTPRESSED
        temp=NOKEY;
        //Serial.println("");
      }
    }break;

    case SHIFTPRESSED:{
     //Serial.println(temp+10);
      temp+=10; // next key layer
      state=START_STATE;                        //==> START_STATE
    }break;

  }
  return temp;
}
/**************************************************************************************

  Check if a key was pushed.
  Pushing the '*' key will not be recognized as pressed key. This key is used as
  "shift key" for the second layer of the keyboard.

  return value:
    uint8_t 0 if no key is pressed
            1 if key is pressed

**************************************************************************************/
static uint8_t LastInKey;
uint8_t keyPressed()
{
  uint8_t temp;
  if(keyHit())
  {

    temp=__keyPressed();
    if(temp!=STARKEY) LastInKey=temp;
    else LastInKey=NOKEY;
    if(LastInKey==NOKEY) return 0;
    else return 1;
  }
  return 0;
}

/**************************************************************************************

  Return value from the last pressed key.

  return value:
    key

**************************************************************************************/
uint8_t getKey()
{
  return LastInKey;
}

uint8_t getChar()
{
uint8_t n,c;

n= LastInKey;

  if(n<10)c=n+48; // 0..9
  if((n>=11)&&(n<17))c=n+'A'-11; // A..F
  if(n==HASHKEY) c=13; // Return
  if(n==17)c='C'; // Clear board, reset
  if(n==18)c='E'; // Exchange sides
  if(n==19)c='P'; // Play - ie, let Microchess make its move
  
  if (n==16)      // *6 = F = Toggle Blitz mode (speeds play up 10x, dumbs down too)
  {
    c='W';        // W? Command to enter BlitzMode. Not in 6502 code but picked up by emulator.
/* code below redundant, now handled in cpu.c
if (blitzMode==0)
    {  blitzMode=1;
 //     tone(9, 1760,5); // 1760=NOTE_A6    
    }
    else
    { blitzMode=0;
//      tone(9, 1047,5); // 1047=NOTE_C6
    }
*/
  }

  if (n==14)      // *4
    c='L';        // L? Command to load board. Not in 6502 code but picked up by emulator.


  if (n==15)      // *5
    c='S';        // S? Command to load board. Not in 6502 code but picked up by emulator.
  
  return c;
}
/**************************************************************************************

  Wait for key pressed and return its code.

  return value:
    keyCode
    0..16       ( 0-0xF )
    17: shift 7
    18: shift 8
    19: shift 9
    0xFD: HASHKEY

**************************************************************************************/
uint8_t getKeyCode()
{
  uint8_t n;
  do
  {
    showMatrix(20);
  }while(!keyPressed());

  n=getKey();

  return n;
}
/**************************************************************************************

  Wait for key pressed and return its ASCII code

  return value:
    ASCII code

**************************************************************************************/
int8_t _getchar()
{
  uint8_t n;
  int8_t c;
  n=getKeyCode();
  if(n<10)c=n+48; // 0..9
  if((n>=11)&&(n<17))c=n+'A'-11; // A..F
  if(n==HASHKEY) c='i'; // increment
  if(n==17)c='r'; // reset
  if(n==18)c='g'; // go
  if(n==19)c='s'; // set address
 
  return c;
}

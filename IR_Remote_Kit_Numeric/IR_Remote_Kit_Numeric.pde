/* YourDuino.com Example Software Sketch
 IR Remote Kit Test: Returns numeric value for button pressed
 Uses YourDuino.com IR Infrared Remote Control Kit V2
 http://arduino-direct.com/sunshop/index.php?l=product_detail&p=153
 based on code by Ken Shirriff - http://arcfn.com
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include "IRremote.h"
#include "IRremoteInt.h"

/*-----( Declare Constants )-----*/
#define  REPEAT_DELAY  500   // Delay before checking for another button / repeat
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11
                   // NOTE: Other pins can be used, except pin 3 and 13
                  
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

/*-----( Declare Variables )-----*/
int  ButtonValue;  // 0..9,100,200, top 9 buttons encoded 10..18, -1 Bad Code


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("YourDuino.com IR Infrared Remote Control Kit V2");  
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
//    Serial.println(results.value, HEX); // UN Comment to see raw values
    ButtonValue = translateIR(); 
    Serial.println(ButtonValue, DEC);
    delay(REPEAT_DELAY);    // Adjust for repeat / lockout time
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
int translateIR() // returns value of "Car MP3 remote IR code received

{

 switch(results.value)

  {

  case 0xFFA25D:  
    return 10;  // CH-
    break;

  case 0xFF629D:  
    return 11; // CH
    break;

  case 0xFFE21D:  
    return 12; // CH+
    break;

  case 0xFF22DD:  
    return 13; // PREV
    break;

  case 0xFF02FD:  
    return 14; // NEXT
    break;

  case 0xFFC23D:  
    return 15; //  PLAY/PAUSE     
    break;

  case 0xFFE01F:  
    return 16; // VOL-
    break;

  case 0xFFA857:  
    return 17; // VOL+ 
    break;

  case 0xFF906F:  
    return 18; // EQ 
    break;

  case 0xFF6897:  
    return 0; // ZERO
    break;

  case 0xFF9867:  
    return 100; // 100+ 
    break;

  case 0xFFB04F:  
    return 200; // 200+ 
    break;

  case 0xFF30CF:  
    return 1;  // 1 etc.. to 9
    break;

  case 0xFF18E7:  
    return 2; 
    break;

  case 0xFF7A85:  
    return 3; 
    break;

  case 0xFF10EF:  
    return 4;  
    break;

  case 0xFF38C7:  
    return 5; 
    break;

  case 0xFF5AA5:  
    return 6; 
    break;

  case 0xFF42BD:  
    return 7; 
    break;

  case 0xFF4AB5:  
    return 8;  
    break;

  case 0xFF52AD:  
    return 9; // 9 
    break;

  case 0xFFFFFFFF:  
    return -2; // REPEAT: Button Held down longer than 
    break;
  default: 
    return -1; // Other Button  / Bad Code

  } //END case

} //END translateIR



/* ( THE END ) */


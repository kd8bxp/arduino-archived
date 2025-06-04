/******************************************
 * 
 * This program is meant to demonstrate how to use a NEC IR remote with an UNO board.
 * 
 * A LED array wired to the UNO is controlled remotely via a NEC TV remote controller. The LED array can be turned off, on, display sequences, and light user-speciifed LEDS
 * 
 * This projects uses four LEDs and four 8.2Ω (or simmilar) resistors for the LED array, a TSOP4838 (or compatable) IR reciever, a (ideally) 100Ω resistor, and a (ideally) 0.1uF capacitor for the IR reciever and an UNO (obviously)
 * 
 * Sam Damron, RabbitRobots.com 2017
 * 
 ******************************************/

/*-----( Import required libraries )-----*/
#include <IRremote.h> //Library for using decoding signals from IR recievers and using them in general

/*-----( Define constants )-----*/
#define LED_0_PIN 8 //Macros representing the pin each component is attatched to on the UNO
#define LED_1_PIN 9
#define LED_2_PIN 10
#define LED_3_PIN 11
#define IR_RECV_PIN 2

#define ALL_ON 0 //Macros representing the current display mode for the LED array
#define ALL_OFF 1
#define TARGET_MODE 2
#define PATTERN_0_MODE 3
#define PATTERN_1_MODE 4
#define PATTERN_2_MODE 5

/*
* Constants representing different patterns for the LED array to display
* Each "frame" is stored in one "nibble" (four bits, or half a byte of data) with the left-most bit attatched to the
* first LED and the right-most bit to the last. Each pattern is read right-to-left by frame.
*
* It is important that all the data types are unsigned (bytes are unsigned by default) and hold the exact length of the data sequence
* to prevent blank frames from being added to the end of the sequence or sign bits interfering with frames.
* 
*/
const unsigned short PATTERN_0 = 0x8421; //In binary: 1000 0100 0010 0001
const unsigned long PATTERN_1 = 0x12488421; //In binary: 0001 0010 0100 1000 1000 0100 0010 0001
const byte PATTERN_2 = 0x69; //In binary: 0110 1001

//NEC IR codes representing different buttons on the remote. Checked with the decoded results from the IR reciever
const long POWER_CODE = 0xFFA25D; //The "power" button
const long TARGET_CODE = 0xFF22DD; //The "play" button
const long LEFT_CODE = 0xFF02FD; //The "rewind" button
const long RIGHT_CODE = 0xFFC23D; //The "fast-forward" button
const long PATTERN_0_CODE = 0xFF30CF; //The "1" button
const long PATTERN_1_CODE = 0xFF18E7; //The "2" button
const long PATTERN_2_CODE = 0xFF7A85; //The "3" button

/*-----( Declare variables )-----*/
int displayMode = ALL_ON; //Variable holding the current display mode for the LED array
int targetLED = 0; //Variable holding the LED to be lit in "target" mode
int pointer = 0; //Variable holding the bit to start from when reading off pattern data
long lastTime = 0; //Variable used for timing LED sequences

/*-----( Declare objects )-----*/
IRrecv irReceiver(IR_RECV_PIN); //Declare the IR receiver that will be used, attatched to its respectve pin
decode_results irDecoder; //Declare the IR decoder object that will convert the data from the reciever into a hex code

void setup() { /*----( SETUP: RUNS ONCE )----*/

  Serial.begin(9600); //Open up a serial port at 9600 baud for debugging (if wanted)
  
  irReceiver.enableIRIn(); //Enable IR signals to be read by the receiver

  pinMode(LED_0_PIN,OUTPUT); //Set the pin mode for the four LED pins to be output so digital I/O can be applied.
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);

  lastTime = millis(); //Update lastTime to the current time in milliseconds
  
}

/*--(end setup )---*/



void loop() { /*----( LOOP: RUNS CONSTANTLY )----*/

  if(irReceiver.decode(&irDecoder)){//Decode the signal stored in the receiver's memory buffer, and do the following if the data is a valid code:
    
    //Serial.println(irDecoder.value); //Uncomment to print out each IR code recieved

    if(irDecoder.value == POWER_CODE && displayMode != ALL_OFF){ //If a power button press was detected, and the LED array is currently on in some form, turn the LED array off
      displayMode = ALL_OFF;
    }

    else if(irDecoder.value == POWER_CODE && displayMode == ALL_OFF){//Otherwise, if the power button was pressed and the LED array is off, turn the entire array on.
      displayMode = ALL_ON;
    }

    else if(irDecoder.value == TARGET_CODE){ //If the "Play" button press was detected, put the LED array into target mode.
      displayMode = TARGET_MODE;
    }

    else if(irDecoder.value == LEFT_CODE && displayMode == TARGET_MODE){ //If the LED array is currently in target mode, and a "rewind" button press was detected, move the active LED left.
      targetLED--; //Decrement the active LED by one (Move active LED left one)
      targetLED = constrain(targetLED, 0, 3); //Set the active LED to within the bounds of the LED array
    }

    else if(irDecoder.value == RIGHT_CODE && displayMode == TARGET_MODE){ //If the LED array is currently in target mode, and a "fast forward" button press was detected, move the active LED right.
      targetLED++; //Increment the active LED by one (Move active LED right one)
      targetLED = constrain(targetLED, 0, 3); //Set the active LED to within the bounds of the LED array
    }

    else if(irDecoder.value == PATTERN_0_CODE){ //If a "1" button press was detected, start running pattern 0 on the LED array
      displayMode = PATTERN_0_MODE;
    }

    else if(irDecoder.value == PATTERN_1_CODE){ //If a "2" button press was detected, start running pattern 1 on the LED array
      displayMode = PATTERN_1_MODE;
    }

    else if(irDecoder.value == PATTERN_2_CODE){ //If a "3" button press was deteced, start running pattern 2 on the LED array
      displayMode = PATTERN_2_MODE;
    }

    irReceiver.enableIRIn(); //Since the decoder is done reading the signal from the reciever's memory buffer, allow the reciever to write data to that buffer again.
  }

  switch(displayMode){ //Do one of the following blocks of code depedning on the current display mode:

    default:
    case ALL_ON: //Block of code for the ALL_ON display mode (case ALL_ON:) and any unrecognized display modes (default:)
      
      digitalWrite(LED_0_PIN, HIGH); //Send voltage to the 1st LED
      digitalWrite(LED_1_PIN, HIGH); //Send voltage to the 2nd LED
      digitalWrite(LED_2_PIN, HIGH); //Send voltage to the 3rd LED
      digitalWrite(LED_3_PIN, HIGH); //Send voltage to the 4th LED
      
      break; //Exit block of code

  case ALL_OFF: //Block of code for the ALL_OFF display mode
    
    digitalWrite(LED_0_PIN, LOW); //Don't send voltage to the 1st LED
    digitalWrite(LED_1_PIN, LOW); //Don't send voltage to the 2nd LED
    digitalWrite(LED_2_PIN, LOW); //Don't send voltage to the 3rd LED
    digitalWrite(LED_3_PIN, LOW); //Don't send voltage to the 4th LED
    
    break; //Exit block of code

  case TARGET_MODE: //Block of code for the TARGET_MODE display mode
     
     digitalWrite(LED_0_PIN, (targetLED + LED_0_PIN == LED_0_PIN ? HIGH:LOW)); //If targetLED plus the pin# of the 1st LED is equal to the pin# of the 1st LED, turn the 1st LED on. Otherwise, turn the 1st LED off.
     digitalWrite(LED_1_PIN, (targetLED + LED_0_PIN == LED_1_PIN ? HIGH:LOW)); //If targetLED plus the pin# of the 1st LED is equal to the pin# of the 2nd LED, turn the 2nd LED on. Otherwise, turn the 2nd LED off.
     digitalWrite(LED_2_PIN, (targetLED + LED_0_PIN == LED_2_PIN ? HIGH:LOW)); //If targetLED plus the pin# of the 1st LED is equal to the pin# of the 3rd LED, turn the 3rd LED on. Otherwise, turn the 3rd LED off.
     digitalWrite(LED_3_PIN, (targetLED + LED_0_PIN == LED_3_PIN ? HIGH:LOW)); //If targetLED plus the pin# of the 1st LED is equal to the pin# of the 4th LED, turn the 4th LED on. Otherwise, turn the 4th LED off.
    
    break; //Exit block of code

  case PATTERN_0_MODE: //Block of code for the 1st pattern display mode
     
      if(millis() - lastTime >= 50){ //If 50 milliseconds have elapsed since the last frame, do the following:
        
        digitalWrite(LED_0_PIN, bitRead(PATTERN_0, pointer)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame
        digitalWrite(LED_1_PIN, bitRead(PATTERN_0, pointer + 1)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (1) this frame
        digitalWrite(LED_2_PIN, bitRead(PATTERN_0, pointer + 2)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (2) this frame
        digitalWrite(LED_3_PIN, bitRead(PATTERN_0, pointer + 3)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (3) this frame
        
        pointer += 4; //Increment the pointer by four bits

        if(pointer/8 >= sizeof(PATTERN_0)) pointer = 0; //If the length of the pointer in bytes is at or exceeds the size of the current pattern (PATTERN_0) in bytes, reset the pointer to zero.

        lastTime = millis(); //Update lastTime to the current time in milliseconds
      }
      
      break; //Exit block of code

  case PATTERN_1_MODE: //Block of code for the 2nd display mode
  
    if(millis() - lastTime >= 50){ //If 50 milliseconds have elapsed since the last frame, do the following:
      
      digitalWrite(LED_0_PIN, bitRead(PATTERN_1, pointer)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame
      digitalWrite(LED_1_PIN, bitRead(PATTERN_1, pointer + 1)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (1) this frame
      digitalWrite(LED_2_PIN, bitRead(PATTERN_1, pointer + 2)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (2) this frame
      digitalWrite(LED_3_PIN, bitRead(PATTERN_1, pointer + 3)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (3) this frame
      
      pointer += 4; //Increment the pointer by four bits

      if(pointer/8 >= sizeof(PATTERN_1)) pointer = 0; //If the length of the pointer in bytes is at or exceeds the size of the current pattern (PATTERN_1) in bytes, reset the pointer to zero.

      lastTime = millis(); //Update lastTime to the current time in milliseconds
    }
    
    break; //Exit block of code

  case PATTERN_2_MODE: //Block of code for the 3rd display mode
    
    if(millis() - lastTime >= 100){ //If 100 milliseconds have elapsed since the last frame, do the following:v
    
      digitalWrite(LED_0_PIN, bitRead(PATTERN_2, pointer)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame
      digitalWrite(LED_1_PIN, bitRead(PATTERN_2, pointer + 1)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame
      digitalWrite(LED_2_PIN, bitRead(PATTERN_2, pointer + 2)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame
      digitalWrite(LED_3_PIN, bitRead(PATTERN_2, pointer + 3)); //Send voltage to the LED depending if there is a 1 (on) or a 0 (off) at the LED's position (0) this frame

      pointer += 4; //Increment the pointer by four bits

      if(pointer/8 >= sizeof(PATTERN_2)) pointer = 0; //If the length of the pointer in bytes is at or exceeds the size of the current pattern (PATTERN_2) in bytes, reset the pointer to zero.

      lastTime = millis(); //Update lastTime to the current time in milliseconds
    } 
  
    break; //Exit block of code
      
  }//End switch-case statement

}

/* --(end main loop )-- */


/* ( End ) */



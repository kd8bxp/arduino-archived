
/*
by Ray Scott for testing needs
for testing the Compact Rover and Bitty Rover
Compact Rover using Arduino Uno

Rover Serial Control

Uses the 2-wire mode for each motor on the L298 motor driver
 */

#include <Servo.h>

Servo servo9G;

const int RightWheelA = 4; //digital pin
const int RightWheelB = 5; //PWM pin
const int LeftWheelA = 6; //PWM pin
const int LeftWheelB = 7; //digital pin
// 4-wire mode will use PWM and /PWM

int PWMRight = 0;
int PWMLeft = 0;
int CharsInString = 0;
int var = 0;

char serialData[16];   // using up to 16 byte command structure.
                       // .. less is allowable. ..
                       //  end of command with special character 
                       
char CommandChar[ ] = "d" ; //  special character for serial stream terminator

void setup() {

  Serial.begin(9600);      // open the serial port at 9600 bps:   
  
  servo9G.attach(3); //PWM pin 3
   
   // Motor Driver Setup
   pinMode(RightWheelA, OUTPUT);
   pinMode(RightWheelB, OUTPUT);
   pinMode(LeftWheelA, OUTPUT);
   pinMode(LeftWheelB, OUTPUT);
      
// end of "void setup" 
}

void loop() {
//   GoForward();
 //  servo9G.write(1500); // ignore the servo during this test
//   delay(250); // turning time
//   GoStop();
//   delay(100);
   
//   PWMRight = 225; // full speed
//   PWMLeft  = 155; // half speed
//   GoPWMForward(PWMRight, PWMLeft);
//   delay(1000);
   
//   GoStop();
//   delay(100);

// get commands from master controller (via serial connection to PC or other)
//  if no serial data within XX time, then shut off all movement...

  if(Serial.available() > 0) 
  {
    Serial.readBytesUntil(CommandChar[0], serialData, 15); // wait for special character .. recommend '\0'
    // problem is that the bytes are processed after the serial timeout.
    // check here for the characters in the string.
    
    CharsInString = 0;
    while(serialData[CharsInString] != 0 ){ // searching for null character
       ++CharsInString;
    }
//         Serial.println (serialData[var]); //  ... this is always blank.. the null character to identify the end of string
//         Serial.println (serialData);
//         Serial.print("size:");
//         Serial.println (CharsInString);
    
    switch(serialData[0]) // check first character of the serial data string
    {
      case 's':
          // do commands for case 's'
          // check that the correct character is "CommandChar" .. else possible data corruption.
          if (CharsInString == 5) {
            Serial.println("command: s");
            //Serial.print('\0');
          }
        break;

      case 'r':
          // do commands for case 'r'
          Serial.println("command: r");
          //Serial.print('\0');
        break;
        
      default:
          Serial.println("ack"); 
      break;
    }
    // always echo
    Serial.println(serialData); // this writes the number of the char code to the serial port.
    Serial.println("");

    // clear serialData array
     memset(serialData, 0, sizeof(serialData));
  }

}


// now have special functions


void GoPWMForward(int PWML, int PWMR) {
   digitalWrite(LeftWheelB,LOW);
   digitalWrite(RightWheelA,LOW);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void GoPWMReverse(int PWML, int PWMR) {
  PWMR = 255-PWMR;
  PWML = 255-PWML;
  digitalWrite(LeftWheelB,HIGH);
   digitalWrite(RightWheelA,HIGH);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void PWMSpinLeft(int PWML, int PWMR){
  PWML = 255-PWML;
   digitalWrite(LeftWheelB,HIGH);
   digitalWrite(RightWheelA,LOW);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void PWMSpinRight(int PWML, int PWMR){
  PWMR = 255-PWMR;
   digitalWrite(LeftWheelB,LOW);
   digitalWrite(RightWheelA,HIGH);
   analogWrite(RightWheelB, PWMR);  // PWMR values go from 0 to 255
   analogWrite(LeftWheelA, PWML);  // PWML values go from 0 to 255
}

void SpinLeft(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,HIGH);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void GoLeft(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void SpinRight(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelA,HIGH);
  digitalWrite(RightWheelB,LOW);
}

void GoRight(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelA,LOW);
  digitalWrite(RightWheelB,LOW);
}


void GoReverse(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,HIGH);
  digitalWrite(RightWheelB,LOW);
  digitalWrite(RightWheelA,HIGH);


}
void GoForward(){
  digitalWrite(LeftWheelA,HIGH);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,HIGH);
  digitalWrite(RightWheelA,LOW);
}

void GoStop(){
  digitalWrite(LeftWheelA,LOW);
  digitalWrite(LeftWheelB,LOW);
  digitalWrite(RightWheelB,LOW);
  digitalWrite(RightWheelA,LOW);
}


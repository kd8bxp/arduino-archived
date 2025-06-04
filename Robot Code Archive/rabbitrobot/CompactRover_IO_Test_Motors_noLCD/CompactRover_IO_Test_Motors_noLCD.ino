
/* YourDuino.com Example Software Sketch
 16 character 2 line I2C Display
 Backpack Interface labelled "A0 A1 A2" at lower right.
 ..and
 I2C Backpack Interface 
 MOST use address 0x27, a FEW use 0x3F
..

Using Serial communications on Pins 0, and 1
Using Ultrasonic sensor on Pins 2,3
Using Motor control on Pins 4,5,6,7
pin 8 open
Using bumper switches on Pins 9 and 10
Using Servo control on Pin 11
pin 12 .. open
Using a Piezo buzzer on Pin 13

Using Photoresistors on Analog 0 and 1  ** These are polarity dependant, so don't wire them backward.
Analog 2 and 3 are open/available

Using I2C communications on AN4 and AN5
If not using the I2C LCD, then you need to remove all the respective code. 

  */

/*-----( Import needed libraries )-----*/

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/


#include <Servo.h>
Servo servo1;

/*-----( Declare Variables )-----*/
int ledPin = 13;   // LED connected to digital pin 13

int val = 0;       // variable to store the read value
int A0Pin = 0;
int A1Pin = 1;
int A2Pin = 2;
int A3Pin = 3;
int A4Pin = 4;  // reserved for future I2C
int A5Pin = 5;  // reserved for future I2C

int IO0Pin = 0;  // Do not use.. Reserved for Serial Communications
int IO1Pin = 1;  // Do not use.. Reserved for Serial Communications 
int IO2Pin = 2;  // using for Ultrasonic distance measurement
int IO3Pin = 3;  // using for Ultrasonic distance measurement

int IO4Pin = 4; // reserved for motor control
int IO5Pin = 5; // motor
int IO6Pin = 6; // motor
int IO7Pin = 7;  // motor

//The digital pins that the motor controller is attatched to on the UNO
const int L_PIN = 4; //Non-PWM Left wheel
const int L_PWM = 5; //PWM Left wheel (speed control)
const int R_PIN = 7; //Non-PWM Right wheel
const int R_PWM = 6; //PWM Right wheel (speed control)

int IO8Pin = 8;
int IO9Pin = 9;  // bumper right
int IO10Pin = 10;  // bumper left
int IO11Pin = 11;  // Servo
int IO12Pin = 12;  
int BuzzPin = 13;  // buzzer
int IO13Pin = 13;  // onboard LED

int TestMode = 1;
int ServoPosition = 90;

int MotorLeftSpeed = 200;
int MotorRightSpeed = 200;

String strTemp =  "";
String strTemp2 = "";
int TestBotMode = 0;


int trigPin=2; //Sensor Trigger pin connected to Arduino UNO pin 13
int echoPin=3;  //Sensor Echo pin connected to Arduino UNO pin 11
int myCounter=0;  //declare your variable myCounter and set to 0
float pingTime;  //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound=776.5; //Speed of sound in miles per hour when temp is 77 degrees.

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // Used to type in characters..Can use Serial Comm for this program

  pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
  
  pinMode(A0Pin, INPUT);   // use for photo-resistor 
  pinMode(A1Pin, INPUT);   // use for photo-resistor
  pinMode(A2Pin, INPUT);
  pinMode(A3Pin, INPUT);
//  pinMode(A4Pin, INPUT);   // Reserve AN4 for I2C
//  pinMode(A5Pin, INPUT);   // Reserve AN5 for I2C

//  pinMode(IO0Pin, INPUT);        // sets the digital pin 0 as input  .. Reserved for Serial Communications
//  pinMode(IO1Pin, OUTPUT);        // sets the digital pin 1 as output  .. Reserved for Serial Communications
  pinMode(IO2Pin, INPUT);        // sets the digital pin 2 as input
//  digitalWrite(IO2Pin, HIGH);       // turn on pullup resistors
  pinMode(IO3Pin, OUTPUT);        // sets the digital pin 3 as output 
  
  // motors at 4,5,6,7
  pinMode(IO4Pin, OUTPUT);        // sets the digital pin 4 as output
  pinMode(IO5Pin, OUTPUT);        // sets the digital pin 5 as output
  pinMode(IO6Pin, OUTPUT);        // sets the digital pin 6 as output
  pinMode(IO7Pin, OUTPUT);        // sets the digital pin 7 as input
  
//  pinMode(IO8Pin, INPUT);        // sets the digital pin 8 as input
//  digitalWrite(IO8Pin, HIGH);       // turn on pullup resistors  

  pinMode(IO9Pin, INPUT);        // sets the digital pin 9 as input
  digitalWrite(IO9Pin, HIGH);       // turn on pullup resistors
  pinMode(IO10Pin, INPUT);        // sets the digital pin 10 as input
  digitalWrite(IO10Pin, HIGH);       // turn on pullup resistors
//  pinMode(IO11Pin, OUTPUT);        // sets the digital pin 11 as output .. using for servo
  
//  pinMode(IO12Pin, INPUT);        // sets the digital pin 12 as input
//  digitalWrite(IO12Pin, HIGH);       // turn on pullup resistors
 
  pinMode(IO13Pin, OUTPUT);        // sets the digital pin 13 as output

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Setup port for servo
  pinMode(11, OUTPUT);
  servo1.attach(11);
  servo1.write(ServoPosition);

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_PIN, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(R_PWM, OUTPUT);  


// ------- Quick 3 beeps  -------------

  for(int i = 0; i< 4; i++)
  {
    tone(BuzzPin, 1000, 500);
//    digitalWrite(IO13Pin, HIGH); // only if you do not have the buzzer
    delay(250);
    digitalWrite(IO13Pin, LOW);
    delay(250);
  }

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{

  if (TestBotMode == 0) {
// test the motor control and movements
     if (digitalRead(IO9Pin)==LOW) {
      // roll wheel backward to pull away from the switch
    digitalWrite(L_PIN, HIGH); //Set both the left motor to go reverse
    analogWrite(L_PWM, (255-MotorLeftSpeed));
      }
      else {
      // turn motor off
    digitalWrite(L_PIN, LOW); //Set both the left motor to stop
    analogWrite(L_PWM, 0);
      }

  if (digitalRead(IO10Pin)==LOW) {
      // roll wheel backward to pull away from the switch
    digitalWrite(R_PIN, HIGH); //Set both the Right motor to go reverse
    analogWrite(R_PWM, (255-MotorRightSpeed));
      }
      else {
      // turn motor off
    digitalWrite(R_PIN, LOW); //Set both the left motor to stop
    analogWrite(R_PWM, 0);
      }
  }

  if (TestBotMode == 1) {
// test the Ultra-sonic distance device

  digitalWrite(trigPin, LOW); //Set trigger pin low
  delayMicroseconds(2000); //Let signal settle
  digitalWrite(trigPin, HIGH); //Set trigPin high
  delayMicroseconds(15); //Delay in high state
  digitalWrite(trigPin, LOW); //ping has now been sent
  delayMicroseconds(10); //Delay in high state
  
  pingTime = pulseIn(echoPin, HIGH);  //pingTime is presented in microceconds
  pingTime=pingTime/1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
  pingTime=pingTime/3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
  targetDistance= speedOfSound * pingTime;  //This will be in miles, since speed of sound was miles per hour
  targetDistance=targetDistance/2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
  targetDistance= targetDistance*63360;    //Convert miles to inches by multipling by 63360 (inches per mile)
  
  Serial.print("Distance: ");
  Serial.print(targetDistance);
  Serial.println(" inches");
  delay(250); //pause to let things settle
  }


  if (TestBotMode == 2) {
// test Servo
 
 if ((digitalRead(IO9Pin)==LOW)&(digitalRead(IO10Pin)!=LOW)) {
      ServoPosition = ServoPosition + 5;
      if (ServoPosition > 180){ ServoPosition = 180; }
      servo1.write(ServoPosition);
  }


  if ((digitalRead(IO10Pin)==LOW)&(digitalRead(IO9Pin)!=LOW)) {
      ServoPosition = ServoPosition - 5;
      if (ServoPosition < 5){ ServoPosition = 5; }
      servo1.write(ServoPosition);
  }
 
  }

  
 if (TestBotMode == 3) {   
  // test the photo resistors.. 
  
   if (analogRead(A0Pin) >=25) {
      // read analog value from the Photoresistor AN0
      val=analogRead(A0Pin);
      Serial.print("Analog 0: ");
      Serial.println(val);
      delay(250);
      }  

  if (analogRead(A1Pin) >=25) {
      // Read analog value from the Photoresistor AN2
      val=analogRead(A1Pin);
      Serial.print("Analog 1: ");
      Serial.println(val);
      delay(250);
      }  
 }


 if (TestBotMode == 4) {   
// testing the bumper switches

  if ((digitalRead(IO9Pin)==LOW)&(digitalRead(IO10Pin)!=LOW)) {
      tone(BuzzPin, 1400, 300);
  }
  if ((digitalRead(IO10Pin)==LOW)&(digitalRead(IO9Pin)!=LOW))  {
      tone(BuzzPin, 600, 300);
  }

  if ((digitalRead(IO10Pin)==LOW)&&(digitalRead(IO9Pin)==LOW)) {
      // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      tone(BuzzPin, 1000, 500);
  }  
 }
    
    // when characters arrive over the serial port...
    if (Serial.available()) {
      // wait a bit for the entire message to arrive
      delay(100);
      // clear the screen
      strTemp = "";
      strTemp2 = "";      
      // read all the available characters
      while (Serial.available() > 0) {
        // display each character to the LCD
        strTemp = String(char(Serial.read()));
        strTemp2 =  String(strTemp2 + strTemp); // concatenating two strings
        //Serial.println(strTemp2);
        
        
        if (strTemp2 == "TestBotMode2") {
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);
          TestBotMode = 2;
          delay(1000);
        }

                // Serial.println(strTemp2.substring(0,12));
        if (strTemp2.substring(0,12) == "TestBotMode0") {
          tone(BuzzPin, 400, 800);
          TestBotMode = 0;
          delay(1000);
        }

        // Serial.println(strTemp2.substring(0,12));
        if (strTemp2.substring(0,12) == "TestBotMode1") {
          tone(BuzzPin, 1000, 200);
          TestBotMode = 1;
          delay(1000);
        }

                // Serial.println(strTemp2.substring(0,12));
        if (strTemp2.substring(0,12) == "TestBotMode3") {
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);
         TestBotMode = 3;
          delay(1000);
        }

                        // Serial.println(strTemp2.substring(0,12));
        if (strTemp2.substring(0,12) == "TestBotMode4") {
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);
          delay (200);
          tone(BuzzPin, 1000, 200);         
          TestBotMode = 4;
          delay(1000);
        }
        
      }
  
     
  
  }
  
delay (200);

}/* --(end main loop )-- */


/* ( THE END ) */


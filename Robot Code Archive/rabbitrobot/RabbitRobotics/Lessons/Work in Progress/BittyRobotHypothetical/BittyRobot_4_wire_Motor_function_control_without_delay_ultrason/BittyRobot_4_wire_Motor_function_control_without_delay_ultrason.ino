/******************************************
 * 
 * This program contains a base set of sub routines for motor control.
 * the program it's self does nothing, and is used to show how to create sub-routines
 * This is using a standard 4 wire set, with 2 wires being digital (HIGH/LOW) and 2 
 * PWM pins for speed control.
 * 
 * Aug 2, 2017 - Moved motor control to sub routines. LeRoy Miller, 2017 RabbitRobots.com
 * Routines are:
 * left(speed, duration)
 * right(speed, duration)
 * forward(speed, duration)
 * backwards(speed, duration)
 * stop()
 * 
 * These are your 5 basic functions: What other functions could be added?
 * 
 ******************************************/

/*  Using just 4 wires, you can control 2 motors with speed and direction.
 *  Each motor needs a digital pin (HIGH/LOW) and a PWM pin.
 *  for direction control, inverted logic is used. See examples below.
 *  PWM for Speed Control and direction.  
 */

 /* Aug 8, 2017 - LeRoy Miller 2017, RabbitRobots.com This example improves on the previous motor functions by elimanating the
  *  delay.  A new function is added.
  *  update() - which is used to check if the delay has expired and turn the motors off
  *  
  *  Also a few new variables are added:
  *  
  *  isRunning - which can be used to check to see if the motors should be turning. (not 
  *  that they are turning)
  *  
  *  previousMillis - should be a unsigned long since time variables can get very large very quickly
  *  this stores the last millisec the motor drive functions were called, and is used
  *  as a check against the current millisec and the onTime for the motor.
  *  
  *  onTime - this takes the place of the delay time in the previous sketch. It is checked
  *  against the currentMillis and the previousMillis to see if that change is greater.
  *  
  *  CurrentMillis - holding variable for the current millisec at the time the update() function
  *  is called. 
  *  
  */
  
 /* Aug 8, 2017 - LeRoy Miller 2017 - RabbitRobots.com
  *  In this example we are going to add the ultrasonic, and try to have the robot avoid hitting
  *  objects. We are building on running without delays and are going to make a 
  *  new function called ping()
  *  The ping() function will return a distance and let the robot figure out how to handle the
  *  motors. (Stop, turn, backup, etc)
  *  We will base this new function off our previous ultrasonic code.
  *  
  *  We will also add a function to check distance while the robot isRunning. We will call this
  *  function checkDistance()
  */

 #define US_TRIGGER_PIN 8 //pins used on the UNO
 #define US_RECEIVE_PIN 9 //pins used on the UNO

float pingTime;  //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound=776.5; //Speed of sound in miles per hour when temp is 77 degrees.
unsigned long echoDuration = 0; //Used to store the duration of the echo in microseconds 
const int DURATION_PER_INCH = 550; //The approximate duration in microseconds of the echo for an object one inch away.
int pingavg = 0; //avgerage ping distances
int pingcount = 0; //ping counter
  
//The digital pins that the motor controller is attatched to on the UNO
const int L_PIN = 4; //Non-PWM Left wheel
const int L_PWM = 5; //PWM Left wheel (speed control)
const int R_PIN = 7; //Non-PWM Right wheel
const int R_PWM = 6; //PWM Right wheel (speed control)

//Generally, you should use "unsigned long" for variables that hold time
//The value will quickly become too large for an int to store
unsigned long previousMillis = 0; //store the last time the motor were updated.
unsigned long currentMillis; //used to store a snap shot of the current millisec at the time the update function is called.
int isRunning; //set flag if motors are running.
int onTime; //this is used as a check against time

int setSpeed = 200; //set the speed you want the robot to move.

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_PIN, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(R_PWM, OUTPUT);  
   //Set the pin connected to the trigger of the ultrasound sensor to be output to send the pulse and set the receiver to input to read the echo coming back
  pinMode(US_TRIGGER_PIN, OUTPUT);
  pinMode(US_RECEIVE_PIN, INPUT);
}

void loop() { 
 
  forward(setSpeed, 250); 
 
  while (isRunning) {
    ping(); //Notice we add the check ping() function inside the isRunning loop
    update(); //update time for robot motors
    checkDistance(); //we also check our distances often.
  }
  
  }

void checkDistance() {
//If we are too close to an object, we will turn left (fast) and then move forward again.
int ping1 = (pingavg/pingcount);

  if (ping1 <= 10) {
    stop();
    pingavg = 0;
    pingcount = 0;
    left(255,250);
    while (isRunning) {
     update();
        }
    stop();
  }
}
void ping() {
  pingcount ++; //increase counter by one
  digitalWrite(US_TRIGGER_PIN, LOW);
  delayMicroseconds(2000);
  digitalWrite(US_TRIGGER_PIN, HIGH); //Send voltage to the trigger pin to begin the ultrasound pusle
  delayMicroseconds(15); //Pause the program for 10 microseconds (to produce a 10 microsecond pulse)
  digitalWrite(US_TRIGGER_PIN, LOW); //Stop sendig voltage to the trigger pin, ending the pulse
  delayMicroseconds(10);
  echoDuration = pulseIn(US_RECEIVE_PIN, HIGH); //Get the duration of the echo coming back in microseconds 
  pingTime = echoDuration;
  pingTime=pingTime/1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
  pingTime=pingTime/3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
  targetDistance= speedOfSound * pingTime;  //This will be in miles, since speed of sound was miles per hour
  targetDistance=targetDistance/2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
  targetDistance= targetDistance*63360;    //Convert miles to inches by multipling by 63360 (inches per mile)
if (targetDistance >= 100 || targetDistance <= 0) {
  pingavg = 0;
  pingcount = 0;
} else {
  pingavg = pingavg + targetDistance;
}
 
}

void left(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(R_PIN, LOW); 
    analogWrite(R_PWM, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void right(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_PIN, LOW); 
    analogWrite(L_PWM, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void forward(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_PIN, LOW); //Set both the left and right motors to go forwards
    analogWrite(L_PWM, speed);
    digitalWrite(R_PIN, LOW);
    analogWrite(R_PWM, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void backwards(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_PIN, HIGH); //Set both the left and right motors to go in reverse
    analogWrite(L_PWM, 255 - speed);
    digitalWrite(R_PIN, HIGH);
    analogWrite(R_PWM, 255 - speed);       
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void stop() {
    previousMillis = millis(); //set timings to millisecs the function was called
    digitalWrite(L_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_PWM, LOW);
    digitalWrite(R_PIN, LOW);
    digitalWrite(R_PWM, LOW);
    isRunning = 0; //motors should not be running flag.
        }

void update() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= onTime) {
    previousMillis = currentMillis;
    stop();
  }
}
/* ( End ) */

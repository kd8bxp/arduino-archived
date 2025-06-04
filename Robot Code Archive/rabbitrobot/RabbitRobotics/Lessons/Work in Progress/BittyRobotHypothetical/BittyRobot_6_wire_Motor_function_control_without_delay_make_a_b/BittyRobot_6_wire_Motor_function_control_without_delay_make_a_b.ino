/******************************************
 * 
 * This program contains a base set of sub routines for motor control.
 * the program it's self does nothing, and is used to show how to create sub-routines
 * This is using a standard 6 wire set, with 4 wires being digital (HIGH/LOW) and 2 enable
 * pins with speed control using PWM.
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

/* Enable Pins are using PWM for Speed Control, they also "enable" the motor to move, 
 * The other pins are used for direction control. 
 * Using this method, it takes 6 wires to control two motor with speed and direction control
 */

 /* Aug 8, 2017 - This example improves on the previous motor functions by elimanating the
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
  
const int L_ENABLE_PIN = 7; //The digital pins that the motor controller is attatched to one the MEGA
const int L_FORWARDS_PIN = 6;
const int L_REVERSE_PIN = 5;
const int R_FORWARDS_PIN = 4;
const int R_REVERSE_PIN = 3;
const int R_ENABLE_PIN = 2;

//Generally, you should use "unsigned long" for variables that hold time
//The value will quickly become too large for an int to store
unsigned long previousMillis = 0; //store the last time the motor were updated.
unsigned long currentMillis; //used to store a snap shot of the current millisec at the time the update function is called.
int isRunning; //set flag if motors are running.
int onTime; //this is used as a check against time

void setup() { /*----( SETUP: RUNS ONCE )----*/

  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_ENABLE_PIN, OUTPUT);
  pinMode(L_FORWARDS_PIN, OUTPUT);
  pinMode(L_REVERSE_PIN, OUTPUT);
  pinMode(R_FORWARDS_PIN, OUTPUT);
  pinMode(R_REVERSE_PIN, OUTPUT);
  pinMode(R_ENABLE_PIN, OUTPUT);
  
}

void loop() { 
  /* We are going to attempt to make a box with the robot. 
   *  Here you see a new subroutine called box(). We did this to show how subroutines can
   *  be used, we could have just as easily make a box in the main loop.
   *  
   * Do you see a problem with what we have done?  
   *
   */
  box();
  while(1) {} //infinate loop and stop sketch
}

void box() {

  /* Answer: Depending on how various factors, your motors may spin at different rates
   *  even if you tell them to spin at the same speed.
   *  different surfaces can also cause your motors to slip.
   *  One possiable way to correct for this is to use encoders on the wheels.
   *  Encoders count the number of rotations and can correct (speed up or slow down) a wheel
   *  
   *  Unfortunatly the compact rover doesn't have room for wheel encoders.
   *  So what other possiable ways can we get the robot to make a box?
   *  
   */
   
  forward(60,1000);
  while (isRunning) {
    update();  
  }
  for (int i=1; i<4; i++) {
  right(60, 1000);
  while (isRunning) {
    update();
  }
  forward(60, 1000);
  while (isRunning) {
    update();
  }
  } 
}

void forward(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set the left motor to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, LOW); //Set the right motor to go in reverse
    digitalWrite(R_REVERSE_PIN, HIGH);
    analogWrite(L_ENABLE_PIN, speed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running

}

void backwards(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_FORWARDS_PIN, LOW); //Set the left motor to in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    digitalWrite(R_FORWARDS_PIN, HIGH); //Set the right motor to go forwards
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, speed); //Make both motors run at spinning driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void left(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_FORWARDS_PIN, HIGH); //Set both the left and right motors to go forwards
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, HIGH);
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, speed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void right(int speed, int duration) {
    previousMillis = millis();
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, HIGH);
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, HIGH);
    analogWrite(L_ENABLE_PIN, speed); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, speed);
    onTime = duration; //notice the replacement of delay with the variable onTime.
    isRunning = 1; //motors should be running
}

void stop() {
   previousMillis = millis(); //set timings to millisecs the function was called
    digitalWrite(L_FORWARDS_PIN, LOW); //Set both the left and right motors to go in reverse
    digitalWrite(L_REVERSE_PIN, LOW);
    digitalWrite(R_FORWARDS_PIN, LOW);
    digitalWrite(R_REVERSE_PIN, LOW);
    analogWrite(L_ENABLE_PIN, 0); //Set both the left and right motors to run at driving driveSpeed
    analogWrite(R_ENABLE_PIN, 0);
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

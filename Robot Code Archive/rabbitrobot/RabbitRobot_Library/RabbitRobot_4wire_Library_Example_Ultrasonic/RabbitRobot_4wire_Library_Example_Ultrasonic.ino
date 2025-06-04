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

/* Aug 9, 2017 - LeRoy Miller RabbitRobots.com 2017 - 
 *  This example shows how to create your own custom library, please see the included RabbitRobot.h
 *  and RabbitRobot.cpp files that are included.
 *  Built from the functions/subroutines from the ultrasonic sketch. See notes for how the 
 *  sketch has changed.
 *  
 *  Good Creating your Own library Tutorials:
 *  https://playground.arduino.cc/Code/Library
 *  https://www.arduino.cc/en/Hacking/LibraryTutorial
 */

#include "RabbitRobot.h" //this is our custom library

/* The library has two classes that need to be invoked. RabbitRobot which is the motor drive
 *  class. And UltraSonic which is used for the UltraSonic.
 */

RabbitRobot bot(4,5,7,6); //Notice these are the same pin numbers we used before.

/* We are telling the library which pins our motor are hooked up too. It follows the same
 *  Pattern as before. Left NonPWM, Left PWM, Right NonPWM, Right PWM pin
 *  //The digital pins that the motor controller is attatched to on the UNO
 *  const int L_PIN = 4; //Non-PWM Left wheel
 *  const int L_PWM = 5; //PWM Left wheel (speed control)
 *  const int R_PIN = 7; //Non-PWM Right wheel
 *  const int R_PWM = 6; //PWM Right wheel (speed control)
 */

//Next we need to invoke a instance for the UltraSonic class

UltraSonic ping(8, 9); //Notice how we tell the library which pins to use 
/* This takes the place of  
 #define US_TRIGGER_PIN 8 //pins used on the UNO
 #define US_RECEIVE_PIN 9 //pins used on the UNO
*/

// The Library handles the rest of the variables we need to use.
/* 
float pingTime;  //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound=776.5; //Speed of sound in miles per hour when temp is 77 degrees.
unsigned long echoDuration = 0; //Used to store the duration of the echo in microseconds 
const int DURATION_PER_INCH = 550; //The approximate duration in microseconds of the echo for an object one inch away.
int pingavg = 0; //avgerage ping distances
int pingcount = 0; //ping counter
  

//Generally, you should use "unsigned long" for variables that hold time
//The value will quickly become too large for an int to store
unsigned long previousMillis = 0; //store the last time the motor were updated.
unsigned long currentMillis; //used to store a snap shot of the current millisec at the time the update function is called.
int isRunning; //set flag if motors are running.
int onTime; //this is used as a check against time
*/

int setSpeed = 200; //set the speed you want the robot to move.

void setup() { /*----( SETUP: RUNS ONCE )----*/

bot.begin();
ping.begin();

//The two statements above take the place of setting up pins and initilizing variables.

/*
  //Set all of the pins connected to the motor controller to run in output mode (Data needs to be sent to the controller, not receiveed from it)
  pinMode(L_PIN, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(R_PWM, OUTPUT);  
   //Set the pin connected to the trigger of the ultrasound sensor to be output to send the pulse and set the receiver to input to read the echo coming back
  pinMode(US_TRIGGER_PIN, OUTPUT);
  pinMode(US_RECEIVE_PIN, INPUT);
  */
}

void loop() { 
 
  bot.forward(setSpeed, 250); //notice we added the name of our RabbitRobot instance "bot" 
 //We need to add the Robot instance to all of our previous functions.
 //Note: That isRunning was also changed it IsRunning() 
 ping.ping(); //we added another check of the ultrasonic
  while (bot.IsRunning()) {
    //We add the ping instance to our ping function.
    ping.ping(); //Notice we add the check ping() function inside the isRunning loop
    bot.update(); //update time for robot motors
    checkDistance(); //we also check our distances often. This subroutine is not in the library
    //We don't use the Ultrasonic instance name here
  }
  
  }

void checkDistance() {

  ping.ping(); //we added another check of the ultrasonic
//If we are too close to an object, we will turn left (fast) and then move forward again.
/* This is really the biggest change to our sketch, the line
 *  int ping1 = (pingavg/pingcount);  will be replaced with 
 *  the UltraSonic functions avgCount() and avgDistance()
 */
 int avgDistance = ping.avgDistance();
 int avgCount = ping.avgCount();
int ping1 = (avgDistance/avgCount);  

//We need to add the Robot instance to all of our previous functions.
  if (ping1 <= 10) {
    bot.stop();
    ping.avgClear();
    /* These two lines are replaced with our Ultrasonic instance and avgClear()
    pingavg = 0;
    pingcount = 0;
    */
    bot.left(255,250);
    //Note: That isRunning was also changed it IsRunning()
    while (bot.IsRunning()) {
     
     bot.update();
             }
        
    bot.stop();
  }
}

// The subroutines and functions from this point on were moved to the library and are
// in the RabbitRobot.cpp file

/*
 * 
 * BittyBot Compact Rover Ultrasonic with timedAction Demo
 * By LeRoy Miller, Mar 19, 2017
 * 
 * Required Libraries: 
 * NewPing, CompactRover, and TimedAction
 * 
 *  Obstacle Avoidance using Ultrasonic Sensor.
 *  
 *  Limitations: * if the ultrasonic approches a obstacle at a
 *  angle it may hit that obstacle. This is a limitation of how
 *  the sensor works.
 *    
 *  License:  
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
 */

 
#include <NewPing.h>
#include "CompactRover.h"
#include <TimedAction.h>

void checkping(); //required for TimedAction
TimedAction timedAction(15, checkping);

//Left, LeftPWM, Right, RightPWM
CompactRover bot(7,10,8,11);

//Direction to include the time you want the motors on and a speed
//in PWM.

int maxspeed = 150; //Max Speed the robot should move at
int minspeed = 65; //Minimum Speed the robot will still operate at

#define TRIGGER_PIN 9
#define ECHO_PIN 12
#define MAX_DISTANCE 200
int pingavg = 0;
int pingcount = 0;
unsigned int uS;
unsigned int sI;
  
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
 Serial.begin(9600); //for debug, not needed for robot
 bot.begin();
}

void loop() {
  
bot.forward(5000, maxspeed);

while(bot.IsRunning()) {
  checksonar(); //check distance conditions
  timedAction.check(); //update the ultrasonic distance
  Serial.println(pingavg/pingcount);
  bot.update(); //update the robot motors
}
bot.stop(); //Stop the robots motors
}

void checksonar() {

 timedAction.check();
 int ping1 = (pingavg/pingcount);

  if (ping1 > 30 && ping1 <=39) {
      bot.stop();
      int tempspeed = maxspeed / 2; //slow the robot down
      if (tempspeed < minspeed) {tempspeed = minspeed;} //if new speed is too slow set to minimum speed
       bot.forward(500, tempspeed);
       while(bot.IsRunning()) {
    bot.update();
    timedAction.check();
}
  }
  if (ping1 > 11 && ping1 <=29) {
    bot.stop();
    int tempspeed = maxspeed / 3; //slow robot down some more
    if (tempspeed < minspeed) {tempspeed = minspeed;} //if new speed is to slow set to minimum speed
    bot.forward(500, tempspeed);
    while(bot.IsRunning()) {
    bot.update();
    timedAction.check();
}
  }
  if (ping1 <= 10 && ping1 >= 0) {
    bot.stop();
    pingavg = 0; //reset the avgerage from ultrasonic
    pingcount = 0; //reset the counter
    int tempspeed = minspeed * 2; //speed the motors up to turn
    if (tempspeed > maxspeed) {tempspeed = maxspeed;} //if new speed is too fast set to maxspeed
    bot.leftTight(450, tempspeed); //turn the robot to avoid obstacle (hopefully)
    while (bot.IsRunning()) {
    bot.update();
    timedAction.check(); 
    }
    bot.stop();
  }  
}

void checkping() {
  pingcount ++; //start a counter
   uS = sonar.ping(); //get a timing from sensor
   sI = (uS/ US_ROUNDTRIP_IN); //convert to INCHES

//Check distances and update the avgerage and counter if needed.

   if (sI >= MAX_DISTANCE || sI <= 0) {
    pingavg = 0;
    pingcount = 0;
   } else {
    pingavg = pingavg + (sI);
   }
    
}



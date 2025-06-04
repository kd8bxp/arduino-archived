#include <NewPing.h>
#include "CompactRover.h"

//Left, LeftPWM, Right, RightPWM
CompactRover bot(7,10,8,11);

//Direction to include the time you want the motors on and a speed
//in PWM.

int maxspeed = 200;

#define TRIGGER_PIN 9
#define ECHO_PIN 12
#define MAX_DISTANCE 400
int pingavg = 0;
int pingcount = 0;
int errcnt = 0;
unsigned int uS;
unsigned int sI;
  
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 bot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
bot.forward(5000, maxspeed);

while(bot.IsRunning()) {
  checksonar();
  Serial.println(sI);
  bot.update();
}
}

void checksonar() {

 pingavg1();
 
  if (sI > 20 && sI <=29) {
      bot.stop();
       bot.forward(250, maxspeed / 2);
       while(bot.IsRunning()) {
    bot.update();
}
  }
  if (sI > 6 && sI <=19) {
    bot.stop();
    bot.forward(250, maxspeed / 3);
    while(bot.IsRunning()) {
    bot.update();
}
  }
  if (sI <= 5) {
    bot.stop();
    bot.leftTight(450, maxspeed / 2);
      while (bot.IsRunning()) {
      //Serial.println("Bot Running");
      bot.update();
      
    }
    bot.stop();
    
  }
}

void pingavg1() {
  while(pingcount<10) {
   uS = sonar.ping();
   sI = (uS/ US_ROUNDTRIP_IN);
   if (sI <= 0) {
    pingcount --;
    errcnt ++;
    if (errcnt >= 5) {
      bot.stop();
      Serial.println("Error Count High!");
      /*
       bot.leftTight(650, maxspeed);
       while (bot.IsRunning()) {
      //Serial.println("Bot Running");
      bot.update();
      }
      */
      errcnt = 0;
      bot.stop();
      while(1);      
        }
    } else {
  pingcount ++;
  pingavg = pingavg + sI;
    }
  }
  sI = pingavg/pingcount;
  pingcount = 0;
  pingavg = 0;
}



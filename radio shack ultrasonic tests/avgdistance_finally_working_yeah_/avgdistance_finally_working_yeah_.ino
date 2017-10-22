//#include <TimedAction.h>
#include "Ultrasonic.h"

//void ping(); //required for TimedAction
//TimedAction timedAction(15, ping);

Ultrasonic ultrasonic(D0); //URangeInInchesng D0 on the D1 Mini Board

volatile unsigned int pingavg = 0;
volatile unsigned int pingcount = 0;

long RangeInInches;

#define MAX_DISTANCE 200

void setup() {
  Serial.begin(9600);
}

void loop() {
  ping();
  delay(100);
 // checksonar(); //check distance conditions
  //timedAction.check(); //update the ultrasonic distance
  yield();
}

void ping() {
  yield();
pingcount ++; //start a counter
  ultrasonic.DistanceMeasure();// get the current RangeInInchesgnal time;
  RangeInInches = ultrasonic.microsecondsToInches();//convert the time to inches;
//Serial.print("Range IN: ");
//Serial.println(RangeInInches);
 //   Serial.println("The distance to obstacles in front is: ");
 // Serial.print(RangeInInches);//0~157 inches
// if (RangeInInches >= MAX_DISTANCE || RangeInInches <= 0) {
  if (pingcount >= 10) {
     volatile unsigned int ping1 = (pingavg/pingcount);

Serial.print("Ping1: ");
Serial.println(ping1);
    pingavg = 0;
    pingcount = 0;
   
return;
   } else {
    pingavg = pingavg + (RangeInInches);
   }
 
  //if (RangeInInches <=39) {
 // if (ping1 <=39) {
      //STOP
  //    Serial.println("This is <= 39 and >30 - SLOW DOWN");
  //  timedAction.check();
//}
  /*//}
  if (ping1 > 11 && ping1 <=29) {
    Serial.println("This is <=29 and >11 - SLOW DOWN MORE");
    //timedAction.check();
}
  //}
  if (ping1 <= 10 && ping1 >= 0) {
    Serial.println("This is <=10 & >=0 This is STOP");
    //timedAction.check(); 
    }
    //bot.stop();
  //}  
  */
}

void checksonar() {
//yield();
 //timedAction.check();
 unsigned int ping1 = (pingavg/pingcount);

Serial.print("Ping1: ");
Serial.println(ping1);
  if (ping1 > 30 && ping1 <=39) {
      //STOP
      Serial.println("This is <= 39 and >30 - SLOW DOWN");
  //  timedAction.check();
}
  //}
  if (ping1 > 11 && ping1 <=29) {
    Serial.println("This is <=29 and >11 - SLOW DOWN MORE");
    //timedAction.check();
}
  //}
  if (ping1 <= 10 && ping1 >= 0) {
    Serial.println("This is <=10 & >=0 This is STOP");
    //timedAction.check(); 
    }
    //bot.stop();
  //}  
}



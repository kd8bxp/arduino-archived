//#include <TimedAction.h>
#include "Ultrasonic.h"

//void ping(); //required for TimedAction
//TimedAction timedAction(15, ping);

Ultrasonic ultrasonic(D0); //URangeInInchesng D0 on the D1 Mini Board

int pingavg = 0;
int pingcount = 0;
int ping1;
long RangeInInches;

#define MAX_DISTANCE 200

void setup() {
  Serial.begin(9600);
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
}

void loop() {
  ESP.wdtFeed();
  ping();
  //delay(250);
 // checksonar(); //check distance conditions
  //timedAction.check(); //update the ultrasonic distance
  //yield();
}

void ping() {
  //yield();
   // ESP.wdtFeed();

pingcount ++; //start a counter
  ultrasonic.DistanceMeasure();// get the current RangeInInchesgnal time;
  RangeInInches = ultrasonic.microsecondsToInches();//convert the time to inches;

 //   Serial.println("The distance to obstacles in front is: ");
 // Serial.print(RangeInInches);//0~157 inches
// if (RangeInInches >= MAX_DISTANCE || RangeInInches <= 0) {
  if (pingcount >= 100) {
     // ESP.wdtFeed();
ping1 = (pingavg/pingcount);

Serial.print("Ping1: ");
Serial.println(ping1);
    //Serial.print("Range IN: ");
//Serial.println(RangeInInches);
    pingavg = 0;
    pingcount = 0;
    return;
   } else {
    pingavg += RangeInInches;
   }
 
 // ESP.wdtFeed();

  //if (RangeInInches <=39) {
  //if (ping1 <=39) {
      //STOP
    //  Serial.println("This is <= 39 and >30 - SLOW DOWN");
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



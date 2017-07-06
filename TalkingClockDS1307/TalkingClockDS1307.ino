
#include <SoftwareSerial.h> // MOVI needs SoftwareSerial
#include <Wire.h>
#include "MOVIShield.h"     // Include MOVI library.    Download at http://www.audeme.com/downloads.html
#include "RTClib.h"         // Real Time Clock Library. Download at https://github.com/adafruit/RTClib
//
// This example implements a talking clock. 
// Connect the DS1307 to the Arduino/MOVI headers in the following way:
//        DS1307       Uno R3
//          GND   ->    GND
//          VCC   ->    5V
//          SDA   ->    SDA
//          SCL   ->    SCL

MOVI recognizer(true);    // Get a MOVI object, true enables serial monitor interface
RTC_DS1307 rtc;           // Real Time Clock using the DS1307 Chip


char monthsOfTheYear[12][12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
int   Timer = 0; 
long  TimerEndTime;     // Timer end time when set
long  CountdownEndTime=0;
long  Countdown = -1;

void setup()  
{
  recognizer.init();      // Initialize MOVI (waits for it to boot)
  // recognizer.setSynthesizer(SYNTH_PICO);  // Comment out if you have the MOVI Kernel 1.1
  recognizer.callSign("Arduino"); // Train callsign Arduino (may take 20 seconds)
  recognizer.addSentence(F("What time is it ?"));        // Add sentence 1
  recognizer.addSentence(F("What is the time ?"));        // Add sentence 2
  recognizer.addSentence(F("What is the date ?"));       // Add sentence 3
  recognizer.addSentence(F("What is today ?"));          // Add sentence 4
  recognizer.addSentence(F("Cancel timer"));             // Add sentence 5
  recognizer.addSentence(F("Is timer set ?"));           // Add sentence 6
  recognizer.addSentence(F("How much time remaining ?"));     // Add sentence 7
  recognizer.addSentence(F("Set Timer for 1 minute"));    // Add sentence 8
  recognizer.addSentence(F("Set Timer for 2 minutes"));   // Add sentence 9
  recognizer.addSentence(F("Set Timer for 3 minutes"));   // Add sentence 10
  recognizer.addSentence(F("Set Timer for 4 minutes"));   // Add sentence 11
  recognizer.addSentence(F("Set Timer for 5 minutes"));   // Add sentence 12
  recognizer.addSentence(F("Countdown"));                 // Add sentence 13

  recognizer.train();                          // Train (may take 20seconds) 

  recognizer.setThreshold(20);			// uncomment and set to a higher value (valid range 2-95) if you have a problems due to a noisy environment.

  if (! rtc.begin()) {
    recognizer.say("Error, could not find real time clock module");
    while (1);
  }
  // Comment out the first time to set the time and date on the RTC module. After this,
  // it will remember it via it non-volatile memory. 
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  recognizer.say("Real Time Clock Starting");

}


void loop() // run over and over
{
  DateTime     now = rtc.now();         // Get real time
  
  signed int   res = recognizer.poll(); // Get result from MOVI, 0 denotes nothing happened, negative values denote events (see docs)

  // Tell current time
  if (res==1 | res==2) {                     // Sentence 1 & 2
    if ( now.hour() > 12) 
      recognizer.say("It's " + String(now.hour()-12) + " " + ( now.minute() < 10 ? "O" : "" ) + String(now.minute()) + "P M" ); // Speak the time
    else
      recognizer.say("It's " + String(now.hour())    + " " + ( now.minute() < 10 ? "O" : "" ) + String(now.minute()) + "A M" ); // Speak the time
  } 

  // Tell current date
  if (res==3 || res==4) {                    // Sentence 3 & 4
   recognizer.say("Today is" + String(monthsOfTheYear[now.month()-1]) + String(now.day()) + " " + String(now.year()));
  }

  // Cancel Timer
  if (res==5) {                    // Sentence 6
    Timer = 0; 
    recognizer.say("Timer was cancelled");
  }

  // Check Timer Status
  if (res==6 || res==7 ) { 
     if (Timer) {
      if (res==6) recognizer.say("Yes."); 
      long left = TimerEndTime - now.secondstime();
      if ( left < 60 ) 
        recognizer.say("There are " + String(left) + " seconds left");
      else
        recognizer.say("There is " + String((int) (left/60)) + "minutes and " + String(left%60) + "seconds left");
     }
     else
      recognizer.say("Timer is not set");
  }

  // Set timer
  if (res>=8 && res <=12) {
    Timer = 1; 
    TimerEndTime = now.secondstime() + (res-7)*60 ;
    recognizer.say("Timer set for " + String(res-7) + " minutes");    
  }

  // 10 second countdown
  if (res==13) {
    CountdownEndTime = now.secondstime() + 11; 
    Countdown = 11;
  }

  if ( Timer  && ( now.secondstime() >= TimerEndTime) ) {
    Timer = 0; 
    recognizer.say("Time is up!");
  }

  if (Countdown >= 0 && ( CountdownEndTime-now.secondstime() != Countdown )) {
    Countdown = CountdownEndTime-now.secondstime() ;
    Serial.println(String(Countdown));
    recognizer.say(String(Countdown)
    
    
    );
  }
}



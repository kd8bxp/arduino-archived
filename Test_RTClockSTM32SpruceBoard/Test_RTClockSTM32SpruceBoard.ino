#include <RTClock.h>

RTClock rt (RTCSEL_LSE); // initialise
uint32 tt; 

#define LED_PIN PB1

// This function is called in the attachSecondsInterrpt
void blink () 
{
 digitalWrite(LED_PIN,!digitalRead(LED_PIN));
}

void setup() 
{
  Serial1.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  rt.attachSecondsInterrupt(blink);// Call blink
}



void loop() 
{
  
  if (rt.getTime()!=tt)
  {
    tt = rt.getTime();
    
    Serial1.print("time is: ");
    Serial1.println(tt);
  }
}

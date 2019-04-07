/*
Version: 0.1
This code was written by Grook.net
Please feel free to edit/add any thing to the code but email it back to us for sharing it 
www.grook.net
http://www.youtube.com/GROOKnet
https://www.facebook.com/GROOKnet
*/


#define ECHOPIN 5        // Pin to receive echo pulse 
#define TRIGPIN 6
#include <Servo.h>

Servo myservo; // create servo object to control a servo
int pos = 0; // variable to store the servo position
void setup() {
 Serial.begin(9600); 

  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  pinMode(3,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);
  pinMode(ECHOPIN, INPUT); 
  pinMode(TRIGPIN, OUTPUT); 
}

void Print (int R , int T)
{
   Serial.print(R);Serial.print(", ");
   Serial.print(T);Serial.println(".");
   delay(100);
}

float Distance () {
  
  digitalWrite(TRIGPIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIGPIN, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIGPIN, LOW);
  // Distance Calculation
  
  float distance = pulseIn(ECHOPIN, HIGH); 
  distance= distance/58; 
  return(distance);
}

void loop() {
  
      myservo.write(45); // tell servo to go to position in variable 'pos'
  
 
  delay(2000);
  
  
  for(pos = 45; pos <= 135; pos += 3) // goes from 45 degrees to 135 degrees
  { // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    
  Print(Distance() , pos);    
    delay(10); // waits 15ms for the servo to reach the position
  }
  delay(1000);
    
    
  for(pos = 135; pos>=45; pos-=3) // goes from 135 degrees to 45 degrees
  {
    myservo.write(pos); // tell servo to go to position in variable 'pos'
      Print(Distance() , pos);    

    delay(10); // waits 15ms for the servo to reach the position
  }  
  

  
}




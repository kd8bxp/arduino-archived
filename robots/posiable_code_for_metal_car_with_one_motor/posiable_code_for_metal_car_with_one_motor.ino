/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object

}

void loop() {
 myservo.write(90); //straight
delay(2000);
//myservo.write(140); //turn right
//delay(2000);
myservo.write(60); //turn left
delay(2000);
}


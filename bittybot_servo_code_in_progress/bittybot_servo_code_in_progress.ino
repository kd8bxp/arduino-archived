/************************************************/
#include <Servo.h>
/************************************************/
Servo myservo;//create servo object to control a servo
/************************************************/
void setup()
{
  myservo.attach(9);//attachs the servo on pin 9 to servo object

}
/*************************************************/
void loop()
{  
    myservo.write(91);//back to 0 degrees 
  delay(1000);//wait for a second

  myservo.write(45);//goes to 45 degrees 
  delay(1000);//wait for a second.33

  myservo.write(91);//goes to 90 degrees
  delay(1000);//wait for a second
  myservo.write(135);//back to 75 degrees 
  delay(1000);//wait for a second.33

  myservo.write(91);//back to 0 degrees 
  delay(1000);//wait for a second
}
/**************************************************/

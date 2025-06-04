#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define PHOTO_CELL_LEFT A3
#define PHOTO_CELL_RIGHT A4
#define LEFTSWITCH 6 //D6 Blue Connector Marked "B"
#define RIGHTSWITCH 5 //D5 Blue Connector Marked "G"
#define M1SPD 10 //D10 PWM M1 speed pin
#define M2SPD 11 //D11 PWM M2 speed pin
#define M2DIR 13 //D13 PWM M2 direction
#define M1DIR 12 //D12 M1 direction
#define BUZZER 4 //D4
#define PINGT 7 //D7 yellow connector
#define PINGR 8 //D8 yellow connector

//LCD Veriables
float pingTime;  //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound = 776.5; //Speed of sound in miles per hour when temp is 77 degrees.
const int DURATION_PER_INCH = 550; //The approximate duration in microseconds of the echo for an object one inch away.
unsigned long echoDuration = 0; //Used to store the duration of the echo in microseconds

//Motor Information
const int lowspd = 100; //PWM for stall of motors
int setspd = 150; //This is PWM the motors will run at.

//Servo Information
const int raise = 165; //Move the servo to position 180
const int lower = 120; //Move the servo to position 9
Servo myservo;  // create servo object to control a servo

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup() {
  Serial.begin(9600);

  //LCD Setup
  lcd.begin(16,2); //Tell Arduino to start your 16 column 2 row LCD
  lcd.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  lcd.print("Target Distance:");  //Print Message on First Row

  //Switches Setup
  pinMode(LEFTSWITCH, INPUT_PULLUP);
  pinMode(RIGHTSWITCH, INPUT_PULLUP);

  //Motor Setup
  pinMode(M1SPD, OUTPUT);
  pinMode(M2SPD, OUTPUT);
  pinMode(M1DIR, OUTPUT);
  pinMode(M2DIR, OUTPUT);
  stop();

  //Servo Setup
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  penUp();

}

void loop() {
  setSpd(&setspd, &setspd); //set the PWM for motor1 and motor2
  distanceUpdate();
  delay(250);
  Forward();
  delay(500);
  Left();
  delay(400);
  Right();
  delay(400);
  Left();
  delay(400);
  setspd = (255 - setspd);
  setSpd(setspd, setspd);
  Backward();
  delay(400);
  stop();
  delay(400);
  penDown();
  delay(400);
  penUp();
  delay(1500);
  setspd = (255 - setspd);

}

void stop() {
  analogWrite(M1SPD, 0);
  digitalWrite(M1DIR, LOW);
  analogWrite(M2SPD, 0);
  digitalWrite(M2DIR, LOW);
}

void setSpd(int m1, int m2) {
  if (m1 < lowspd) {
    m1 = lowspd;
  }
  if (m2 < lowspd) {
    m2 = lowspd;
  }
  if (m1 > 255) {
    m1 = 255;
  }
  if (m2 > 255) {
    m2 = 255;
  }
  analogWrite(M1SPD, m1);
  analogWrite(M2SPD, m2);
}

void Forward() {
  digitalWrite(M1DIR, LOW);
  digitalWrite(M2DIR, LOW);
}

void Backward() {
  digitalWrite(M1DIR, HIGH);
  digitalWrite(M2DIR, HIGH);
}

void Left() {
  digitalWrite(M1DIR, HIGH);
  digitalWrite(M2DIR, LOW);
}

void Right() {
  digitalWrite(M1DIR, LOW);
  digitalWrite(M2DIR, HIGH);
}

void penUp() {
  myservo.write(raise);
  delay(100);
}

void penDown() {
  myservo.write(lower);
  delay(100);
}

void distanceUpdate() {
  pinMode(PINGT, OUTPUT);
  digitalWrite(PINGT, LOW);
  delayMicroseconds(2000);
  digitalWrite(PINGT, HIGH); //Send voltage to the trigger pin to begin the ultrasound pusle
  delayMicroseconds(15); //Pause the program for 10 microseconds (to produce a 10 microsecond pulse)
  digitalWrite(PINGT, LOW); //Stop sendig voltage to the trigger pin, ending the pulse
  delayMicroseconds(10);
  pinMode(PINGR, INPUT);
  echoDuration = pulseIn(PINGR, HIGH); //Get the duration of the echo coming back in microseconds
  pingTime = echoDuration;
  pingTime /= 1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
  pingTime /= 3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
  targetDistance = speedOfSound * pingTime; //This will be in miles, since speed of sound was miles per hour
  targetDistance /= 2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
  targetDistance *= 63360;    //Convert miles to inches by multipling by 63360 (inches per mile)

  lcd.setCursor(0, 1); //Set cursor to first column of second row
  lcd.print("                "); //Print blanks to clear the row
  lcd.setCursor(0, 1);  //Set Cursor again to first column of second row
  lcd.print(targetDistance); //Print measured distance
  lcd.print(" inches");  //Print your units.
  delay(1);
  digitalWrite(PINGT, LOW); //Set trigger pin low
}


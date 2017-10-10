#include "WEMOS_Motor.h"

const int M1_ENC_1_PIN=13;
const int M1_ENC_2_PIN=12;
const int M2_ENC_1_PIN=14;
const int M2_ENC_2_PIN=0;
Motor M1(0x30, _MOTOR_A, 1000);
Motor M2(0x30, _MOTOR_B, 1000);

volatile int m1_dir = 0;
volatile int m2_dir = 0;
long m1_steps, m2_steps, m1_steps_last, m2_steps_last;
unsigned long next_print_ms;

const byte buffSize=40;
char inputBuffer[buffSize];
int bytesIn=0;

int rotationCount = 360; //one rotation
int setSpeed = 100; //PWM for motor
int target = 10; //how many times to roate wheel
int targetCount = 0;

void setup() {
  m1_steps=m2_steps=0;
  m1_steps_last=m2_steps_last=0;
  next_print_ms=0;

  Serial.begin(9600);
  pinMode(M1_ENC_1_PIN, INPUT_PULLUP);
  pinMode(M1_ENC_2_PIN, INPUT_PULLUP);
  attachInterrupt(M1_ENC_1_PIN, m1_interrupt_change, RISING);
  pinMode(M2_ENC_1_PIN, INPUT_PULLUP);
  pinMode(M2_ENC_2_PIN, INPUT_PULLUP);
 attachInterrupt(M2_ENC_1_PIN, m2_interrupt_change, RISING);
 delay(5000);
 M2.setmotor(_CCW, setSpeed);
 M1.setmotor(_CCW, setSpeed);
targetCount = rotationCount * target;
}

void loop() {
  yield();
  if (millis()>=next_print_ms || m1_steps!=m1_steps_last || m2_steps!=m2_steps_last){
    // the "last" variables are used to prevent spamming of the same message over and over
    Serial.println((String)"m1="+m1_steps+",m2="+m2_steps);
    
    // ever 10 seconds or so send the steps regardless of whether they're changed or not
    next_print_ms=millis()+100;

  //  if (m1_steps >= targetCount - (setSpeed % setSpeed)) {M2.setmotor(_SHORT_BRAKE); M2.setmotor(_STOP); while(1) {yield();}}
   checkTarget();
    m1_steps_last=m1_steps;
    m2_steps_last=m2_steps;
    //delay(1000);
  }

  //getSerialData();
}


void checkTarget() {
  int diff = targetCount - m1_steps;
  if (diff <= 75) { M2.setmotor(_CCW, setSpeed % 20); }
  //if (diff <= 30) { M2.setmotor(_CCW, setSpeed % 15); } 
  if (diff <= 2 ) { M2.setmotor(_SHORT_BRAKE); M2.setmotor(_STOP); /*while(1) {yield();}*/}
  
}

void getSerialData(){
  if (Serial.available() > 0){
    char x=Serial.read();
    if (x=='\n'){
      inputBuffer[bytesIn]=0;
      serialDataHandler(inputBuffer);
      bytesIn=0;
    } else {
      inputBuffer[bytesIn]=x;
      bytesIn++;
      if (bytesIn==buffSize){
        bytesIn=buffSize-1;
      }
    }
  }
}

void serialDataHandler(char inputString[buffSize]){
  if (strcmp(inputString, "get_encoders")==0){
    Serial.println((String)"m1="+m1_steps+",m2="+m2_steps);
  } else if (strcmp(inputString, "reset_encoders")==0){
    m1_steps=m2_steps=0;
    Serial.println((String)"m1="+m1_steps+",m2="+m2_steps);
  }
}

void m1_interrupt_change() {
  detachInterrupt(M1_ENC_1_PIN);
  unsigned long snap1 = millis(); //digitalRead(M1_ENC_1_PIN);
  //Serial.print(" ");
  //Serial.println(snap1);
  int snap2 = digitalRead(M1_ENC_2_PIN);
  if (snap2 == 0) {
    // encoder channels are the same, direction is positive
    m1_dir = 1;
    m1_steps++;
  } else {
    // encoder channels are not the same, direction is negative
   m1_dir = -1;
    m1_steps--;
  }
  attachInterrupt(M1_ENC_1_PIN, m1_interrupt_change, RISING);
}

void m2_interrupt_change() {
  detachInterrupt(M2_ENC_1_PIN);
  unsigned long snap1 = millis(); //digitalRead(M1_ENC_1_PIN);
  //Serial.print(" ");
  //Serial.println(snap1);
  int snap2 = digitalRead(M2_ENC_2_PIN);
  if (snap2 == 0) {
    // encoder channels are the same, direction is positive
    m2_dir = -1;
    m2_steps--;
  } else {
    // encoder channels are not the same, direction is negative
   m2_dir = 1;
    m2_steps++;
  }
  attachInterrupt(M2_ENC_1_PIN, m2_interrupt_change, RISING);
}

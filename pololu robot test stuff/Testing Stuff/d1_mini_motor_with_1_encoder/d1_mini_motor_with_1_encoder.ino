const int M1_ENC_1_PIN=13;
const int M1_ENC_2_PIN=D8;
const int M2_ENC_1_PIN=D6;
const int M2_ENC_2_PIN=D5;

volatile int m1_dir = 0;
volatile int m2_dir = 0;
long m1_steps, m2_steps, m1_steps_last, m2_steps_last;
unsigned long next_print_ms;

const byte buffSize=40;
char inputBuffer[buffSize];
int bytesIn=0;
#include "WEMOS_Motor.h"

int pwm;

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B


void setup() {
  m1_steps=m2_steps=0;
  m1_steps_last=m2_steps_last=0;
  next_print_ms=0;

  Serial.begin(9600);
  pinMode(M1_ENC_1_PIN, INPUT_PULLUP);
  pinMode(M1_ENC_2_PIN, INPUT);
  attachInterrupt(M1_ENC_1_PIN, m1_interrupt_change, FALLING);
  //pinMode(M2_ENC_1_PIN, INPUT);
  //pinMode(M2_ENC_2_PIN, INPUT);
  attachInterrupt(M2_ENC_1_PIN, m2_interrupt_change, CHANGE);
}

void loop() {
  if (millis()>=next_print_ms || m1_steps!=m1_steps_last || m2_steps!=m2_steps_last){
    // the "last" variables are used to prevent spamming of the same message over and over
    Serial.println((String)"m1="+m1_steps+",m2="+m2_steps);
    
    // ever 10 seconds or so send the steps regardless of whether they're changed or not
    next_print_ms=millis()+10;
    
    m1_steps_last=m1_steps;
    m2_steps_last=m2_steps;
  }
for (pwm = 0; pwm <= 100; pwm++)
  {
    M1.setmotor( _CW, pwm);
    M2.setmotor(_CW, 100-pwm);
    Serial.printf("A:%d%, B:%d%, DIR:CW\r\n", pwm,100-pwm);
     Serial.println((String)"m1="+m1_steps+",m2="+m2_steps);
  }
  
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  Serial.println("Motor A&B STOP");
  delay(200);
  
  for (pwm = 0; pwm <=100; pwm++)
  {
    M1.setmotor(_CCW, pwm);
    M2.setmotor(_CCW, 100-pwm);
    Serial.printf("A:%d%, B:%d%, DIR:CCW\r\n", pwm,100-pwm);

  }
  
  M1.setmotor(_STOP);
  M2.setmotor( _STOP);
  delay(200);
  Serial.println("Motor A&B STOP");

  M1.setmotor(_SHORT_BRAKE);
  M2.setmotor( _SHORT_BRAKE);
  Serial.println("Motor A&B SHORT BRAKE");  
  delay(1000);
  
  M1.setmotor(_STANDBY);//Both Motor standby
  //M2.setmotor( _STANDBY);
  Serial.println("Motor A&B STANDBY");  
  delay(1000);
  
 // getSerialData();
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
  if (digitalRead(M1_ENC_1_PIN) == digitalRead(M1_ENC_2_PIN)) {
    // encoder channels are the same, direction is positive
    m1_dir = 1;
    m1_steps++;
  } else {
    // encoder channels are not the same, direction is negative
    m1_dir = -1;
    m1_steps--;
  }
}

void m2_interrupt_change() {
  if (digitalRead(M2_ENC_1_PIN) == digitalRead(M2_ENC_2_PIN)) {
    m2_dir = 1;
    m2_steps++;
  } else {
    m2_dir = -1;
    m2_steps--;
  }
}

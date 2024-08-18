// SainSmart Instabots Upright Rover rev. 2.0
// http://www.sainsmart.com


#include <Wire.h>
#include <SPI.h>  
#include <Mirf.h>
#include "nRF24L01.h"
#include <MirfHardwareSpiDriver.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 accelgyro;
MPU6050 initialize;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define Gry_offset 0  //The offset of the gyro
#define Gyr_Gain 16.348
#define Angle_offset 0  // The offset of the accelerator
#define RMotor_offset 0 // The offset of the Motor
#define LMotor_offset 0  // The offset of the Motor
#define pi 3.14159 

float kp = 12.7, ki = 0.01, kd = 0.0;
float Angle_Raw, Angle_Filtered, omega;
float Turn_Speed = 0, Turn_Speed_K = 0;
float Run_Speed = 0, Run_Speed_K = 0;
float LOutput,ROutput;

unsigned long preTime = 0;
unsigned long lastTime = 0;
float Input, Output;
float errSum, dErr, error, lastErr;
int timeChange; 

int TN1=4; //3;
int TN2=3; //4;
int ENA=9;
int TN3=5;
int TN4=6;
int ENB=10;

struct Axis  // Datas from remote control 
{
  uint16_t axis_1;
  uint16_t axis_2;
  uint16_t axis_3;
  uint16_t axis_4;
  uint16_t axis_5;
  uint16_t axis_6;
  uint16_t axis_7;
  uint16_t axis_8;
};
Axis axis_x;

struct Gesture  // Datas send back to remote control 
{
  float angle;
  float omega;
  int speed; 
  int P;
  int I;
  int D;
  uint16_t null_1; 
  uint16_t null_2;
};
Gesture data;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  /* If the robot was turned on with the angle over 45(-45) degrees,the wheels will not spin until the robot is in right position. */
  accelgyro.initialize();
  initialize.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);;
  for(int i = 0; i < 200; i++)  // Looping 200 times to get the real gesture when starting
  {
    Filter();
  }
  if (abs(Angle_Filtered) < 45)  // Start the robot after cleaning data
  {
    omega = Angle_Raw = Angle_Filtered = 0;
    Filter();
    Output = error = errSum = dErr = Run_Speed = Turn_Speed = 0;
    myPID();
  }

  pinMode(TN1,OUTPUT);
  pinMode(TN2,OUTPUT);
  pinMode(TN3,OUTPUT);
  pinMode(TN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  
  // 24L01 initialization
  Mirf.spi = &MirfHardwareSpi;   
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = 16;
  Mirf.config();
}

void loop() 
{
  Filter();
  Serial.print("  Angle = ");
  Serial.print(Angle_Filtered);
  // If angle > 45 or < -45 then stop the robot
  if (abs(Angle_Filtered) < 45)
  {
    Recive();
    myPID();
    PWMControl();
  }
  else
  {
    analogWrite(ENA, 0);  // Stop the wheels
    analogWrite(ENB, 0);  // Stop the wheels
    
    for(int i = 0; i < 100; i++)  // Keep reading the gesture after falling down
    {
      Filter();
    }
    
    if(abs(Angle_Filtered) < 45)  // Empty datas and restart the robot automaticly
    {
      for(int i = 0; i <= 500; i++)  // Reset the robot and delay 2 seconds
      {
        omega = Angle_Raw = Angle_Filtered = 0;
        Filter();
        Output = error = errSum = dErr = Run_Speed = Turn_Speed = 0;
        myPID();
      }
    }
  }
  Serial.println();
}

void Recive()
{
  if(!Mirf.isSending() && Mirf.dataReady()){  // Read datas from the romote controller
    Mirf.getData((byte *) &axis_x);
    /*Serial.print("axis_1=");
    Serial.print(axis_x.axis_1);
    Serial.print("  axis_2=");
    Serial.println(axis_x.axis_2);
    Serial.print("  axis_3=");
    Serial.print(axis_x.axis_3);
    Serial.print("  axis_4=");
    Serial.print(axis_x.axis_4);
    Serial.print("  axis_5=");
    Serial.print(axis_x.axis_5);
    Serial.print("  axis_6=");
    Serial.print(axis_x.axis_6);
    Serial.print("  axis_7=");
    Serial.print(axis_x.axis_7);
    Serial.print("  axis_8=");
    Serial.println(axis_x.axis_8);*/
    
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send((byte *) &data);  // Send datas back to the controller
    
    if(axis_x.axis_1 >= 520)  // Y axis datas from joystick_1
    {
      Turn_Speed = map(axis_x.axis_1, 520, 1023, 0, 30);  // 30 is the max turning speed, you can set it yourself
    }
    else if(axis_x.axis_1 <= 480)
    {
      Turn_Speed = map(axis_x.axis_1, 480 ,0, 0, -30);  // 30 is the max turning speed, you can set it yourself
    }
    else
    {
      Turn_Speed = 0;
    }
    
    if(axis_x.axis_2 >= 520)  // X axis datas from joystick_1
    {
      Run_Speed_K = map(axis_x.axis_2, 520, 1023, 0, 50);  // 50 is the max running speed, you can set it yourself
      Run_Speed += Run_Speed_K;
    }
    else if(axis_x.axis_2 <= 480)
    {
      Run_Speed_K = map(axis_x.axis_2, 480, 0, 0, -50);  // 50 is the max running speed, you can set it yourself
      Run_Speed += Run_Speed_K;
    }
    else
    {
      Run_Speed_K = 0;
    }
  }
  else
  {
    axis_x.axis_1 = axis_x.axis_2 = 500;
  }
}

void Filter()
{
  // Raw datas from MPU6050
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Angle_Raw = (atan2(ay, az) * 180 / pi + Angle_offset);
  omega =  gx / Gyr_Gain + Gry_offset;
  // Filters datas to get the real gesture
  unsigned long now = millis();
  float dt = (now - preTime) / 1000.00;
  preTime = now;
  float K = 0.8;
  float A = K / (K + dt);
  Angle_Filtered = A * (Angle_Filtered + omega * dt) + (1 - A) * Angle_Raw;
  // Preparing datas which need to be shown on the LCD.
  data.omega = omega;
  data.angle = Angle_Filtered;
}

void myPID()
{ 
  // You can see the values both on the serial monitor and the remote controller.
  // Watch the tutorial video on youtube.
  //kp = analogRead(A0) * 0.1;      Serial.print("  kp = "); Serial.print(kp);
  //ki = analogRead(A1) * 0.0005;   Serial.print("  ki = "); Serial.print(ki);
  //kd = analogRead(A2) * 1.0;      Serial.print("  kd = "); Serial.print(kd);
  //Serial.println("");
  // Preparing datas which need to be shown on the LCD.
  //data.P = analogRead(A0);
  //data.I = analogRead(A1);
  //data.D = analogRead(A2);
  // Calculating the output values using the gesture values and the PID values.
  unsigned long now = millis();
  timeChange = (now - lastTime);
  lastTime = now;
  
  error = Angle_Filtered;  // Proportion
  //errSum += error * timeChange;  // Integration
  errSum += (error * timeChange)/1000;  // Integration
  //dErr = (error - lastErr) / timeChange;  // Differentiation
  dErr = ((error - lastErr) / timeChange)*1000;  // Differentiation
  Output = kp * error + ki * errSum + kd * dErr;
  lastErr = error;
  
  LOutput = Output - Turn_Speed + Run_Speed;
  ROutput = Output + Turn_Speed + Run_Speed;
  data.speed = (LOutput + ROutput) / 2;
}

void PWMControl()
{
  if(LOutput > 0)
  {
    digitalWrite(TN1, HIGH);
    digitalWrite(TN2, LOW);
  }
  else if(LOutput < 0)
  {
    digitalWrite(TN1, LOW);
    digitalWrite(TN2, HIGH);
  }
  else
  {
    digitalWrite(ENA, 0);
  }
  if(ROutput > 0)
  {
    digitalWrite(TN3, HIGH);
    digitalWrite(TN4, LOW);
  }
  else if(ROutput < 0)
  {   
    digitalWrite(TN3, LOW);
    digitalWrite(TN4, HIGH);
  }
  else
  {
    digitalWrite(ENB, 0);
  }
    analogWrite(ENA, min(255, (abs(LOutput) + LMotor_offset)));
    analogWrite(ENB, min(255, (abs(ROutput) + RMotor_offset)));
}

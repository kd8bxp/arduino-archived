// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include <IRremote.h>
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
const int redLED = 8; // Analog output pin that the LED is attached to
const int blueLED = 11; // Analog output pin that the LED is attached to
const int greenLED = 10; // Analog output pin that the LED is attached to
const int servoLeft = 4;
const int servoRight = 5;
unsigned long time;
unsigned long currentTime;
unsigned long timeNow;
unsigned long lastTime;
int deadBand = 25;
int output;
double errSum, lastErr;
double kp = 1.39; //0.9; //1.4 //3.3; //2.26;
//double kp = 1.92;
//double ki = 0.27;
double ki = .0; //0.001;
//double kd = 2.6;
double kd = 3.59; //0.6; //3.4 //3.5;
double error;
double dErr;
double accelTotal;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int sumgx, sumgy, sumgz;
int sumax, sumay, sumaz;
int ayTotal;
int moveData = -84; //-84;
//int moveData = -86;
int variableDelay1;
int variableDelay2;
int neutral1 = 604; //604;//int neutral1 = 600; //int neutral1 = 486;
int neutral2 = 400; //410;//int neutral2 = 470; //int neutral2 = 482;
static int setpoint = 14965;
static int input = 14965;
static int lastInput = 14965;
//static int setpoint = 20250;
//static int input = 20250;
//static int lastInput = 20250;
bool alternateServo = false;
void setup()
{
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    Serial.begin(9600);
    accelgyro.initialize();
//#define MPU6050_DLPF_BW_256         0x00
//#define MPU6050_DLPF_BW_188         0x01
//#define MPU6050_DLPF_BW_98          0x02
//#define MPU6050_DLPF_BW_42          0x03
//#define MPU6050_DLPF_BW_20          0x04
//#define MPU6050_DLPF_BW_10          0x05
//#define MPU6050_DLPF_BW_5           0x06
   
//int16_t getAccelerationY()
accelgyro.setDLPFMode(MPU6050_DLPF_BW_188);
    for (int i = 1; i < 33; i = i + 1)
    {
      accelgyro.getRotation(&gx, &gy, &gz);
      sumgx = sumgx + gx;
    }
    sumgx = sumgx / 32;
    for (int i = 1; i < 33; i = i + 1)
    {
      accelgyro.getAcceleration(&ax, &ay, &az);
      sumay = sumay + ay;
    }
    sumay = sumay / 32;
 
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(servoLeft, OUTPUT);
    pinMode(servoRight, OUTPUT);   
}
void loop()
{
  currentTime = millis();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  double accelData = (ay-sumay)>>3;
  accelTotal = accelData + setpoint;
  double gyroData = (gx-sumgx)>>3;
  //input = (0.99*(lastInput+(gyroData*0.01))+(0.01*accelTotal));
    //input = 0.92*(lastInput+(gyroData*0.01))+(0.01*accelTotal);
  input = (0.99*(lastInput+(gyroData*0.05))+(0.01*accelTotal));

  calcOutput();
  servoOutput();
  ledoutput();
  lastErr = error;
  correctTime();
  lastInput = input;
}
void calcOutput()
{
  error = setpoint - input;
  error = error + moveData;
  errSum = errSum + (error * 0.01); //intergral part sum of errors past
 
  double dInput = input - lastInput;
  output = (kp * error) + (ki * errSum) - (kd * dInput); 
}
void correctTime()
{
  //take the start time (current) and now time and work out the required delay for 10ms
  timeNow = millis();
  int delayTimeCorrect = timeNow - currentTime;
  delay(10 - delayTimeCorrect);
} 
void servoOutput()
{
  variableDelay1 = neutral1 + output;
  variableDelay2 = neutral2 - output;
  if (variableDelay1 > 1000)
  {
    variableDelay1 = 1000;
  }
  else if (variableDelay1 < 1)
  {
    variableDelay1 = 1;
  } 
  if (variableDelay2 > 1000)
  {
    variableDelay2 = 1000;
  }
  else if (variableDelay2 < 1)
  {
    variableDelay2 = 1;
  } 
  if (alternateServo == false)
  {
    digitalWrite(servoRight, HIGH);
    delayMicroseconds(1000);
    delayMicroseconds(variableDelay1);
    digitalWrite(servoRight, LOW);
  }
  else if (alternateServo != false)
  {
    digitalWrite(servoLeft, HIGH);
    delayMicroseconds(1000);
    delayMicroseconds(variableDelay2);
    digitalWrite(servoLeft, LOW);
  }
alternateServo = !alternateServo;

}
void ledoutput()
{
  int setpoint2 = setpoint + moveData;
    if ((input < setpoint2 + deadBand) && (input > setpoint2 - deadBand))
      {
        digitalWrite(redLED, 0);
        digitalWrite(blueLED, 1);
        digitalWrite(greenLED, 1);
      }
      else if (input < setpoint2 + deadBand)
      {
        digitalWrite(redLED, 1);
        digitalWrite(blueLED, 0);
        digitalWrite(greenLED, 1);
      } 
      else if (input > setpoint2 - deadBand)
      {
        digitalWrite(redLED, 1);
        digitalWrite(blueLED, 1);
        digitalWrite(greenLED, 0);
      } 
}
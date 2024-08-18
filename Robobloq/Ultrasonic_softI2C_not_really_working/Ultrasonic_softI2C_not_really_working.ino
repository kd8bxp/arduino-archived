// SoftI2C using port 2 (A2, A3) I can get Green and Blue to work pretty well
// RED locks the device up, and so far no luck getting the ultrasonic to give a distance


//#include <Wire.h>
#define I2C_TIMEOUT 1
#define I2C_PULLUP 1
#define SDA_PORT PORTC
#define SDA_PIN 2 // = A4
#define SCL_PORT PORTC
#define SCL_PIN 3 // = A5
#include <SoftI2CMaster.h>
#include "avr/wdt.h"
#include <inttypes.h>
#include <Arduino.h>

double ul_distance =0;
uint8_t _SignalPin = A6;
volatile bool _measureFlag;
      volatile long _lastEnterTime = 0;
      volatile float _measureValue;
      
void setup() {
  pinMode(A6, INPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
pinMode(7, OUTPUT);
  //Wire.begin();
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(7, HIGH);
 
    Serial.begin(9600);
    if (!i2c_init()) Serial.println("I2C init failed");
    RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
 //  ul_distance = Uldistance();
//   Serial.println(ul_distance);
//   delay(500);
 // put your setup code here, to run once:
 
    RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    
    delay(100);
  //  RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    RB_Ultrasonic_SetRGB(0x40,0xA1,0,200,0);
    delay(500);
    //RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    delay(500);
  //  RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,200);
    delay(500);
    
    //RB_Ultrasonic_SetRGB(0x40,0xA1,0,0,0);
    delay(500);
    Serial.println(Uldistance(300));
}

void RB_Ultrasonic_SetRGB(uint8_t SlaveAddress,uint8_t RegisterAddress,uint8_t Red,uint8_t Green,uint8_t Blue)
{
    if (!i2c_start_wait((SlaveAddress<<1) | I2C_WRITE)) {
       Serial.println("I2C device busy");
    delay(10);
    i2c_stop();
    return;
    }
   i2c_write(RegisterAddress);
   i2c_write(Red);
   i2c_write(Green);
   i2c_write(Blue);
   i2c_stop();
  /* Wire.beginTransmission(SlaveAddress);
   Wire.write(RegisterAddress);
   Wire.write(Red);
   Wire.write(Green);
   Wire.write(Blue);
   Wire.endTransmission();
   */
   
}


double distanceCm()
{
  uint16_t MAXcm = 250;
  long distance = measure(MAXcm * 100+ 200);
  if(distance == 0)
  {
    distance = MAXcm * 58;
  }
  return( (double)distance / 58.0);
}

double distanceInch()
{
  uint16_t MAXinch= 180;
  long distance = measure(MAXinch * 145 + 200);
  if(distance == 0)
  {
    distance = MAXinch * 148;
  }
  return( (double)(distance / 148.0) );
}


long measure(unsigned long timeout)
{
   
  long duration;
  if(millis() - _lastEnterTime > 16)
  {
    _measureFlag = true; 
  }

  if(_measureFlag == true)
  {
    _lastEnterTime = millis();
    _measureFlag = false;
// (i2c_start_wait((0x40<<1) | I2C_WRITE)); 
   i2c_start(0x40<<1 | I2C_WRITE);
   i2c_write(0xA2);
   i2c_write(0xA2);
   i2c_write(0xA2);
   i2c_write(0x01);
   i2c_stop();
/*  
    Wire.beginTransmission(0x40);
    Wire.write(0xA2);
    Wire.write(0xA2);
    Wire.write(0XA2);
    Wire.write(0X01);
    Wire.endTransmission();
    */
    pinMode(_SignalPin, INPUT);
    duration = pulseIn(_SignalPin, LOW, timeout);
    _measureValue = duration;
  }
  else
  {
    duration = _measureValue;
  }
  return(duration);
}


double KalmanFilter(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_mid ;//= x_last
   static double x_now;
   static double p_mid ;
   static double p_now;
   static double kg;        
   static double x_last,p_last;
   x_mid=x_last;          
   p_mid=p_last+Q;        
   kg=p_mid/(p_mid+R);   
   x_now=x_mid+kg*(ResrcData-x_mid);           
   p_now=(1-kg)*p_mid;   
   p_last = p_now;       
   x_last = x_now;       
   return x_now;                
}


double Uldistance(double max_distance)
{   
  double  Distance[2];
  uint8_t i=0;
  for(i=0;i<2;i++)
  {
    
    Distance[i] = distanceCm();
    wdt_reset();
    delay(20);
  }
  if(Distance[0]>Distance[1])
     {return Distance[0];}
  else 
     {return Distance[1];}
}

void  Swap(double  A[], int i, int j)
{
    double temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

void BubbleSort(double A[], int n)
{
    for (int j = 0; j < n - 1; j++)         // 每次最大元素就像气泡一样"浮"到数组的最后
    {
        for (int i = 0; i < n - 1 - j; i++) // 依次比较相邻的两个元素,使较大的那个向后移
        {
            if (A[i] > A[i + 1])            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
            {
                Swap(A, i, i + 1);
            }
        }
    }
}

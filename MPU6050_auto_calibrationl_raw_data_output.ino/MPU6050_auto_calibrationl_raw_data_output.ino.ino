// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added multiple output formats
//                 - added seamless Fastwire support
//      2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

//===============================================================
//==            howto  Fine tune                          ===
//===============================================================
//If calibration is required to be more precise i.e. the calibrated values to 
//be closer to zero then 2 changes have to be made in the #define section below.
//the AccStepSize or GyroStepSize should be decreased. for example AccStepSize
//is by default 0.2 can be changed to 0.1.
//the second change should be "AllowedRange" value should be lowered to acceptable range
//like 50 or 10.
//===============================================================
//==                   Include Files                          ===
//===============================================================
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Average.h"
//===============================================================

//===============================================================
//==           I2c serial communication Define                ===
//===============================================================
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
//===============================================================

//===============================================================
//==              Defines Section                             ===
//===============================================================
#define ModeRange 100
//#define StabilityTime 25000 //25 seconds could be 40secs dont need only for dmp
#define LED_PIN 13
#define LED_PIN2 8
#define AllowedRange 150//this is value is in range round zero that is acceptable and no calibration of that axis is done.
#define GyroStepSize  1
#define AccStepSize  0.2 // this value can be lower to 0.1 to get calibrated data closer to zero but then calibration will take more time
#define MaxIncVal 10
//=================================================================

//===============================================================
//==            MPU6050 Object decleration                   ===
//===============================================================
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
//=================================================================


//===============================================================
//==             General vraibles                             ===
//===============================================================

Average<int16_t> Mode(ModeRange);
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t CurTempM; //for temperature reading of this sensor
char DataFlowBreak;
unsigned long currentMillis;
bool blinkState = false;
bool i;
//=================================================================


void setup() {
   // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
     currentMillis = millis(); //random can be removed
    Serial.println("Press any key to continue....");
    while(!Serial.available());


      delay(1000); //delay for stability, not necessary
    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
  
      Serial.println("Calibration Started please wait...");
      Serial.println("Calibrating Gyroscope");
      
       if(autocalibrate('g','X',accelgyro)){
          Serial.println("Gyro X axis calibrated");
       }else{
          Serial.println("Gyro X axis calibration failed");}
          
      if(autocalibrate('G','y',accelgyro)){
               Serial.println("Gyro Y axis calibrated");
      }else{
          Serial.println("Gyro Y axis calibration failed");}
          
      if(autocalibrate('g','Z',accelgyro)){
               Serial.println("Gyro Z axis calibrated");
      }else{
          Serial.println("Gyro Z axis calibration failed");}
/*
       Serial.println("Calibrating Accelerometer");
       
      if(autocalibrate('a','x',accelgyro)){
               Serial.println("Accel X axis calibrated");
      }else{
          Serial.println("Accel X axis calibration failed");}
          
      if(autocalibrate('A','Y',accelgyro)){
               Serial.println("Accel Y axis calibrated");
      }else{
          Serial.println("Accel Y axis calibration failed");}
          
     if(autocalibrate('a','z',accelgyro)){
               Serial.println("Accel Z axis calibrated");
      }else{
          Serial.println("Accel Z axis calibration failed");}
     // SetZAccelOffset(accelgyro,16384);
  */   

       Serial.println("Press X or x to stop flow of data");
      delay(1200);
      //this loop can be stopped by sending 'x' or 'X' through serial terminal
    while(1){
      
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        //CurTempM=accelgyro.getTemperature();
        //CurTempM=(int16_t)((CurTempM/340.0)+36.53);
     
           
        delay(5); //grab value after every 5 miliseconds
        Serial.print(millis()-currentMillis); Serial.print("\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz); Serial.println("\t");
       // Serial.print(CurTempM); Serial.println("\t");
        
        if(Serial.available()){
          DataFlowBreak=Serial.read();
          if(DataFlowBreak=='x' ||DataFlowBreak=='X' ) //if user enters x X the loop exits and arduino stops extracting displaying data from mpu.
            break;
          }
          
        blinkState = !blinkState;
        digitalWrite(LED_PIN2, blinkState);
      } //stopping the loop here
    while(1);
}

//The purpose of this function is to get a value out of a very variable data stream. This function uses
//mode technique to get a very accurate value. the ModeRange is defined as the maximum number of values on which modes is to be applied
//here the user have to pass a or g to indicate sensor, axis required i.e. x y z and finally mpu object
//will retun 0 if arguments are incorrect
int16_t getValue(char sensor, char axis,MPU6050 mpu ){ 

   int ii;
   int16_t dummy1,dummy2,dummy3,dummy4,dummy5; //neglecting values
   int16_t Final;

    if(sensor != 'a' && sensor !='A' && sensor != 'g' && sensor !='G'){ //if user enters anything except a A g or G return false
      return 0;
    }else if(axis !='x' && axis !='X' && axis !='y' && axis !='Y' && axis !='z' && axis !='Z'){ //if user enters anything except x X y Y z or Z return false
          return 0;}

   for(ii=0; ii<=ModeRange; ii++)
        {
          switch(sensor){ //sensor can be accelerometer or gyroscope.
            case 'a'://acceleormeter is a or A
            case 'A':
              switch(axis)
              {case 'x': //axis x
              case 'X': //axis x
                   mpu.getMotion6(&Final,&dummy1,&dummy2, &dummy3, &dummy4, &dummy5);
              break;
                case 'y':
                case 'Y':
                       mpu.getMotion6(&dummy1,&Final, &dummy2, &dummy3, &dummy4, &dummy5);
                break;
                
                case'z':
                case'Z':
                     mpu.getMotion6(&dummy1,&dummy2, &Final,&dummy3, &dummy4, &dummy5);
                break;
                }
            break;
            case 'g': //gyrometer is g or G
            case 'G': 
               switch(axis){
              case 'x': //axis x
              case 'X': //axis x
                   mpu.getMotion6(&dummy3, &dummy4, &dummy5,&Final,&dummy1,&dummy2);
              break;
                case 'y':
                case 'Y':
                       mpu.getMotion6(&dummy3, &dummy4, &dummy5,&dummy1,&Final, &dummy2);
                break;
                
                case'z':
                case'Z':
                     mpu.getMotion6(&dummy3, &dummy4, &dummy5, &dummy1,&dummy2, &Final);
                break;
                }
                
            break;
            }
       
        //delay(1);
        delayMicroseconds(500); //the primary reason to use micorseconds here is to reduce time taken for calibration
        Mode.push(Final);
        
        }
  Final=Mode.mode();

  return Final;
    }

//This function attempts to auto calibrate the Accelerometer and Gyroscope and returns true if operation is successful
//This function takes in 3 arguments 1st is sensor ('a'/'A'=Accelerometer and 'g'/'G'=Gyroscope), then 2nd argument is the axis to claibrate ('X'/'x','Y'/'y','Z'/'z') 
//and the last argument this function takes is the mpu6050 object that is created.
bool autocalibrate(char sensor,char axis,MPU6050 accelgyro){
       
        float n=1;
        float stepsize=0.5;
        int16_t initalvalue;
        int16_t calibrateValue;
        int16_t firstoutput, secondoutput;
        void (*FuncPointerToSetOffset)(MPU6050, int16_t); //a pointer to the function.
        
    if(sensor != 'a' && sensor !='A' && sensor != 'g' && sensor !='G'){ //if user enters anything except a A g or G return false
      return false;
    }else if(axis !='x' && axis !='X' && axis !='y' && axis !='Y' && axis !='z' && axis !='Z'){ //if user enters anything except x X y Y z or Z return false
          return false;}
          
        switch(sensor){ //sensor can be accelerometer or gyroscope.
            case 'a'://acceleormeter is a or A
            case 'A':
            stepsize=AccStepSize; //here the default step size for incrementing n is 0.2 
              switch(axis)
              {case 'x': //axis x
              case 'X': //axis x
                  
                  FuncPointerToSetOffset =  SetXAccelOffset;
              break;
                case 'y':
                case 'Y':
               
                    FuncPointerToSetOffset = SetYAccelOffset;
                break;
                
                case'z':
                case'Z':
               
                    FuncPointerToSetOffset =SetZAccelOffset;
                break;
                }
            break;
            case 'g': //gyrometer is g or G
            case 'G': 
            stepsize=GyroStepSize; //here the default step size for incrementing n is 1 
               switch(axis)
              {case 'x': //axis x
              case 'X': //axis x
              
                   FuncPointerToSetOffset = SetXGyroOffset;
              break;
                case 'y':
                case 'Y':
                    FuncPointerToSetOffset = SetYGyroOffset;
                break;
                
                case'z':
                case'Z':
                     FuncPointerToSetOffset = SetZGyroOffset;
                break;
                }
                
            break;
            }
      calibrateValue=getValue(sensor,axis, accelgyro); // sensor (gyro/accl), axis(x y z) and mpu object
     
      if (abs(calibrateValue) <=AllowedRange ) //when you dont need to calibrate after all. when data is near to zero in allowable range (default is 150)
       return true;
          
  
      initalvalue=calibrateValue*(-1); //inversion is important here.
     
      calibrateValue=initalvalue/n;
     FuncPointerToSetOffset(accelgyro,calibrateValue);
      firstoutput=getValue(sensor,axis, accelgyro); // sensor (gyro/accl), axis(x y z) and mpu object
      
   while(n<MaxIncVal){ //here 10 is a random select and this 10 will make sure the loop does not run for more than 50 loops (if step size is 0.2)
     n+=stepsize;
      calibrateValue=initalvalue/n;
      FuncPointerToSetOffset(accelgyro, calibrateValue);//function pointer with function arguments
      secondoutput=getValue(sensor,axis, accelgyro); // sensor (gyro/accl), axis(x y z) and mpu object
        //here we check wether the value due to previous offset  is closer to zero or the value due to current offset is closer to zero
       //additionally the sign of value is also checked i.e the moment it changes we stop (fro example value goes from +ve to -ve)
      if( abs(firstoutput) < abs(secondoutput) && (firstoutput/abs(firstoutput) * secondoutput/abs(secondoutput))!= -1 ){
        n=n-stepsize;
        FuncPointerToSetOffset(accelgyro ,initalvalue/n); //we set the previous value because the current value is more far away from zero mark
       
        break;
      
        }else{
          firstoutput=secondoutput; //if the current value is lesser than previous value than we check next value.
          }
        }
         
return true;
} //end of function

//the reason for creation of these function was that function pointer was used to point to these function.
//function pointer can not point to a static member function of an object i think. so these function had to be used.
//additionally we can use these functions to add subtract any other constants for offset calibration such as +16384 for Az can be inserted here.

void SetXAccelOffset(MPU6050 accelgyro ,int16_t value){
  
  accelgyro.setXAccelOffset(value);
  }

  void SetYAccelOffset(MPU6050 accelgyro ,int16_t value){
  
  accelgyro.setYAccelOffset(value);
  }

  void SetZAccelOffset(MPU6050 accelgyro ,int16_t value){
  
  accelgyro.setZAccelOffset(value);
  }

  void SetXGyroOffset(MPU6050 accelgyro ,int16_t value){
    
   accelgyro.setXGyroOffset(value);
   }
   
  void SetYGyroOffset(MPU6050 accelgyro ,int16_t value){
    
   accelgyro.setYGyroOffset(value);
   }

   
  void SetZGyroOffset(MPU6050 accelgyro ,int16_t value){
    
   accelgyro.setZGyroOffset(value);
   }


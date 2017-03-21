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
//==           Output Format Selected here                   ===
//===============================================================
// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL
//=================================================================

//===============================================================
//==                   Include Files                          ===
//===============================================================
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include "Average.h"
//=================================================================

//===============================================================
//==           I2c serial communication Define                ===
//===============================================================
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
//================================================================

//===============================================================
//==           General + calibration related Defines          ===
//===============================================================
#define ModeRange 100
#define StabilityTime 25000 //25 seconds, could be 40secs, only for dmp
#define LED_PIN 13
#define LED_PIN2 8
#define AllowedRange 150    //this is value is in range round zero that is acceptable and no calibration of that axis is done.
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
MPU6050 mpu;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
//=================================================================

//===============================================================
//==             DMP related Variables                   ===
//===============================================================
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
//=================================================================

//===============================================================
//==            Data output related variables                      ===
//===============================================================
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
#if defined(OUTPUT_READABLE_WORLDACCEL) || defined(OUTPUT_READABLE_REALACCEL)
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
#endif

#if defined(OUTPUT_READABLE_WORLDACCEL) || defined(OUTPUT_READABLE_REALACCEL)
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
#endif

#if defined(OUTPUT_READABLE_WORLDACCEL) || defined(OUTPUT_READABLE_REALACCEL) || defined(OUTPUT_READABLE_YAWPITCHROLL)
VectorFloat gravity;    // [x, y, z]            gravity vector
#endif

#ifdef OUTPUT_READABLE_EULER
float euler[3];         // [psi, theta, phi]    Euler angle container
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
#endif
//=================================================================
//===============================================================
//==              Function Prototypes                         ===
//===============================================================

int16_t getValue(char , char,MPU6050); //see function definition for details
bool autocalibrate(char ,char,MPU6050);
void SetXAccelOffset(MPU6050,int16_t);
void SetYAccelOffset(MPU6050,int16_t);
void SetZAccelOffset(MPU6050,int16_t);
void SetXGyroOffset(MPU6050,int16_t);
void SetYGyroOffset(MPU6050,int16_t);
void SetZGyroOffset(MPU6050,int16_t);
//=================================================================
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

//===================================================================

// ================================================================
// ===               General Variable                          ===
// ================================================================
char DataFlowBreak;
int16_t CurTempM; //for temperature reading of this sensor
unsigned long currentMillis;
bool waitstatus=false;  
Average<int16_t> Mode(ModeRange);
bool blinkState = false;
//=====================================================================

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
    mpu.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
    
    Serial.println("Press any key to continue....");
    while(!Serial.available());

       
      Serial.println("Calibration Started please wait...");
      Serial.println("Calibrating Gyroscope");
      
       if(autocalibrate('g','X',mpu)){
          Serial.println("Gyro X axis calibrated");
       }else{
          Serial.println("Gyro X axis calibration failed");}
          
      if(autocalibrate('G','y',mpu)){
               Serial.println("Gyro Y axis calibrated");
      }else{
          Serial.println("Gyro Y axis calibration failed");}
          
      if(autocalibrate('g','Z',mpu)){
               Serial.println("Gyro Z axis calibrated");
      }else{
          Serial.println("Gyro Z axis calibration failed");}

       Serial.println("Calibrating Accelerometer");
       
     /* if(autocalibrate('a','x',mpu)){
               Serial.println("Accel X axis calibrated");
      }else{
          Serial.println("Accel X axis calibration failed");}
          
      if(autocalibrate('A','Y',mpu)){
               Serial.println("Accel Y axis calibrated");
      }else{
          Serial.println("Accel Y axis calibration failed");}
          
     if(autocalibrate('A','Z',mpu)){
               Serial.println("Accel Z axis calibrated");
      }else{
          Serial.println("Accel Z axis calibration failed");}
     // SetZAccelOffset(mpu,16384);      
*/
// load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

      // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
         currentMillis = millis();
        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    
      Serial.println("Press X or x to stop flow of data");
    
    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() { 
while(1){
        if (!dmpReady){ continue;
        
        }  

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            if((millis()-currentMillis) <= StabilityTime)
            {
                 if(waitstatus==false){
              Serial.println("WAITING FOR STABILITY");
              waitStatus=true;
              }else{
                Serial.print("..");
                }
              
           }else{
            Serial.print(millis()-currentMillis); Serial.print("\t");
            Serial.print("quat\t");
            Serial.print(q.w);
            Serial.print("\t");
            Serial.print(q.x);
            Serial.print("\t");
            Serial.print(q.y);
            Serial.print("\t");
            Serial.println(q.z);
            }
            
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
             if((millis()-currentMillis) <= StabilityTime)
            {
              if(waitstatus==false){
              Serial.println("WAITING FOR STABILITY");
              waitStatus=true;
              }else{
                Serial.print("..");
                }
              
           }else{
            Serial.print(millis()-currentMillis); Serial.print("\t");
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
           }
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
             if((millis()-currentMillis) <= StabilityTime)
            {
              if(waitstatus==false){
              Serial.println("WAITING FOR STABILITY");
              waitstatus=true;
              }else{
                Serial.print("..");
                }
              
           }else{
            Serial.print(millis()-currentMillis); Serial.print("\t");
            Serial.print("\t");
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
           }
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
             if((millis()-currentMillis) <= StabilityTime)
            {
                  if(waitstatus==false){
              Serial.println("WAITING FOR STABILITY");
              waitstatus=true;
              }else{
                Serial.print("..");
                }
              
           }else{
            Serial.print(millis()-currentMillis); Serial.print("\t");
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
           }      
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
           
              if((millis()-currentMillis) <= StabilityTime)
            {
                  if(waitstatus==false){
              Serial.println("WAITING FOR STABILITY");
             waitstatus=true;
              }else{
                Serial.print("..");
                }
              
           }else{
            Serial.print(millis()-currentMillis); Serial.print("\t");
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");        
           Serial.println(aaWorld.z);
           }
        #endif
  
        if(Serial.available()){
          DataFlowBreak=Serial.read();
          if(DataFlowBreak=='x' ||DataFlowBreak=='X' ) //if user enters x X the loop exits and arduino stops extracting displaying data from mpu.
            break;
            }
          
        blinkState = !blinkState;
        digitalWrite(LED_PIN2, blinkState);
      } //stopping the loop here
   
}
 while(1); //stop sending data and enter into an infinte loop
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

   for(ii=0; ii<=ModeRange; ii++){
        
          switch(sensor){ //sensor can be accelerometer or gyroscope.
            case 'a'://acceleormeter is a or A
            case 'A':
              switch(axis){
              
              case 'x': //axis x
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
              
              case 'x': 
              case 'X': 
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
bool autocalibrate(char sensor,char axis,MPU6050 mpu){
        float n=1;
        int16_t initalvalue;
        int16_t calibrateValue;
        int16_t firstoutput, secondoutput;
        void (*FuncPointerToSetOffset)(MPU6050, int16_t);
        float stepsize=0.5;
    if(sensor != 'a' && sensor !='A' && sensor != 'g' && sensor !='G'){ //if user enters anything except a A g or G return false
      return false;
    }else if(axis !='x' && axis !='X' && axis !='y' && axis !='Y' && axis !='z' && axis !='Z'){ //if user enters anything except x X y Y z or Z return false
          return false;}
          
        switch(sensor){ //sensor can be accelerometer or gyroscope.
            case 'a'://acceleormeter is a or A
            case 'A':
            stepsize=AccStepSize;
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
            stepsize=GyroStepSize;
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
      calibrateValue=getValue(sensor,axis, mpu); // sensor (gyro/accl), axis(x y z) and mpu object
     
      if (abs(calibrateValue) <=AllowedRange ) //when you dont need to calibrate after all. when data is near to zero in allowable range (default 150)
       return true;
          
  
      initalvalue=calibrateValue*(-1); //inversion is important here.
      calibrateValue=initalvalue/n;
     FuncPointerToSetOffset(mpu,calibrateValue); //function pointer with function arguments
      firstoutput=getValue(sensor,axis, mpu); // sensor (gyro/accl), axis(x y z) and mpu object
      
   while(n<10){ //here 10 is a random select and this 10 will make sure the loop does not run for more than 50 loops (if step size is 0.2)
     n+=stepsize;
      calibrateValue=initalvalue/n;
      FuncPointerToSetOffset(mpu, calibrateValue);
      secondoutput=getValue(sensor,axis, mpu); // sensor (gyro/accl), axis(x y z) and mpu object
 //here we check wether the value due to previous offset  is closer to zero or the value due to current offset is closer to zero
       //additionally the sign of value is also checked i.e the moment it changes we stop (fro example value goes from +ve to -ve)
      if( abs(firstoutput) < abs(secondoutput) && (firstoutput/abs(firstoutput) * secondoutput/abs(secondoutput))!= -1 ){
        n=n-stepsize;
        FuncPointerToSetOffset(mpu ,initalvalue/n); //we set the previous value because the current value is more far away from zero mark
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

void SetXAccelOffset(MPU6050 mpu ,int16_t value){
  
  mpu.setXAccelOffset(value);
  }

  void SetYAccelOffset(MPU6050 mpu ,int16_t value){
  
  mpu.setYAccelOffset(value);
  }

  void SetZAccelOffset(MPU6050 mpu ,int16_t value){
  
  mpu.setZAccelOffset(value);
  }

  void SetXGyroOffset(MPU6050 mpu ,int16_t value){
    
   mpu.setXGyroOffset(value);
   }

   
  void SetYGyroOffset(MPU6050 mpu ,int16_t value){
    
   mpu.setYGyroOffset(value);
   }

   
  void SetZGyroOffset(MPU6050 mpu ,int16_t value){
    
   mpu.setZGyroOffset(value);
   }

/****************************************
 * Board    :BPI-BIT                    *
 * Hardware :v1.2&v1.4 + BPI:CAR        *
 * function :Control Motor              *
 * IDE      :Arduino                    * 
 *                                 hulk *
 ****************************************/
 
#include <Wire.h>
#include "src/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

//called this way, it uses the default address 0x40
Adafruit_PWMServoDriver BpiCar = Adafruit_PWMServoDriver();

void setup() 
{
  //Serial set up
  Serial.begin(9600);
  Serial.println("BPI-CAR Steering Engine Test!");
  //PCA9685 set up
  BpiCar.begin();
  BpiCar.setPWMFreq(1000);//usually use 1KHz,This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
}

/********************************************
 * IN1 and IN2 control the motor to reverse,*
 * PWM is responsible for the duty cycle of *
 * the motor                                *
 * ******************************************
 * Motor A: PWM:0   **   Motor B: PWM:3     *
 *          IN1:2   **            IN1:4     *
 *          IN2:1   **            IN2:5     *
 * ******************************************
 * Motor C: PWM:8   **  Motor D: PWM:13     *
 *          IN1:10  **           IN1:11     *
 *          IN2:9   **           IN2:12     *
 ********************************************
 * example code:                            *
 * BpiCar.setPWM(IN1, 4096, 0);//High       *
 * BpiCar.setPWM(IN2, 0, 4096);//Low        *
 * BpiCar.setPWM(PWM, 0, duty);//Duty       *
 * Note:                                    *
 *1.Change the direction of motor rotation, *
 *you can exchange IN1 and IN2              *
 *2.The duty cycle is variable,but it cannot*
 *exceed 4096. It is recommended to take    *
 *values in the range 0-2048, but this is   *
 *not absolute. The larger the duty,the     *
 *larger the motor current and the faster   *
 *the speed. If there is a risk of burning  *
 *the motor in the assembly.                *
 *3.The direction of rotation of the motor  *
 *is relative                               *
 ********************************************/

void loop() {
  //Motor A 
  //Direction of rotation: forward
  //Duty:1024/4096= 25%
  BpiCar.setPWM(1, 4096, 0);
  BpiCar.setPWM(2, 0, 4096);
  BpiCar.setPWM(0, 0, 1024);

  //Motor D
  //Direction of rotation: forward
  //Duty:1024/4096= 25%
  BpiCar.setPWM(11, 4096, 0);
  BpiCar.setPWM(12, 0, 4096);
  BpiCar.setPWM(13, 0, 1024);
  
  delay(5000);

  //Motor A
  //Direction of rotation: reverse
  //Duty:1024/4096= 25%
  BpiCar.setPWM(2, 4096, 0);
  BpiCar.setPWM(1, 0, 4096);
  BpiCar.setPWM(0, 0, 1024);

  //Motor D
  //Direction of rotation: reverse
  //Duty:1024/4096= 25%
  BpiCar.setPWM(12, 4096, 0);
  BpiCar.setPWM(11, 0, 4096);
  BpiCar.setPWM(13, 0, 1024);

  delay(5000);
}

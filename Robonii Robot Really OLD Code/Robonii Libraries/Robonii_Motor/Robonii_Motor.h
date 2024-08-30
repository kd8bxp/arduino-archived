#ifndef ROBONII_MOTOR_H
#define ROBONII_MOTOR_H

/*
This Library is to be used with the Robonii product. 
Schematics can be downloaded from "www.robonii.co.za"
A sample of how to use this library is given at the end of "Driver_RFID.c"
The RFID chip can not be driven from USB power.
Ensure that the RFID antenna is plugged in, else the device might get stuck in an interupt trying to decode random pulses
*/




#include <avr/io.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>


// ====== Defines =====================================================


#define POWER_FREE        	125
#define POWER_BREAK      	0
#define POWER_MAX         	PWM_TC.PER+1
#define POWER_100 			100
#define POWER_90			90
#define POWER_80 			80
#define POWER_70			70
#define POWER_60			60
#define POWER_50 			50
#define POWER_40			40
#define POWER_30			30
#define POWER_20			20
#define POWER_10			10
 
 
 
#define PWM_TC TCD0

#define LEFT_FORWARD_PWM_PORT PORTD
#define LEFT_FORWARD_PWM_PIN_MASK PIN0_bm
#define LEFT_FORWARD_PWM_TC_CCEN_bm TC0_CCAEN_bm
#define LEFT_FORWARD_PWM_CCBUF CCABUF

#define LEFT_REVERSE_PWM_PORT PORTD
#define LEFT_REVERSE_PWM_PIN_MASK PIN1_bm
#define LEFT_REVERSE_PWM_TC_CCEN_bm TC0_CCBEN_bm
#define LEFT_REVERSE_PWM_CCBUF CCBBUF


#define RIGHT_FORWARD_PWM_PORT PORTD
#define RIGHT_FORWARD_PWM_PIN_MASK PIN3_bm
#define RIGHT_FORWARD_PWM_TC_CCEN_bm TC0_CCDEN_bm
#define RIGHT_FORWARD_PWM_CCBUF CCDBUF

#define RIGHT_REVERSE_PWM_PORT PORTD
#define RIGHT_REVERSE_PWM_PIN_MASK PIN2_bm
#define RIGHT_REVERSE_PWM_TC_CCEN_bm TC0_CCCEN_bm
#define RIGHT_REVERSE_PWM_CCBUF CCCBUF


#define ENABLE_MOTORS  PORTD.DIRSET = PIN4_bm; PORTD.OUT = PORTD.OUT & (~PIN4_bm)
#define DISABLE_MOTORS PORTD.DIRCLR = PIN4_bm


//====================================================================


enum e_TMotor
  {
    Motor_Left=1,
    Motor_Right,
	Motor_Both
  } ; 
  
  enum e_TPower
  {
    Power_Forward=1,
    Power_Referse,
	Power_Break,
	Power_Free
	
  } ; 




class Robonii_Motors_Driver {
 public:
 unsigned char updated;
 Robonii_Motors_Driver(e_TMotor motorSide2);
 void InitMotor(void);
 unsigned int EvaluatePower(int power);
 void OffRight(void);
 void OffLeft(void);
 void OffBoth(void);
 void MotorPower(e_TMotor motor, char powerPercentage);
 void SetSpeed(char powerPercentage);
 void Brake();
 void Free();
 char GetSpeed();
 bool IsBraked();
 
 private:
 e_TMotor motorSide;
 char speed;
 
 ;
 
 };
  
extern Robonii_Motors_Driver Robonii_Motors;
extern Robonii_Motors_Driver Robonii_Motor_Left;
extern Robonii_Motors_Driver Robonii_Motor_Right;

#endif

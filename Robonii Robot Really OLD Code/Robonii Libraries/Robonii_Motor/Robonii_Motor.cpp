#include "Robonii_Motor.h"

//#include <avr/io.h>
//#include <compat/deprecated.h>

void MotorTEST2()
{

}




char CurrentLeftPower;
 char CurrentRightPower;
 char PreviousLeftPower;
 char PreviousRightPower;

 bool LeftBreaked;
 bool RightBreaked;

void Robonii_Motors_Driver::InitMotor(void)
{
  //Setup Timer PWM_TC for generating a PWM
  //Set compare outputs
  LEFT_FORWARD_PWM_PORT.DIRSET = LEFT_FORWARD_PWM_PIN_MASK;
  LEFT_REVERSE_PWM_PORT.DIRSET = LEFT_REVERSE_PWM_PIN_MASK;
  RIGHT_FORWARD_PWM_PORT.DIRSET = RIGHT_FORWARD_PWM_PIN_MASK;
  RIGHT_REVERSE_PWM_PORT.DIRSET = RIGHT_REVERSE_PWM_PIN_MASK;

	// Set the TC period.
 // PWM_TC.PER = FULL_DUTY_CYCLE;

	// Configure the TC for single slope mode.
  PWM_TC.CTRLB = ( PWM_TC.CTRLB & ~TC0_WGMODE_gm ) | TC_WGMODE_SS_gc;

	// Start timer by selecting a clock source.
  //PWM_TC.CTRLA = ( PWM_TC.CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_DIV1_gc;

  //PWM_TC.INTCTRLA = ( PWM_TC.INTCTRLA & ~TC0_OVFINTLVL_gm ) | TC_OVFINTLVL_LO_gc; //set interupt level for Overflow as Low

  //Set all the PWM duty cycles to zero
  PWM_TC.LEFT_FORWARD_PWM_CCBUF = 0;
  PWM_TC.LEFT_REVERSE_PWM_CCBUF = 0;
  PWM_TC.RIGHT_FORWARD_PWM_CCBUF = 0;
  PWM_TC.RIGHT_REVERSE_PWM_CCBUF = 0;
  
  RightBreaked = false;
  LeftBreaked = false;
  
  CurrentLeftPower =0;
  CurrentRightPower =0;
  PreviousLeftPower =0;
  PreviousRightPower =0;
}


Robonii_Motors_Driver::Robonii_Motors_Driver(e_TMotor motorSide2)
{
	InitMotor();
	motorSide = motorSide2;
}

unsigned int Robonii_Motors_Driver::EvaluatePower(int power)
{
  
  if(power < 0)
  {
    power = 0;
  }
  
  if (power>=100)
  {
    power = PWM_TC.PER;
  }else
  {
    power = PWM_TC.PER/100*power;
  }
  
  return power;
}


void Robonii_Motors_Driver::Robonii_Motors_Driver::OffRight()
{
  PWM_TC.CTRLB = PWM_TC.CTRLB & (~RIGHT_FORWARD_PWM_TC_CCEN_bm) & (~RIGHT_REVERSE_PWM_TC_CCEN_bm); //First make sure that all the PWMs are switched off - Right  
}

void Robonii_Motors_Driver::OffLeft()
{
   PWM_TC.CTRLB = PWM_TC.CTRLB & (~LEFT_FORWARD_PWM_TC_CCEN_bm) & (~LEFT_REVERSE_PWM_TC_CCEN_bm); //First make sure that all the PWMs are switched off - Left
}

void Robonii_Motors_Driver::OffBoth()
{
  OffRight();
  OffLeft();
  //  PWM_TC.CTRLB = PWM_TC.CTRLB & (~RIGHT_FORWARD_PWM_TC_CCEN_bm) & (~RIGHT_REVERSE_PWM_TC_CCEN_bm); //First make sure that all the PWMs are switched off
  // PWM_TC.CTRLB = PWM_TC.CTRLB & (~LEFT_FORWARD_PWM_TC_CCEN_bm) & (~LEFT_REVERSE_PWM_TC_CCEN_bm); //First make sure that all the PWMs are switched off
}


void Robonii_Motors_Driver::SetSpeed(char powerPercentage)
{
  MotorPower(motorSide,powerPercentage);
}

void Robonii_Motors_Driver::Brake()
{
  MotorPower(motorSide,POWER_BREAK);
}

void Robonii_Motors_Driver::Free()
{
  MotorPower(motorSide,POWER_FREE);
}
bool Robonii_Motors_Driver::IsBraked()
{
   if (motorSide == Motor_Left)
  {
  return LeftBreaked;
  }else if (motorSide == Motor_Right)
    {
	return RightBreaked;
	}else
	  {
	    return ((LeftBreaked*RightBreaked));
	  }  
}

char Robonii_Motors_Driver::GetSpeed()
{
  if (motorSide == Motor_Left)
  {
  return CurrentLeftPower;
  }else if (motorSide == Motor_Right)
    {
	return CurrentRightPower;
	}else
	  {
	    return ((CurrentRightPower+CurrentLeftPower)/2);
	  }  
}

void Robonii_Motors_Driver::MotorPower(e_TMotor motor, char powerPercentage)
{
  e_TPower MotorPower;
  unsigned int power=0;
  
  
  if (powerPercentage==POWER_FREE)
  {
   // Serial.print("POWER_FREE: ");
    MotorPower = Power_Free;
  }else if (powerPercentage==POWER_BREAK)
  {
    //Serial.print("Break: ");
    MotorPower = Power_Break;
  }else if (powerPercentage<0)
  {
    //Serial.print("Reverse: ");
    MotorPower = Power_Referse;
    powerPercentage=powerPercentage*(-1);
  }else 
  {
    //Serial.print("Forward: ");
    MotorPower = Power_Forward;
  }
  
  
  if ((motor == Motor_Both)||(motor ==Motor_Left))
  {//Do left motor
    OffLeft();
    PreviousLeftPower = CurrentLeftPower;
    if (MotorPower == Power_Free)
    {
      
      //Break:Thus short both to ground!
       PWM_TC.CTRLB |= LEFT_FORWARD_PWM_TC_CCEN_bm;        
        PWM_TC.CTRLB |= LEFT_REVERSE_PWM_TC_CCEN_bm;
        PWM_TC.LEFT_REVERSE_PWM_CCBUF = POWER_MAX;
        PWM_TC.LEFT_FORWARD_PWM_CCBUF = POWER_MAX;
		CurrentLeftPower = Power_Free;
		LeftBreaked = true;
    }else
    {
      //Normal power- LEFT
      power=EvaluatePower(powerPercentage);
      if ((MotorPower == Power_Forward)||(MotorPower == Power_Break))
      {//Left forward here
        PWM_TC.CTRLB |= LEFT_FORWARD_PWM_TC_CCEN_bm;
        PWM_TC.LEFT_FORWARD_PWM_CCBUF = power;
      }else
      {//Reverse here
        PWM_TC.CTRLB |= LEFT_REVERSE_PWM_TC_CCEN_bm;
        PWM_TC.LEFT_REVERSE_PWM_CCBUF = power;
      }  
	  CurrentLeftPower = power;
	  LeftBreaked = false;
    }   
	
  }//End -LEFT
  
   if ((motor == Motor_Both)||(motor ==Motor_Right))
  {//Do Right motor
    OffRight();
    PreviousRightPower = CurrentRightPower;
	
    if (MotorPower == Power_Free)
    {
      //Break:Thus short both to ground!
      PWM_TC.CTRLB |= RIGHT_FORWARD_PWM_TC_CCEN_bm;
      PWM_TC.CTRLB |= RIGHT_REVERSE_PWM_TC_CCEN_bm;
       PWM_TC.RIGHT_FORWARD_PWM_CCBUF = POWER_MAX;
      PWM_TC.RIGHT_REVERSE_PWM_CCBUF = POWER_MAX;
	  CurrentRightPower = Power_Free;
      RightBreaked = true;
    }else
    {
      //Normal power- Motor_Right
      power=EvaluatePower(powerPercentage);
      if ((MotorPower == Power_Forward)||(MotorPower == Power_Break))
      {//Motor_Right forward here
        PWM_TC.CTRLB |= RIGHT_FORWARD_PWM_TC_CCEN_bm;
        PWM_TC.RIGHT_FORWARD_PWM_CCBUF = power;
      }else
      {//Motor_Right  Reverse here
        PWM_TC.CTRLB |= RIGHT_REVERSE_PWM_TC_CCEN_bm;
        PWM_TC.RIGHT_REVERSE_PWM_CCBUF = power;
      }  
	CurrentRightPower = power;	 
	RightBreaked = false;	
    }    
  }//End -Motor_Right
  
  ENABLE_MOTORS; 
}



Robonii_Motors_Driver Robonii_Motors=Robonii_Motors_Driver(Motor_Both);
Robonii_Motors_Driver Robonii_Motor_Right=Robonii_Motors_Driver(Motor_Right);
Robonii_Motors_Driver Robonii_Motor_Left=Robonii_Motors_Driver(Motor_Left);

/*
Driver_RFID RFID=Driver_RFID();
e_rfid_state rfid_state;

uint8_t rfid_disabled_counter;
uint8_t no_sync_timeout;
bool rfid_disabled_flag;

void rfid_reset_status(void);

volatile unsigned int rfid_int_time = 0x00;
volatile unsigned char rfid_sync_state = 0x00;
volatile unsigned char rfid_valid_signal_cntr = 0;
unsigned char rfid_sample = 0;

volatile unsigned char rfid_data[10], rfid_bufdata[5];
volatile unsigned char rfid_data_index = 0;
volatile unsigned char rfid_data_row_cntr = 0;
volatile unsigned char rfid_row_parity = 0;

*/
//===================================================================




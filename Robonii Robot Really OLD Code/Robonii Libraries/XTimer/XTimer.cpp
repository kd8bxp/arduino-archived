/*
 *  Interrupt and PWM utilities for 16 bit Timer3 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified Oct 2009 by Dan Clemens to work with timer3 of the ATMega1280 or Arduino Mega
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include "xTimer.h"
#include <Arduino.h>
//#include "..\Robonii_Motor\Robonii_Motor.h"

xTimer TimerD1A=xTimer();	// preinstatiate

ISR(TCD1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{

		//while (1);
	if (TimerD1A.isrCallbackCounter1 < TimerD1A.isrCallbackCounterCompare1)
	{
	TimerD1A.isrCallbackCounter1++;
	}else
	 {
	 TimerD1A.isrCallbackCounter1 = 0;
	 
	 if (TimerD1A.isrCallback1 != 0) 
	 {
		//PORTK.DIRSET = 0x05; //TIAN
		TimerD1A.isrCallback1(); 
	 }
	 }

	if (TimerD1A.isrCallbackCounter2 < TimerD1A.isrCallbackCounterCompare2)
	{
	TimerD1A.isrCallbackCounter2++;
	}else
	 {
	 TimerD1A.isrCallbackCounter2 = 0;
	 if (TimerD1A.isrCallback2 != 0) TimerD1A.isrCallback2(); 
	 }
	 
	 if (TimerD1A.isrCallbackCounter6 < TimerD1A.isrCallbackCounterCompare6)
	{
	TimerD1A.isrCallbackCounter6++;
	}else
	 {
	 TimerD1A.isrCallbackCounter6 = 0;
	 if (TimerD1A.isrCallback6 != 0) TimerD1A.isrCallback6(); 
	 }

	if (TimerD1A.isrCallbackCounter3 < TimerD1A.isrCallbackCounterCompare3)
		{
		TimerD1A.isrCallbackCounter3++;
		}else
		 {
		 TimerD1A.isrCallbackCounter3 = 0;
		 if (TimerD1A.isrCallback3 != 0) TimerD1A.isrCallback3(); 
		 }
		 TCD1.CNT = 0;
}


xTimer::xTimer()
{
isrCallbackCounter1 = 0;
} 
//Toets net eers met 1ms
void xTimer::initialize(unsigned int microseconds) 
{	

  TCD1.CTRLA = ( TCD1.CTRLA & ~TC1_CLKSEL_gm ) | TC_CLKSEL_DIV1_gc; //no prescaler
  // Default clock setup = system clock 32MHz internal clock			WAS:31 x reference clock (1mhz)
  // Peripheral clock = 1* system clock = 32MHz		WAS: 15.5 mhz
  // Set timer to 1 ms exactly
  
  //TCD1.PER = 32000; //TIAN was 15500
  TCD1.INTCTRLA = ( TCD1.INTCTRLA & ~TC1_OVFINTLVL_gm ) | TC_OVFINTLVL_LO_gc; //set interupt level for Overflow as Low
  
	/** Clear timeslice number */
	//currentTimeSliceCount = 0;
	setPeriod(microseconds); // To complete
} //end InitTimer0


void xTimer::setPeriod(unsigned int microseconds)
{
//Leeg vir toets
unsigned int Calc;
Calc = microseconds*32;
TCD1.PER = Calc; //TIAN was 15500
}

void xTimer::setPwmDuty(char pin, int duty)
{
//Leeg vir toets
}

void xTimer::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
}

void xTimer::disablePwm(char pin)
{
}

void xTimer::attachInterrupt1(void (*isr)(), int miliseconds)
{
  if(miliseconds > 0) isrCallbackCounterCompare1 = miliseconds;
  isrCallbackCounter1=0;
  isrCallback1 = isr;                                       // register the user's callback with the real ISR
  //TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  //start();
}

void xTimer::attachInterrupt2(void (*isr)(), int miliseconds)
{
  if(miliseconds > 0) isrCallbackCounterCompare2 = miliseconds;
  isrCallbackCounter2=0;
  isrCallback2 = isr;                                       // register the user's callback with the real ISR
  //TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  //start();
}

void xTimer::attachInterrupt3(void (*isr)(), int miliseconds)
{
  if(miliseconds > 0) isrCallbackCounterCompare3 = miliseconds;
  isrCallbackCounter3=0;
  isrCallback3 = isr;                                       // register the user's callback with the real ISR
  //TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  //start();
}
void xTimer::attachInterrupt6(void (*isr)(), int miliseconds)
{
  if(miliseconds > 0) isrCallbackCounterCompare6 = miliseconds;
  isrCallbackCounter6=0;
  isrCallback6 = isr;                                       // register the user's callback with the real ISR
  //TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  //start();
}

void xTimer::detachInterrupt(unsigned char intNumer)
{
  //TIMSK3 &= ~_BV(TOIE1);                                   // clears the timer overflow interrupt enable bit 
if (intNumer == 1) isrCallback1 = 0;
if (intNumer == 2) isrCallback2 = 0;
if (intNumer == 3) isrCallback3 = 0  ;
if (intNumer == 6) isrCallback6 = 0  ;
  
 }

void xTimer::start()
{
 // TCCR3B |= clockSelectBits;
   TCD1.INTCTRLA = ( TCD1.INTCTRLA & ~TC1_OVFINTLVL_gm ) | TC_OVFINTLVL_LO_gc; //set interupt level for Overflow as Low
 
  
}

void xTimer::stop()
{
 // TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));          // clears all clock selects bits
  TCD1.CTRLA = 0;
  
}

void xTimer::restart()
{
  //TCNT3 = 0;
  TCD1.CNT = 0;
}


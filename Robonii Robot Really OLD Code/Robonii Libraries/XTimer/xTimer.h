#ifndef XTIMER_H
#define XTIMER_H

/*
 *  Interrupt and PWM utilities for 16 bit Timer3 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // Timer3 is 16 bit


class xTimer {
  public:
  
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
	
	unsigned int isrCallbackCounter1;
	unsigned int isrCallbackCounter2;
	unsigned int isrCallbackCounter3;
	
	unsigned int isrCallbackCounter6; // Sound
	
	unsigned int isrCallbackCounterCompare1;
	unsigned int isrCallbackCounterCompare2;
	unsigned int isrCallbackCounterCompare3;
	
	unsigned int isrCallbackCounterCompare6;//Used for sound
	
	

    // methods
	xTimer();
    void initialize(unsigned int microseconds=1000);
    void start();
    void stop();
    void restart();
    void pwm(char pin, int duty, long microseconds=-1);
    void disablePwm(char pin);
    void attachInterrupt1(void (*isr)(), int miliseconds=-1);
	void attachInterrupt2(void (*isr)(), int miliseconds=-1);
	void attachInterrupt3(void (*isr)(), int miliseconds=-1);
	void attachInterrupt6(void (*isr)(), int miliseconds=-1); // For Sound
    void detachInterrupt(unsigned char intNumer);
    void setPeriod(unsigned int microseconds);
    void setPwmDuty(char pin, int duty);
    void (*isrCallback1)();
	void (*isrCallback2)();
	void (*isrCallback3)();
	
	void (*isrCallback6)(); //Sound
	
	
};

extern xTimer TimerD1A;


#endif
/*
"The Troll" Arduino compatible practical joke machine!
by Patrick Thomas Mitchell
http://www.engineeringshock.com/the-troll-project-page.html

Library version 1.0.3 June 26, 2019 by LeRoy Miller

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

Copyright (c) 2019 LeRoy Miller

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
*/

/* This is still a work in progress, stay tune for updates */


#ifndef troll_h
#define troll_h

#include "Arduino.h"
#include <SPI.h>

//Pin Setup
#define SW4 8 //5
#define SW3 7 //6
#define SW2 6 //7
#define SW1 5 //8
#define LED 2
#define IN 9  //External Trigger Pin
#define SELECT 0
#define LDR A0
#define CS 10
#define _PLAY 0x98
#define _RAMPUP 0xA8 //COUT ramp up - this value never changes
#define _RAMPDOWN 0xB8 //COUT ram down

//Sound List and Play Timings
#define EVIL_LAUGH1 0x00, 4250
#define EVIL_LAUGH2 0x01, 4500
#define MEOW 0x02, 3000
#define GROWL 0x03, 10000
#define EVIL_LAUGH3 0x04, 13500
#define DYING 0x05, 5000
#define NOOOO 0x06, 3000
#define BREATHING 0x07, 8500
#define NO_MERCY 0x08, 6000
#define SCREAM1 0x09, 4000
#define SCREAM2 0x0A, 3000
#define SCREAM_FEMALE 0x0B, 2000
#define MEEP1 0x0C, 1000
#define MEEP2 0x0D, 1000
#define MEEP3 0x0E, 1000
#define SEE_YOU 0x0F, 1500
#define BURP1 0x10, 1000
#define BURP2 0x11, 1000
#define BURP3 0x12, 1000
#define BURP4 0x13, 1000
#define BURP5 0x14, 1000
#define BURP6 0x15, 1000
#define FART1 0x16, 1000
#define FART2 0x17, 1000
#define FART3 0x18, 1000
#define FART4 0x19, 1000
#define FART5 0x1A, 1000
#define FART6 0x1B, 1000
#define SEX1 0x1C, 1000
#define SEX2 0x1D, 1000
#define SEX3 0x1E, 1000
#define SEX4 0x1F, 1000
#define SEX5 0x20, 1000
#define SEX6 0x21, 1500
#define BALLS 0x22, 1000
#define ZOMBIE1 0x23, 1500
#define ZOMBIE2 0x24, 1500
#define F_CK 0x25, 1000
#define ZOMBIE3 0x26, 1500
#define SH_T 0x27, 1000
#define DOOR 0x28, 6500
#define CHALKBOARD 0x29, 4000
#define CRICKET 0x2A, 3500
#define WILHELM 0x2B, 1500
#define BABY 0x2C, 3500
#define SNAKE 0x2D, 13000
#define WEATHER 0x2E, 2000
#define RATS 0x2F, 4000
#define MOSQUITO 0x30, 11000
#define SMOKE 0x31, 500
#define GRUDGE 0x32, 10500
#define FROG 0x33, 6000
#define SCREAM_GIRL 0x34, 2500
#define MOMMY 0x35, 5500
#define KNOCK 0x36, 7000
#define FOOT_STEPS 0x37, 3000 //Timings not listed...
#define RED_ALERT 0x38,3000  //The timing maybe off for this one too
#define HEADLIGHTS 0x39, 2500 //the timing probably is off for this one

extern int timings[]; // = {4250, 4500, 3000, 10000,13500,5000,3000,8500,6000,4000,3000,2000,1000,1000,1000,1500,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1500,1000,1500,1500,1000,1500,1000,6500,4000,3500,1500,3500,13000,2000,4000,11000,500,10500,6000,2500,5500,7000,3000,2500};


class troll {
private:
    bool _SPEAKER; //IF we are going to use the internal or external speaker - changes how ramp down is used 
    int _BUTTONSTATE; //for debounce on pin 0
    int _LASTBUTTONSTATE = LOW; //for debounce on pin 0
    unsigned long _LASTDEBOUNCETIME = 0;
    unsigned long _DEBOUNCEDELAY = 50;
public:
//int waits[58] = {4250, 4500, 3000, 10000,13500,5000,3000,8500,6000,4000,3000,2000,1000,1000,1000,1500,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1500,1000,1500,1500,1000,1500,1000,6500,4000,3500,1500,3500,13000,2000,4000,11000,500,10500,6000,2500,5500,7000,3000,2500};

troll(bool SPEAKER);
void begin();
void sound(int value, int wait);
int button();
int readSwitch();
};

#endif

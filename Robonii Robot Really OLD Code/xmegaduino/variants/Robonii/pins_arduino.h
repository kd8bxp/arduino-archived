/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>
#include <stdint.h>	//TIAN
#include <Robonii.h>
#include <morepgmspace.h>
//#include <dma_driver.h>

#define REPEAT8(x) x, x, x, x, x, x, x, x
#define BV0TO7 _BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7)
#define BV7TO0 _BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0)

#define NUM_DIGITAL_PINS            35
#define NUM_ANALOG_INPUTS           9
#define EXTERNAL_NUM_INTERRUPTS     48

#define analogInputToDigitalPin(p)  ((p < 9) ? p : -1)
#define digitalPinHasPWM(p)         (((p)=9) || ((p) >= 10 && (p) <= 12) || ((p) >= 23 && (p)<= 26) || ((p) >= 32 && (p)<= 33) || ((p) = 29 )

//static const uint8_t SS    = 20;
//static const uint8_t MOSI  = 21;
//static const uint8_t MISO  = 22;
//static const uint8_t SCK   = 23;
//static const uint8_t SS1   = 28;
//static const uint8_t MOSI1 = 29;
//static const uint8_t MISO1 = 30;
//static const uint8_t SCK1  = 31;
//static const uint8_t SS2   = 36;
//static const uint8_t MOSI2 = 37;
//static const uint8_t MISO2 = 38;
//static const uint8_t SCK2  = 39;
//static const uint8_t SS3   = 44;
//static const uint8_t MOSI3 = 45;
//static const uint8_t MISO3 = 46;
//static const uint8_t SCK3  = 47;

static const uint8_t SDA = 31;
static const uint8_t SCL = 32;
static const uint8_t LED_BUILTIN = 13; //TIAN was 13

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;	// V-BAT
static const uint8_t A4 = 8;
static const uint8_t A5 = 9;
static const uint8_t A6 = 10;
static const uint8_t A7 = 11;
//static const uint8_t A8 = 8;
//static const uint8_t A9 = 9;
//static const uint8_t A10 = 10;
//static const uint8_t A11 = 11;
//static const uint8_t A12 = 12;
//static const uint8_t A13 = 13;
//static const uint8_t A14 = 14;
//static const uint8_t A15 = 15;

#define Wire xmWireC
#define Wire1 xmWireD
#define Wire2 xmWireE
#define Wire3 xmWireF

#define SPI_PORT	SPIC

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#ifdef ARDUINO_MAIN



// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)

const uint16_t PROGMEM port_to_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
	NOT_A_PORT,
	(uint16_t) &PORTH,
	NOT_A_PORT,
	(uint16_t) &PORTJ,
	(uint16_t) &PORTK,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTQ,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST
	
PA,  //  0   Spesport    J1.3
PA,  //  1   Spesport    J1.9
PA,  //  2   Spesport    J1.10
PA,  //  3   Read Bat V    
PB,  //  4   Can be JTAG    J4.9
PB,  //  5   Can be JTAG    J4.5
PB,  //  6   Can be JTAG    J4.1
PB,  //  7   Can be JTAG    J4.3
PD,  //  8   Rear Bumper    J14.1
PJ,  //  9   Front Bumper    
PF,  //  10   Obstacle detection Left    J2.1
PF,  //  11   Obstacle detection Right    J3.1
PK,  //  12   LED Green D1    
PK,  //  13   LED Red D1    J16.5
PK,  //  14   LED Green D2    
PK,  //  15   LED Red D2    J16.4
PK,  //  16   LED Green D3    
PK,  //  17   LED Red D3    J16.3
PK,  //  18   LED Green D4    
PK,  //  19   LED Red D4    J16.2
PQ,  //  20   LED Green D5    
PQ,  //  21   LED Red D5    J16.1
PD,  //  22   MOTOR1A    
PD,  //  23   MOTOR1B    
PD,  //  24   MOTOR2A    
PD,  //  25   MOTOR2B    
PD,  //  26   MOTOR-Power Save    
PD,  //  27   Sound Shutdown    
PE,  //  28   Sound     
PJ,  //  29   RFID Field Enable    
PJ,  //  30   RFID decode pin    
PE,  //  31   TWI_SDA/Spesport    J1.7
PE,  //  32   TWI_SCL/Spesport    J1.8

};
//const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST
//	REPEAT8(PA), // Header 									8
//	REPEAT8(PB), // Pot and Speaker 						16
//	REPEAT8(PC), // USARTC0 connected to USB on 2&3 		24
//	REPEAT8(PD), // Header  								32
//	REPEAT8(PE), // LEDs 									40
//	REPEAT8(PF), // Switches								48
//	REPEAT8(PH), // Switches								56
//	REPEAT8(PJ), // Switches								64
//	REPEAT8(PK), // Switches								72
//	PQ,PQ,
//};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
_BV(0),  //  0   Spesport  J1.3
_BV(1),  //  1   Spesport  J1.9
_BV(2),  //  2   Spesport  J1.10
_BV(3),  //  3   Read Bat V  
_BV(0),  //  0   MotorStall Current 1A  
_BV(1),  //  1   MotorStall Current 1B  
_BV(2),  //  2   MotorStall Current 2A  
_BV(3),  //  3   MotorStall Current 2B  
_BV(5),  //  8   Rear Bumper  J14.1
_BV(7),  //  9   Front Bumper  
_BV(0),  //  10   Obstacle detection Left  J2.1
_BV(1),  //  11   Obstacle detection Right  J3.1
_BV(0),  //  12   LED Green D1  
_BV(1),  //  13   LED Red D1  J16.5
_BV(2),  //  14   LED Green D2  
_BV(3),  //  15   LED Red D2  J16.4
_BV(4),  //  16   LED Green D3  
_BV(5),  //  17   LED Red D3  J16.3
_BV(6),  //  18   LED Green D4  
_BV(7),  //  19   LED Red D4  J16.2
_BV(0),  //  20   LED Green D5  
_BV(1),  //  21   LED Red D5  J16.1
_BV(0),  //  22   MOTOR1A  
_BV(1),  //  23   MOTOR1B  
_BV(2),  //  24   MOTOR2A  
_BV(3),  //  25   MOTOR2B  
_BV(4),  //  26   MOTOR-Power Save  
_BV(7),  //  27   Sound Shutdown  
_BV(2),  //  28   Sound   
_BV(0),  //  29   RFID Field Enable  
_BV(1),  //  30   RFID decode pin  
_BV(0),  //  31   TWI_SDA/Spesport  J1.7
_BV(1),  //  32   TWI_SCL/Spesport  J1.8


};

//const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
//	BV0TO7, // PORT B
//	BV0TO7, // PORT C
//	BV0TO7, // PORT D
//	BV0TO7, // PORT E
//	BV0TO7, // PORT F
//	BV0TO7, // PORT H
//	BV0TO7, // PORT J
//	BV0TO7, // PORT K
//	_BV(0), _BV(1),// PORT Q
//};


const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
NOT_ON_TIMER,  //  0  Spesport  J1.3
NOT_ON_TIMER,  //  1  Spesport  J1.9
NOT_ON_TIMER,  //  2  Spesport  J1.10
NOT_ON_TIMER,  //  3  Read Bat V  
NOT_ON_TIMER,  //  4  Can be JTAG  J4.9
NOT_ON_TIMER,  //  5  Can be JTAG  J4.5
NOT_ON_TIMER,  //  6  Can be JTAG  J4.1
NOT_ON_TIMER,  //  7  Can be JTAG  J4.3
TIMER_D1B,  //  8  Rear Bumper  J14.1
NOT_ON_TIMER,  //  9  Front Bumper  
TIMER_F0A,  //  10  Obstacle detection Left  J2.1
TIMER_F0B,  //  11  Obstacle detection Right  J3.1
NOT_ON_TIMER,  //  12  LED Green D1  
NOT_ON_TIMER,  //  13  LED Red D1  J16.5
NOT_ON_TIMER,  //  14  LED Green D2  
NOT_ON_TIMER,  //  15  LED Red D2  J16.4
NOT_ON_TIMER,  //  16  LED Green D3  
NOT_ON_TIMER,  //  17  LED Red D3  J16.3
NOT_ON_TIMER,  //  18  LED Green D4  
NOT_ON_TIMER,  //  19  LED Red D4  J16.2
NOT_ON_TIMER,  //  20  LED Green D5  
NOT_ON_TIMER,  //  21  LED Red D5  J16.1
TIMER_D0A,  //  22  MOTOR1A  
TIMER_D0B,  //  23  MOTOR1B  
TIMER_D0C,  //  24  MOTOR2A  
TIMER_D0D,  //  25  MOTOR2B  
NOT_ON_TIMER,  //  26  MOTOR-Power Save  
NOT_ON_TIMER,  //  27  Sound Shutdown  
TIMER_E0C,  //  28  Sound   
NOT_ON_TIMER,  //  29  RFID Field Enable  
NOT_ON_TIMER,  //  30  RFID decode pin  
TIMER_E0A,  //  31  TWI_SDA/Spesport  J1.7
TIMER_E0B,  //  32  TWI_SCL/Spesport  J1.8


};
//const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
//	REPEAT8(NOT_ON_TIMER), // PORT A Header
//	REPEAT8(NOT_ON_TIMER), // PORT B Pot and Speaker
//	REPEAT8(NOT_ON_TIMER), // PORT C USARTC0 to USB on 2&3
//	TIMER_D0A, // PORT D Header
//	TIMER_D0B,
//	TIMER_D0C,
//	TIMER_D0D,
//	TIMER_D1A,
//	TIMER_D1B,
//	NOT_ON_TIMER,
//	NOT_ON_TIMER,
//	TIMER_E0A, // PORT E LEDs
//	TIMER_E0B,
//	TIMER_E0C,
//	TIMER_E0D,
//	TIMER_E1A,
//	TIMER_E1B,
//	NOT_ON_TIMER,
//	NOT_ON_TIMER,
	
//	TIMER_F0A, // TIAN
//	TIMER_F0B,
//	TIMER_F0C,
//	TIMER_F0D,
//	TIMER_F1A,
//	TIMER_F1B,
//	NOT_ON_TIMER,
//	NOT_ON_TIMER,	
	//REPEAT8(NOT_ON_TIMER), // PORT F Switches
	
//	REPEAT8(NOT_ON_TIMER), // PORT H Switches
//	REPEAT8(NOT_ON_TIMER), // PORT J Switches
//	REPEAT8(NOT_ON_TIMER), // PORT k Switches
//};

//TIAN - Add TCF0
const TC0_t* PROGMEM timer_to_tc0_PGM[] = {
	NULL,

	&TCC0,
	&TCC0,
	&TCC0,
	&TCC0,
	NULL,
	NULL,

	&TCD0,
	&TCD0,
	&TCD0,
	&TCD0,
	NULL,
	NULL,

	&TCE0,
	&TCE0,
	&TCE0,
	&TCE0,
	NULL,
	NULL,
//TIAN +	
	&TCF0,
	&TCF0,
	&TCF0,
	&TCF0,
	NULL,
	NULL,
	//TIAN +
	
	};

	//TIAN - Add TCF1
const TC1_t* PROGMEM timer_to_tc1_PGM[] = {
	NULL,

	NULL,
	NULL,
	NULL,
	NULL,
	&TCC1,
	&TCC1,

	NULL,
	NULL,
	NULL,
	NULL,
	&TCD1,
	&TCD1,

	NULL,
	NULL,
	NULL,
	NULL,
	// TCE1 not defined on all xmegas
#if defined(PORTH)
	&TCE1,
	&TCE1,
#else
	NULL,
	NULL,
#endif

//TIAN +
	NULL,
	NULL,
	NULL,
	NULL,
	&TCF1,
	&TCF1,
	//TIAN +
	};

const uint8_t PROGMEM timer_to_channel_PGM[] = {
	NOT_ON_TIMER,

    0,
    1,
    2,
    3,
    0,
    1,

    0,
    1,
    2,
    3,
    0,
    1,

    0,
    1,
    2,
    3,
    0,
    1,
	
//TIAN +
	0,
    1,
    2,
    3,
    0,
    1,
	//TIAN +
};

const uint8_t PROGMEM adc_to_channel_PGM[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,  
    10,
    11,
    12,
    13,
    14,
    15
};


//#include "Driver_RFID.h"


#endif

#endif

/***************************************************************************
Title: PCA9554
by: AD0ND

This file is free software; you can redistribute it and/or modify
it under the terms of either the GNU General Public License version 2
or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
***************************************************************************/

#ifndef PCA9554_h
#define PCA9554_h

#include <Arduino.h>
#include <Wire.h>

//PCA9554 Command Byte
#define INPUTPORT	0x00
#define OUTPUTPORT	0x01
#define POLINVPORT	0x02
#define CONFIGPORT	0x03


#define ALLOUTPUT	0x00
#define ALLINPUT	0xFF

class PCA9554
{
	public :
		PCA9554(byte SlaveAddress);

		bool twiRead(byte &registerAddress);
		bool twiWrite(byte registerAddress, byte dataWrite);

		bool pinMode(byte pinNumber, bool state);
		bool portMode(byte value);
		
		bool digitalWrite(byte pinNumber, bool state);
		bool digitalWritePort(byte value);
				
		bool digitalRead(byte pinNumber);
		bool digitalReadPort(byte value);
		
	private :
		int _SlaveAddress;
};

#endif

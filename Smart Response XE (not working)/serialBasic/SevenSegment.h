/*
||
|| SevenSegment Library 
|| Released into the public domain
|| 
|| @author Nart A.Schinakow, Alexander Brevig
|| @version 1.2
|| @author Paul Richards  update to get to work
||
*/

#ifndef SevenSegment_h
#define SevenSegment_h

#include <Arduino.h>

class ShiftRegister{
	public:
		virtual void buffer(byte out)=0;
		virtual void write()=0;
		virtual void write(byte out)=0;
};

class SevenSegment{

public:
  SevenSegment();
  SevenSegment( ShiftRegister& userShiftRegister );
  SevenSegment( byte userPins[] ); 

  void setPins ( byte userPins[] );  
  void buffer(byte number);
  void print();
  void print(byte number);
  
  void attachShiftRegister( ShiftRegister& userShiftRegister );
  
private:
  void initialize();
  
  ShiftRegister* shiftRegister;
  byte dataArray[10];
  byte pinArray[7];
  byte bufferByte;
};

#endif

/*
|| Changelog:
||
|| 1.3 2012         Paul Richards    : Get it to compile by itself
|| 1.2 2009-04-13 - Alexander Brevig : ready for ShiftRegisters 
|| 1.1 2009-03-16 - Alexander Brevig : uses binary logic to represent states, not ints. [uses 14 x less ram]
||                                     modified API slightly
||                                     modified datatypes to be as small as possible
|| 1.0 2009-03-13 - Nart : InitialRelease
*/

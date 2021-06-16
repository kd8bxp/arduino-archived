#include "SevenSegment.h"

//<<constructor>>
SevenSegment::SevenSegment(){
	byte c[]={2,3,4,5,6,7,8};
	initialize();
	/* 
	for (byte i=0;i<7;i++){
		pinArray[i]=c[i];
		pinMode(pinArray[i],OUTPUT);
	}
	*/
}

//<<constructor>>
SevenSegment::SevenSegment( ShiftRegister& userShiftRegister ){
	shiftRegister = &userShiftRegister;
	initialize();
}

//<<constructor>> parameter userPins defines the pins to use in order to drive the display
SevenSegment::SevenSegment(byte userPins[]){
	initialize();
	for (byte set=0;set<7;set++){
		pinArray[set]=userPins[set];
		pinMode(pinArray[set],OUTPUT);
	}
}

void SevenSegment::setPins (byte userPins[]) {
  for (byte set=0; set<7; set++) {
    pinArray[set] = userPins[set];
    pinMode (pinArray[set], OUTPUT);
  }                      
}

//buffer a number to display
void SevenSegment::buffer(byte number){
	bufferByte = number;
}

//print a number to the display
void SevenSegment::print(){
/*
	if(shiftRegister){
		shiftRegister->write( bufferByte );
	}else{
		for (byte col=0;col<7;col++){
			digitalWrite(pinArray[col],bitRead(bufferByte,col));
		}
	}
	*/
}

//print a number to the display
void SevenSegment::print(byte number){
  int pin;
  int value;
  if(shiftRegister){
	shiftRegister->write( dataArray[number] );
  }else{
	for (byte col=0;col<7;col++){
      pin = pinArray[col];
      // Make sure number if bounded
      value = bitRead(dataArray[number % 10],col);
      digitalWrite (pin,value);  
      //  digitalWrite(pinArray[col],bitRead(dataArray[number],7-col));
	}
  }
}

void SevenSegment::attachShiftRegister( ShiftRegister& userShiftRegister ){
	shiftRegister = &userShiftRegister;
}

//initialize data
void SevenSegment::initialize(){
	bufferByte = 0;
	shiftRegister = 0;
	byte data[10] = {
        0x3F,
		// B011 1111, 0 (6 segments )
		0x06,
		// B000 0110, 1 (2 segments)
		0x5B,
		// B101 1011, 2 (5 segments)
		0x4F,
		// B100 1111, 3 (5 segments)
		0x66,
		// B110 0110, 4 (4 segments)
		0x6D,
		// B110 1101, 5 (5 segments)
		0x7D,
		// B111 1101, 6 (6 segments)
		0x07,
		// B000 0111, 7 (3 segments)
		0x7F,
		// B111 1111, 8 (All 7 segments)
		0x6F
		// B110 1111  9 (6 segments)
	};
	for (byte row=0; row<10; row++){
		dataArray[row]=data[row];
	}
}

/*
|| Changelog:
||
|| 2012         Paul Richards    : cleaned it up 
|| 2009-04-13 - Alexander Brevig : implemented ShiftRegister related logic
|| 2009-03-16 - Alexander Brevig : implemented all changes noted in the .h file
*/

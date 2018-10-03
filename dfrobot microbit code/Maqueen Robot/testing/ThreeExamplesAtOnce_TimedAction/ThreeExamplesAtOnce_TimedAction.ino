/*
||
|| @file ThreeExamplesAtOnce.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | This sketch blinks an LED as [url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url] 
|| |             sets a led on or off according to serial buffer as [url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url] 
|| |             prints the ascii table as [url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url] 
|| #
||
*/

#include <TimedAction.h>

void blink();
void physicalPixel();
void asciiTable();

//this initializes a TimedAction object that will change the state of an LED every second.
TimedAction blinkAction 		=	TimedAction(1000,blink);
//this initializes a TimedAction object that will change the state of an LED 
//according to the serial buffer contents, every 50 milliseconds
TimedAction physicalPixelAction = 	TimedAction(1000,physicalPixel);
//this initializes a TimedAction object that will write tha ascii table to the serial every ten seconds
TimedAction asciiTableAction 	= 	TimedAction(10000,asciiTable); 

//pin / state variables
#define ledPin 12
#define physicalPin 8
boolean ledState = false;
boolean led2State = true;


void setup(){
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,ledState);
  pinMode(physicalPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  blinkAction.check(); //trigger every second
  physicalPixelAction.check(); //trigger every 50 millisecond
  asciiTableAction.check(); //trigger every 10 second
}

//[url=http://arduino.cc/en/Tutorial/Blink]Examples->Digital->Blink[/url] 
void blink(){
  ledState ? ledState=false : ledState=true;
  digitalWrite(ledPin,ledState);
}

//[url=http://arduino.cc/en/Tutorial/PhysicalPixel]Examples->Digital->PhysicalPixel[/url] 
void physicalPixel()
{
  led2State ? led2State=false : led2State=true;
  digitalWrite(physicalPin,led2State);
}

//[url=http://arduino.cc/en/Tutorial/ASCIITable]Examples->Digital->ASCIITable[/url] 
void asciiTable() 
{ 
  byte number = 33; // first visible character '!' is #33 
  // print until we have printed last visible character '~' #126 ... 
  while(number <= 126) { 
	Serial.print((unsigned char)number);    // prints value unaltered, first will be '!' 
	  
	Serial.print(", dec: "); 
	Serial.print(number);          // prints value as string in decimal (base 10) 
	  // Serial.print(number, DEC);  // this also works 
	  
	Serial.print(", hex: "); 
	Serial.print(number, HEX);     // prints value as string in hexadecimal (base 16) 
	  
	Serial.print(", oct: "); 
	Serial.print(number, OCT);     // prints value as string in octal (base 8) 
	  
	Serial.print(", bin: "); 
	Serial.println(number, BIN);   // prints value as string in binary (base 2) 
									 // also prints ending line break 
	number++; // to the next character 
  } 
  asciiTableAction.disable();
}

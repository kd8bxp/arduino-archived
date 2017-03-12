#include <SPI.h>    // include the SPI communcation library
// Connect pin#13 (SCLK) To S2 on AP23 
// Connect pin#11 (DATOUT) To S3 on AP23 
// Connect pin#13 (SCLK) To S2 on AP23 
// Connect pin#10 (CS) To S1 on AP23
// GPIO 9 = STOP
// DO from Ap23 optional S4 - Not used here
// ** Add stop bit so that MCU knows then  the chip has stopped talking.  OUT1 is what you'll want to use
#define STOP 9      // The STOP wire is the busy signal of the 

int cs=10;          // this is the chip-select pin for SPI
int del=200;        // short 200ms delay
int value1 = 0x98;  // play command - This value never changes
int value2 = 0;  // voice address - when you place a hex value in this integer and call the "readout()" function, that specific word will play
int value3 = 0xA8;  // COUT ramp up - This value never changes
int value4 = 0xB8;   // COUT ramp down - This value never changes

// ALL OF THE ABOVE CODE IS REQUIRED FOR THE OPERATION OF THIS SHIELD


void setup()  // Set up SPI
{
  pinMode(STOP,INPUT);     // Set the "STOP" GPIO as an input.  This is the busy signal, and is high when the shield is busy playing a word
  SPI.begin();             // Initialize SPI
  SPI.setClockDivider(SPI_CLOCK_DIV32); // low frequency SPI
  pinMode(cs,OUTPUT);    // Chip select pins is an output
  digitalWrite(cs,HIGH); // Set chip select to be HIGH (5v) by default.  The chip on the shield is selected when this line is brought low. 
  SPI.setBitOrder(MSBFIRST);  // OTP requires MSB first
  SPI.setDataMode(SPI_MODE0);  // Use MODE0, as all other modes to not work
  value2 = 0x00; // Start at voice group 0
  delay(200);
  // ramp up
  digitalWrite(cs,LOW);
  SPI.transfer(value3);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(7);
  delay(1000);   // One second delay
}



void loop()
// THIS SENTENCE WILL LOOP OVER AND OVER.  THERE ARE DIFFERENT DELAYS IN BETWEEN EACH WORD.  IF YOU WANT DELAYS IN BETWEEN EACH WORD, YOU CAN MODIFY AS YOU SEE KIT
// THE BELOW SENTENCE IS "THE TIME IS TWELVE FORTY PM" = {0xf3, 0x33, 0xcd, 0x42, 0x4c, 0x2d}
{
  value2 = 0xf3;  // Place the value in the lookup table for "the" (0xf3) into "value2" integer.
  readout();      // Call the "radout()' function to play the word 
  delay(100);     // delay of 100ms between this word and the next  
  value2 = 0x33;
  readout();     
  delay(200);     // delay of 200ms between this word and the next  
  value2 = 0xcd;
  readout();     
  delay(250);     // delay of 250ms between this word and the next  
  value2 = 0x42;
  readout();     
  delay(300);     // delay of 300ms between this word and the next  
  value2 = 0x4c;
  readout(); 
  delay(350);     // delay of 350ms between this word and the next  
  value2 = 0x2d; 
  readout(); 
  delay(5000); // Final delay of 5 seconds before repeating the sentence
}


void readout()
{
  // Transmit Data
  digitalWrite(cs,LOW);
  SPI.transfer(value1);
  SPI.transfer(value2);
  digitalWrite(cs,HIGH);
  delay(5);
  while (digitalRead(STOP) == HIGH) { 
       {}
    } 
    
  //delay(5);
  // ramp down
  digitalWrite(cs,LOW);
  SPI.transfer(value4);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(10);
  // YOU REALLY NEED TO ADD A STOP INPUT HERE, OR ELSE YOU'RE REALLY WASTING POWER!

}



   

   




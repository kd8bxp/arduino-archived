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

// THE BELOW THREE INTEGERS ARE SENTENCES.  These arrays have all the word values for each sentence from left to right in order.
int sentence[] = {0xf3, 0x33, 0xcd, 0x42, 0x4c, 0x2d}; // The time is 12:40 pm - Breaks down to "the (0x86)", "time (0x61)", "is (0x83)"...ETC
int sentenceb[] = {0xf3, 0x2e, 0xcd, 0x29, 0x1b, 0x41, 0x38, 0x53, 0x46}; // The date is friday march 11/2016 || (two, thousand, sixeeen) = Three words
int sentencec[] = {0x91, 0x90, 0x0a, 0x92, 0x8e}; // Intruder detected.  Enable security alarm

void setup()  // Set up SPI & Inputs/Outputs
{
  pinMode(STOP,INPUT);     // Set the "STOP" GPIO as an input.  This is the busy signal, and is high when the shield is busy playing a word
  SPI.begin();             // Initialize SPI
  SPI.setClockDivider(SPI_CLOCK_DIV32); // low frequency SPI
  pinMode(cs,OUTPUT);    // Chip select pins is an output
  digitalWrite(cs,HIGH); // Set chip select to be HIGH (5v) by default.  The chip on the shield is selected when this line is brought low. 
  SPI.setBitOrder(MSBFIRST);  // OTP requires MSB first
  SPI.setDataMode(SPI_MODE0);  // Use MODE0, as all other modes to not work
  value2 = 0x00; // Start at voice group 0
  delay(1000);   // One second delay
}

void loop()  // This look will call all three sentences over and over
{
  sentence1();  // Use this command to call sentence1 (The time is 12:40pm)
  delay(5000);  // Wait five seconds for next sentence
  sentence2();  // Use this command to call sentence2 (The date is Friday, March 11/2016)
  delay(5000);  // Wait five seconds for next sentence
  sentence3();  // Use this command to call sentence3 (Intruder detected.  Enable Security Alarm)
  delay(5000);
}
 

void sentence1()    
{
  // 6 words - The time is 12:40 pm
  int i;
  for (i = 0 ; i < 6 ; i++ )
  {
    value2 = sentence[i];
    readout();
  }
}

void sentence2()
{
  // 9 words
  int i;
  for (i = 0 ; i < 9 ; i++ )
  {
    value2 = sentenceb[i];
    readout();
  }
}

void sentence3()
{
  // 5 words
  int i;
  for (i = 0 ; i < 5 ; i++ )
  {
    value2 = sentencec[i];
    readout();
  }
}
    

void readout()    // Calling this function reads words individually
{
  // ramp up
  digitalWrite(cs,LOW);
  SPI.transfer(value3);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(7);
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



   

   




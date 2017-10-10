// THIS CODE SAMPLE MAKES THE SHIELD SAY ALL OF THE PROGRAMMED WORDS IN ORDER IN A LOOP.  THE POINT OF THIS CODE IS SIMPLY TO SHOW YOU HOW THE CODE IS WORKING

#include <SPI.h>    // include the SPI communcation library
// Connected: pin#13 (SCLK) To S2 on AP23 
// Connected: pin#11 (DATOUT) To S3 on AP23 
// Connected: pin#13 (SCLK) To S2 on AP23 
// Connected: pin#10 (CS) To S1 on AP23
// GPIO 9 = STOP
#define STOP 9      // STOP = the busy signal coming from the audio chip

int cs=10;          // this is the chip-select pin for SPI
int del=200;        // short 200ms delay
int value1 = 0x98;  // play command - This value never changes
int value2 = 0;     // voice address - when you place a hex value in this integer and call the "readout()" function, that specific word will play
int value3 = 0xA8;  // COUT ramp up - This value never changes
int value4 = 0xB8;  // COUT ramp down - This value never changes

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
  delay(1000);   // One second delay
}



void loop()
{
  // THIS loop will increment and play all words in the shield library one by one 
  readout();     // SPI command transfer function.  When this function is called, the word associated with the value in integer "value2" is played
  delay(20);   // Wait once second after the message has stopped playing
  value2 = value2 + 1;  // Increment voice command group.    
}

void readout() // Calling this function plays the word associated with the hex number placed in the "value2" integer
{
  // This command ramps up the audio output
  digitalWrite(cs,LOW);
  SPI.transfer(value3);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(7);
  // This command tells the shield to play the word
  digitalWrite(cs,LOW);
  SPI.transfer(value1);  // value#1 is declared at the beginning of this code, and never changes
  SPI.transfer(value2);  // Send the address to the relative word that you've programmed into integer 'value2"
  digitalWrite(cs,HIGH);
  delay(5);
  while (digitalRead(STOP) == HIGH) {   // The code doesn't progress until the busy signal goes low.  While busy, do nothing.
       {}
    } 
  // This command ramps down the audio output once the word has been said 
  digitalWrite(cs,LOW);
  SPI.transfer(value4);
  SPI.transfer(0x00);
  digitalWrite(cs,HIGH);
  delay(10);
}



   

   




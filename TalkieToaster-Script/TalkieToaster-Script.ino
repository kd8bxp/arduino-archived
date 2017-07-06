/*
Original project - R2D2 Droid Security Assistant DSA
December 2015 
This code section converted for Talkie Toaster
November 2016
Slider2732


Nano/Pro Mini - Arduino Pins
SD Card 
13 sck
12 miso
11 mosi
10 cs

4 10mm LED, to show PIR state changes if wished

7 PIR input

9 Speaker output, to amplifier
2x white LED in pallelel from Pin 9 to Ground

*/

// SD library
#include <SD.h>

// SD Pin 
#define SD_CARD 10  

// Audio library
#include <TMRpcm.h>         

// Create an audio object
TMRpcm sample;   

// PIR sensor
int calibrationTime = 10;      
long unsigned int lowIn;         
//milliseconds low before we assume all motion has stopped
long unsigned int pause = 1000;  
boolean lockLow = true;
boolean takeLowTime;  
int pirPin = 7;    // PIR sensor input
int ledPin = 4;    // PIR sensor trigger LED

int ask = 0;       // for the samples to play through the TV show script once


void setup()
{
  // randomiser for speech output, using an unused pin (!)
  randomSeed(analogRead(2));
  
  // speaker pin output 
  sample.speakerPin = 9; 
  
  Serial.begin(9600);
  
  // see if the card is inserted and found
  if (!SD.begin(SD_CARD)) 
  {  
 return;   // It's not there, don't do anything
  }
 Serial.println("SD card started");
 
 // quite loud
 sample.volume(9);
 sample.quality(1);
 
 // play the start up sound
 sample.play("Howdy2.wav"); 
 delay(3000); // enough time to play the sample
 Serial.println("Played Howdy Doodly Do");
 
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("Calibrating sensor ");
  sample.play("BComp.wav");

  delay(5000);
  
  sample.play("Game.wav");
  delay(3000); 
  sample.play("ALAnyT.wav");
  for(int i = 0; i < calibrationTime; i++){
  Serial.print(".");
  delay(1000);
   }
  Serial.println(" Done");
  Serial.println("PIR SENSOR ACTIVE");
}

 
void loop()
{  
   
  // PIR detect and response routine, to follow the script once

     if(digitalRead(pirPin) == LOW){
       digitalWrite(ledPin, LOW);  }

  
     if(digitalRead(pirPin) == HIGH)
     {
      if (ask == 0)
     {
     digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
     delay(1000);
        
        sample.play("Muffin.wav"); 
        delay(18000);
        sample.play("ASYAWM.wav");
        delay(12000); 
        sample.play("Question.wav");
        delay(7000);
        sample.play("ALAnyT.wav"); 
        delay(4000);
        sample.play("Mind.wav"); 
        delay(6000);
        sample.play("ITTIAm.wav");
        delay(6000);
        sample.play("Repair.wav"); 
        delay(10000);
        ask = 1;
        }
        }

        // after script, ask about toast each time PIR triggered, forever.....
        if(digitalRead(pirPin) == HIGH){
        if (ask == 1)
        {
        digitalWrite(ledPin, HIGH);
        sample.play("ALAnyT.wav"); 
        delay(6000);
        respond;
        }
        }
}



// --------------------------
// Alternative implementation
// picks random sound to play - alter PIR code above
void respond()
{
 	// pick a random number + allow for gaps so less repetitive
 	int randNumber = random(20);
 
 // play the randomly picked sound	
 switch (randNumber) {
    case 1:
      sample.play("ITTIAm.wav");
      break;
    case 2:
      delay(5000);
      sample.play("Muffin.wav");
      break;   
	  case 3:   
     delay(5000);
	    sample.play("ASYAWM.wav");
      break;
	  case 4:
	    sample.play("Game.wav");
      break;
    case 5:
      sample.play("Repair.wav");
      break;
    case 6:
      sample.play("Mind.wav");
      break;
    case 7:
      delay(6000);
      sample.play("Question.wav");
      delay(7000);
      sample.play("ALAnyT.wav");
      break;
   case 8:
      delay(3000);
      sample.play("Muffin.wav");
      break;   
  case 9:
      sample.play("Question.wav");
      delay(7000);
      sample.play("ALAnyT.wav");
      break;
 case 10:
      delay(5000);
      sample.play("Muffin.wav");
      break;   
 case 11:
      delay(5000);
      sample.play("Muffin.wav");
      break;   
      
      
  }
 	


  
}

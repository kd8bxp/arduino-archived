#include <xTimer.h>

#include <dma_driver.h>
#include <Robonii_Sound.h>

/*
 This example uses the Robonii sound Library to generate 18 tunes using the DMA channel and xTimer. 
Please note that it is essential to include XTimer.h and dma_driver.h in the sketch-file, as well as Robonii_Sound.h

 */
  

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  tone(28,2000,1000);
  
  delay(1500);              
  Robonii_Sounds.InitSound();  
  
  //Sounds to play : 1-20
  Robonii_Sounds.PlaySound(2,3);  
  delay(1500);  
  Robonii_Sounds.PlaySound(3,3);
  
}

//Sounds to play : 1-20
 char Soundcounter=4;
 
void loop() {
  digitalWrite(13, HIGH);   // set the LED off
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED on
  delay(1000);              // wait for a second
  Robonii_Sounds.PlaySound(Soundcounter,3);
  Soundcounter++;
}






/*
  Robonii Sound.
  This example toggles the Robonii-Speaker pin as a normal IO to generate a square-wave to the sound amplifier.
  The timing is first done using an interupt, and then using the none-critical delay function.
  The amplifier is connected to the Robonii using a 'band-pass' filter.
  The DAC can be used to generate a clean sine wave and to do volume control.

 */

void setup() {                
  // initialize the digital pin as an output.
  pinMode(Sound_Pin, OUTPUT);     
  pinMode(Sound_Shutdown_Pin, OUTPUT);     
  
    delay(100); 
  digitalWrite(Sound_Shutdown_Pin, LOW);    // set Sound off
  delay(100); 
  digitalWrite(Sound_Shutdown_Pin, HIGH);    // set Sound On
  tone(2000,900);
  delay(1000); 
}



//Generate 3 tones, and then restarts the cycle.
unsigned int count = 0;
void loop() {
  const int factor =5;
  const int base= 500;
  
  count++;
  if (count < base)
  {
     Hertz100();
  }else if (count < base*factor)
  {
    Hertz500();
  }else if (count < base*factor*5)
  {
    Hertz5000()  ;  
  }
}



void Hertz5000()
{
   digitalWrite(Sound_Pin, HIGH);   
  delayMicroseconds(100);              
  digitalWrite(Sound_Pin, LOW);    
  delayMicroseconds(100);              
}

void Hertz100()
{
   digitalWrite(Sound_Pin, HIGH);   
  delay(5);              
  digitalWrite(Sound_Pin, LOW);    
  delay(5);               
}

void Hertz500()
{
   digitalWrite(Sound_Pin, HIGH);   
  delay(1);              
  digitalWrite(Sound_Pin, LOW);    
  delay(1);              
}

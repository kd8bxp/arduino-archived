//Program by Jeremy Blum
//www.jeremyblum.com
//Reads Temp in Celcius and Shows it on Computer

//Include Wire I2C Library
#include  <Wire.h>
int temp_address = 72;
  float myFloat =  4.444;
void setup()
{
  //Create Serial Object (9600 Baud)
  Serial.begin(9600);

  
  //Create a Wire Object
//  Wire.begin();
}

void loop()
{
  //Send Request
  //Wire.beginTransmission(temp_address); //Start talking
 //Wire.write(0); //Ask for Register zero
 //Wire.endTransmission(); //Complete Transmission
 //Wire.requestFrom(temp_address, 1); //Request 1 Byte
  //whilf(Wire.available() == 0); //wait for response
  //int c = Wire.read(); // Get the temp
  int c = 33;
  
  //do some math...

  char buffer[9];
  myFloat = myFloat + .00001;  
  String s = dtostrf(myFloat, 1, 5, buffer);
  int f = round(c*9.0/5.0 +32.0);
   Serial.print("bbbb>>,");
  Serial.print(buffer);
  Serial.print("C,");
  Serial.print(f);
  Serial.println("F.");
  
  delay(500);
  
  
}

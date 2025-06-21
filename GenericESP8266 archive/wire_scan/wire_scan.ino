#include <Wire.h>

void setup() 
{
  Serial.begin(115200);
    
  Wire.begin(D2,D1);

  delay(3000);
  
}

void loop() 
{
  int address;
  int error;
  
  Serial.println("scanning Address [HEX]");
  
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if(error==0)
    {
      Serial.print(address,HEX); Serial.print(" ");
    }
    else Serial.print(".");

    delay(10);
  }

    
}


#include <Wire.h>     // Load the Wire Library


void setup()
{
  
  Serial.begin(115200);
  Wire.begin();
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(27, INPUT);
  pinMode(26, INPUT);
  pinMode(25, INPUT);
}



void loop()
{
   Serial.print("Port 1 " ); Serial.println(digitalRead(13));
   Serial.print("Port 2 " ); Serial.println(digitalRead(12));
   Serial.print("Port 3 " ); Serial.println(digitalRead(14));
   Serial.print("Port 4 " ); Serial.println(digitalRead(27));
   Serial.print("Port 5 " ); Serial.println(digitalRead(26));
   Serial.print("Port 6 " ); Serial.println(digitalRead(25));
   delay(500);
}

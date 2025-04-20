
#include "Arduino.h"
#include "RoverC.h"

// the setup routine runs once when M5StickC starts up
void setup(){ 
  Serial.begin(115200);

  Serial.println("Start.....");
  RoverC_Init();
  Move_stop(100);
delay(5000);
Serial.println("Go.....");
 Move_turnleft(30);
      delay(2000);
      Move_left(30);
      delay(500);
      Move_turnright(30);
      delay(2000);
      Move_right(30);
      delay(500);
    Move_stop(100);
Serial.println("End....");
    while(1);
}

// the loop routine runs over and over again forever
void loop() {
 
}

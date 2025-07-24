#include "src/M5Atom/M5Atom.h"
#include "AtomMotion.h"

AtomMotion Atom;
int direction = 0; // 0 = stop, 1 = forward, 2 = reverse, 3 = left, 4 = right



//servo angle range 0 ~ 180
//DC motor speed range -127~127

void TaskMotion(void *pvParameters){
  while (1)
  {
    
    
        
    vTaskDelay( 100 / portTICK_RATE_MS );
    if(direction == 0){
      //stop
      Atom.SetMotorSpeed(1, 0);
      Atom.SetMotorSpeed(2, 0);
      M5.dis.drawpix(0, 0x00FF00); //red
    }
     else if (direction == 1) {
      //forward
      Atom.SetMotorSpeed(1,100);
      Atom.SetMotorSpeed(2,100);
      M5.dis.drawpix(0, 0xff0000); //green
    }
    else if (direction == 2) { 
      //reverse
      Atom.SetMotorSpeed(1,-65);
      Atom.SetMotorSpeed(2,-65);
      M5.dis.drawpix(0, 0x0000ff); //blue
    }
    else if (direction == 3) {
      //left
      Atom.SetMotorSpeed(1, -60);
      Atom.SetMotorSpeed(2, 50);
      M5.dis.drawpix(0, 0x00eeee);
    }
    else if (direction == 4) {
      //right
      Atom.SetMotorSpeed(1, 50);
      Atom.SetMotorSpeed(2, -50);
      M5.dis.drawpix(0, 0x00eeee);
    }
  }
  //vTaskDelay(1000/ portTICK_RATE_MS);
}


void setup(){ 
Serial.begin(115200);
  
  M5.begin(true, true, true);
  Atom.Init();     //sda  25     scl  21 
  
  xTaskCreatePinnedToCore(
    TaskMotion
    ,  "TaskMotion"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);

 direction = 0;
 delay(3000);
}

void loop() {
    M5.update();
    direction = 1;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 2;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 3;
    delay(3000);
    direction = 0;
    delay(3000);
    direction = 4;
    delay(3000);
    direction = 0;
    delay(1000);
    while(1);
}

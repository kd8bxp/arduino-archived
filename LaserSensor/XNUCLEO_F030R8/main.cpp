#include "mbed.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial pc(SERIAL_TX, SERIAL_RX);

DigitalIn LaserSensor(D2);

DigitalOut myled(LED1);
 
int main() {
  pc.printf("\r\nLaser Sensor Exampel !\r\n");
  while(1) { 
      if(LaserSensor == 1)
      {
        pc.printf("NO Obstacles!\r\n");
        myled = 0;
      }
      else
      {
          pc.printf("Obstacles!\r\n");
          myled = 1;
      }
      wait(0.5);
  }
}
 
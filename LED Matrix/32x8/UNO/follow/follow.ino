#include "LedControl.h" //  need the library
LedControl lc=LedControl(D7,D5,D8,4); // 

// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

void setup()
{
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  //lc.shutdown(0,false);// turn off power saving, enables display
  //lc.setIntensity(0,8);// sets brightness (0~15 possible values)
 // lc.clearDisplay(0);// clear screen

  //lc.shutdown(1,false);// turn off power saving, enables display
  //lc.setIntensity(1,8);// sets brightness (0~15 possible values)
  //lc.clearDisplay(1);// clear screen
  
}

void loop()
{
  int devices=lc.getDeviceCount();
  for(int address=devices-1;address>=0;address--) {
  for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
       
      lc.setLed(address,col,row,true); // turns on LED at col, row
      //lc.setLed(address,col,row,false); // turns on LED at col, row
      delay(10);
    }
  }
  }

for(int address=devices-1;address>=0;address--) {
  for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
       
      lc.setLed(address,col,row,false); // turns off LED at col, row
      //lc.setLed(address,col,row,true); // turns on LED at col, row      
      delay(10);
    }
  }}
}

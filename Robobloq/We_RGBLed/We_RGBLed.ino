#include "WeELF328P.h"
WeRGBLed led(2);

int16_t bri = 0, st = 0;
void setup() 
{
}
void loop()
{
  color_loop();
}

void color_loop()   //White Breath Light
{
   if(bri >= 40)     //Brightness:0-255
  {
    st = 1;
  }
  if(bri <= 0)
  {
    st = 0;
  }
  if(st)
  {
    bri--;
  }
  else
  {
    bri++;
  }
  
   led.setColorAt(0, bri, bri, bri);  //led, red, green, blue
   led.show();
   delay(50);   
}

#include "neopixel.h"


void setup()
{
  
  Neopixel np(8, 40);
  np.clear();

  
  // returns 1 if index (here 9) was out of bounds
  np.showColor(9, 255, 0, 159);
  np.show();
delay(1000);
  
  np.clear();

  }

  void loop()
  {
    
  }


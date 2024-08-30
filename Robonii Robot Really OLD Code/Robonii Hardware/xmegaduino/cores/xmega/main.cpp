#include <Arduino.h>

int main(void)
{

 
	init();
#ifdef Robonii_h
Serial.begin(115200);
#endif
	setup();
    
	for (;;)
		loop();
        
	return 0;
}


#include <NXP/iolpc2103.h>
#include "types.h"

/** set a pin as either HIGH or LOW */
void digitalWrite(int pin, byte value)
{
  if (pin >= 0 && pin < 32) {
    if (value == HIGH) {
      IOSET = (1<<pin);
    } else if (value == LOW) {
      IOCLR = (1<<pin);
    }
  }  
}

/** read a pin as either HIGH or LOW */
byte digitalRead(int pin)
{
  byte value;
  if (pin >= 0 && pin < 32) {
    value = IOPIN & (1<<pin);
  }  
  return (value == 0x0) ? LOW : HIGH;
}
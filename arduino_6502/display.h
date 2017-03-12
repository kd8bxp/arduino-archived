// 5x7 matrix driver

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __DISPLAY__
  #define __DISPLAY__

  #include <stdint.h>
  #include <avr/io.h>
  #include "font.h"

  #define NUMCOLUMNS 7
  #define NUMROWS 5

  void initDisplay();
  void setRowPattern(uint8_t bitPattern);
  void setCol(uint8_t col);
  void setRow(uint8_t row);
  void showMatrix(uint16_t n);
  void setPixel(uint8_t x, uint8_t y, uint8_t intensity);
  void printCode(uint16_t c);
  void hex1(uint8_t x);
  void _putchar(int8_t c);
  void showLeds(uint8_t k);
  void ledOn();
  void ledOff();

void print2Code(uint16_t c, uint16_t c2);
void _put2char(int8_t c, int8_t c2);
  
  extern uint8_t DisplayMem[NUMCOLUMNS];

#endif // __DISPLAY__

#ifdef __cplusplus
}
#endif

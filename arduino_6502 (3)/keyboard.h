// 3x4 keyboard driver

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __KEYBOARD__
  #define __KEYBOARD__

  #include <stdint.h>
  #include <avr/io.h>
  //#include "display.h"

/*
 * 	KEYBOARDTYPE1:
	keyboard pin layout for mulicomp keypad ( used in v0.1 )

	key line driver active high PD3, PD4, PD5, PD6
	key line inputs PB0, PB2, PB3,

	keys 1 ,2 ,3 : PD3, row 0
	Keys 4 ,5 ,6 : PD4, row 1
	keys 7 ,8 ,9 : PD5, row 2
	keys * ,0 ,# : PD6, row 3
	PB3 PB2 PB0

	KEYBOARDTYPE2:
	keyboard pin layout for for S512J-BF7KUV

	keys 1 ,2 ,3 : PD6, row 0, display cathode
	Keys 4 ,5 ,6 : PB0, row 1, display anode
	keys 7 ,8 ,9 : PD5, row 2, display cathode
	keys * ,0 ,# : PD4, row 3, display cathode
	PB2 PD2 PB3
	A C A
 */
  #define KEYBOARDTYPE2

  #define NOKEY 0xFF
  #define STARKEY 0XFE
  #define HASHKEY 0xFD

  uint8_t scanKey();
  uint8_t scanKey2();
  uint8_t scanKey1();
  uint8_t getKey();
  uint8_t keyPressed();
  uint8_t getKeyCode();
  int8_t _getchar();
uint8_t getChar();

#endif // __KEYBOARD__

#ifdef __cplusplus
}
#endif

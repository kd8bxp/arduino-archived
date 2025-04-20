#ifndef _WS_Flow_H_
#define _WS_Flow_H_

#include "src/Adafruit_GFX/Adafruit_GFX.h"
#include "src/Adafruit_NeoMatrix/Adafruit_NeoMatrix.h"
#include "src/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

#define RGB_Control_PIN   32 //14  

int getCharWidth(char c);
int getStringWidth(const char* str);
void Text_Flow(char* Text);
void Matrix_Init();   
#endif

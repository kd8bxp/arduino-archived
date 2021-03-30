/*
  HelloWorld.ino
  
  "Hello World" version for U8x8 API
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/

#include <Arduino.h>
#include "src/U8g2/U8x8lib.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
//#include "SmartResponseXE.h"

const static uint8_t LCD_DC = 24;
const static uint8_t LCD_CS = 7;
const static uint8_t LCD_RST = 18;

//U8X8_ST7565_EA_DOGM128_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_EA_DOGM128_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_64128N_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_64128N_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_EA_DOGM132_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ U8X8_PIN_NONE);	// DOGM132 Shield
//U8X8_ST7565_EA_DOGM132_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ U8X8_PIN_NONE);	// DOGM132 Shield
//U8X8_ST7565_ZOLEN_128X64_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_ZOLEN_128X64_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_LM6059_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);	// Adafruit ST7565 GLCD
//U8X8_ST7565_LM6059_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);		// Adafruit ST7565 GLCD
//U8X8_ST7565_KS0713_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);	// KS0713 controller
//U8X8_ST7565_KS0713_4W_HW_SPI u8x8(/* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// KS0713 controller
//U8X8_ST7565_LX12864_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);	
//U8X8_ST7565_LX12864_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);		
//U8X8_ST7565_ERC12864_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_ERC12864_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_ERC12864_ALT_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18); // contrast improved version for ERC12864
//U8X8_ST7565_ERC12864_ALT_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);	// contrast improved version for ERC12864
//U8X8_ST7565_NHD_C12832_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_NHD_C12832_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_NHD_C12864_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8X8_ST7565_NHD_C12864_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);
//U8X8_ST7565_JLX12864_4W_SW_SPI u8x8(/* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
U8X8_ST7565_JLX12864_4W_HW_SPI u8x8(/* cs=*/ 7, /* dc=*/ 24, /* reset=*/ 18);


// End of constructor list


void setup(void)
{
  //SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  /* U8g2 Project: SSD1306 Test Board */
  //pinMode(10, OUTPUT);
  //pinMode(9, OUTPUT);
  //digitalWrite(10, 0);
  //digitalWrite(9, 0);		
  
  /* U8g2 Project: KS0108 Test Board */
  //pinMode(16, OUTPUT);
  //digitalWrite(16, 0);	
  
  u8x8.begin();
  u8x8.setPowerSave(0);
  //  SRXEWriteString(0,30,"Hello World", FONT_MEDIUM, 3, 0);
  
}

void loop(void)
{
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,1,"Hello World!");
  //u8x8.refreshDisplay();		// only required for SSD1606/7  
  delay(2000);
}

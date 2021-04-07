#include "SmartResponseXE.h"
#include "fonts/fonts.h" //These are the Original fonts that were included with the library (Removed from the library, renamed, and included here.) Two fonts are included here: font name: normal size: normal/large, font name: small size: small/medium

/*
 * Alternative fonts are listed below. These fonts were found at https://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
 * These were converted for use with the Smart Response. 
 * There is a fontgen.py script that was provided from the above site, unfortuntally I've been unable to get it to work correctly, but someone 
 * may have better luck then me.
 * 
 * Large Fonts are 3 sets of 3 pixels (The fonts provided are 8 bit fonts, not 9 but appear to work more or less)
 * Small Fonts are 2 sets of 3 pixels 6 bit. 
 * 
 * Some of the fonts work better then others, variable spaced fonts are currently a problem. 
 * Monospaced seem to work best.
 *  Try the fonts and see what works best for you. 
 *
 * Font and library modifications by LeRoy Miller, kd8bxp Mar/Apr 2021.
 * 
 * 
 * SRXEWriteString was slightly modified:
 * 
 * SRXEWriteString(int x, int y, char *szMsg,int iSize, const unsigned char ucFont[], int iFGColor, int iBGColor);
 *                                                       ^^^^^^^^^^^^^^^^^^^^^ This is the font name listed below (see example below)
 *                                                       
 * Use the fonts for the correct size, otherwise you will see garbage characters.                                                       
 * 
 *  */

#include "fonts/5x5.h" //font name: fivexfive size: small/medium
//#include "fonts/acme_5_outlines.h" //font name: acme_5_outlines size: small/medium 
//#include "fonts/aztech.h" //font name: aztech size: small/medium
//#include "fonts/Blokus.h" //font name: blokus size: small/medium
//#include "fonts/BMplain.h" //font name: BMplain size: small/medium
//#include "fonts/BMSPA.h" //font name: bmspa size: normal/large
//#include "fonts/Commo-Monospaced.h" //font name: commoMonospaced size: normal/large
//#include "fonts/crackers.h" //font name: crackers size: small/medium
#include "fonts/formplex12.h" //font name: formplex12 size: normal/large
//#include "fonts/haiku.h" //font name: haiku size: small/medium
//#include "fonts/HISKYF21.h" //font name: hiskyf21 size: small/medium
//#include "fonts/HUNTER.h" //font name: hunter size: normal/large
//#include "fonts/m38.h" //font name: m38 size: normal/large
//#include "fonts/Minimum.h" //font name: minimum size: small/medium
//#include "fonts/Minimum+1.h" //font name: minimum1 size: normal/large
//#include "fonts/sloth.h" //font name: sloth size: small/medium
//#include "fonts/SUPERDIG.h" //font name: superdig size: small/medium
//#include "fonts/zxpix.h" //font name: zxpix size: small/medium

void setup() {
  
  SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  SRXEWriteString(0, 0,"Hello World",FONT_LARGE, normal, 3, 0);
  SRXEWriteString(0,30,"Hello World",FONT_MEDIUM, small, 3, 0);
  SRXEWriteString(0,60,"Hello World",FONT_NORMAL, normal, 3, 0);
  SRXEWriteString(0,96,"Hello World",FONT_SMALL, small, 3, 0);

delay(5 * 1000);

  //SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  //mydigitalWrite(0xa2, HIGH);
  //delay(50);
  //mydigitalWrite(0xa2, LOW);
  //delay(5);
  //mydigitalWrite(0xa2, HIGH);
  //delay(150);
  SRXEFill(0); //clear screen
  
  SRXEWriteString(0, 0,"Hello World",FONT_LARGE, formplex12, 3, 0);
  SRXEWriteString(0,30,"Hello World",FONT_MEDIUM, fivexfive, 3, 0);
  SRXEWriteString(0,60,"Hello World",FONT_NORMAL, formplex12, 3, 0);
  SRXEWriteString(0,96,"Hello World",FONT_SMALL, fivexfive, 3, 0);
delay(1000);
SRXEScroll(90); //causes lines on screen that don't go away

 delay(5 * 1000);
 SRXEScrollReset();
 SRXEFill(0);
 SRXEWriteString(0, 30,"Hello World",FONT_LARGE, formplex12, 3, 0);
  SRXEWriteString(0,0,"Hello World",FONT_MEDIUM, fivexfive, 3, 0);
  SRXEWriteString(0,90,"Hello World",FONT_NORMAL, formplex12, 3, 0);
  SRXEWriteString(0,60,"Hello World",FONT_SMALL, fivexfive, 3, 0);
 
}

void loop() {
 
}

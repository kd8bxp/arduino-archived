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

//#include "fonts/5x5.h" //font name: fivexfive size: small/medium monospaced
//#include "fonts/acme_5_outlines.h" //font name: acme_5_outlines size: small/medium monospaced
//#include "fonts/aztech.h" //font name: aztech size: small/medium monospaced upper and lower case
//#include "fonts/Blokus.h" //font name: blokus size: small/medium variable spaced, upper and lower case
//#include "fonts/BMplain.h" //font name: BMplain size: small/medium monospaced upper and lower case
//#include "fonts/crackers.h" //font name: crackers size: small/medium variable spaced (part lower case, mostly upper case) (IMHO does not look good)
//#include "fonts/haiku.h" //font name: haiku size: small/medium monospaced upper and lower case, missing most of the symbols
//#include "fonts/HISKYF21.h" //font name: hiskyf21 size: small/medium variable spaced upper case, missing some symbols
//#include "fonts/Minimum.h" //font name: minimum size: small/medium mostly monospaced (M seems to a problem), upper case
//#include "fonts/sloth.h" //font name: sloth size: small/medium variable spaced (IMHO does not look good probably would look out spaced correctly)
#include "fonts/SUPERDIG.h" //font name: superdig size: small/medium variable spaced upper case missing some symbols
//#include "fonts/zxpix.h" //font name: zxpix size: small/medium

//Large/Normal Fonts
//#include "fonts/BMSPA.h" //font name: bmspa size: normal/large
//#include "fonts/Commo-Monospaced.h" //font name: commoMonospaced size: normal/large
//#include "fonts/formplex12.h" //font name: formplex12 size: normal/large
//#include "fonts/HUNTER.h" //font name: hunter size: normal/large
//#include "fonts/m38.h" //font name: m38 size: normal/large
//#include "fonts/Minimum+1.h" //font name: minimum1 size: normal/large

void setup() {
  
  SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  SRXEWriteString(0, 0,"SuperDig Font (Small/Medium)",FONT_LARGE, normal, 3, 0);
 
  SRXEWriteString(0,20,"ABCDEFGHIJKLMNOPQRSTUV",FONT_MEDIUM, superdig, 3, 0);
  SRXEWriteString(0,35,"abcdefghijklmnopqrstuv",FONT_MEDIUM, superdig, 3, 0);
  SRXEWriteString(0,50,"1234567890 !@#$%^&*()",FONT_MEDIUM, superdig, 3, 0);
  SRXEWriteString(0,65,"-_=+{}[]'\|? / `~ \"", FONT_MEDIUM, superdig, 3, 0);


 
}

void loop() {
 
}

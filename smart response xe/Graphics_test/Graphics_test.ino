#include "SmartResponseXE.h"
#include "fonts/fonts.h"
//#include "sherlock6.h"
#include "batman2.h"
//#include "mario4.h"
//#include "mario9.h"
//#include "mario7.h" //picture size is 384 x 136, the image was scaled and placed on that canvas

void setup() {
 
  SRXEInit(); // Initialize the LCD
 // SRXESetInverse(1);
  //SRXELoadBitmapRLE(0, 0, bitmap_sherlock6);
  SRXELoadBitmapRLE(0,0, bitmap_batman);
  //SRXELoadBitmapRLE(0,0,bitmap_mario4);
  /*
  SRXELoadBitmapRLE(0,0,bitmap_mario7);
  delay(5000);
  SRXEClearScreen();
  SRXESetPosition(0,0, 384,136);
  SRXEWriteString(0, 20, "Super Mario!", FONT_LARGE, normal, 3, 0);
  */
  //SRXELoadBitmapRLE(0,0,bitmap_mario9);
//delay(5000);
 // SRXEClearScreen();
 // SRXEWriteString(0, 20, "Super Mario!", FONT_LARGE, normal, 3, 0);
}

void loop() {

}

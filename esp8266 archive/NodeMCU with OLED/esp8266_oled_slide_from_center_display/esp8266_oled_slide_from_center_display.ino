
// works with board core esp8266 2.0.0 
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "src/esp8266_ssd1306/SSD1306.h" // alias for `#include "SSD1306Wire.h"`
SSD1306  display(0x3c, /*D1*/ 5, /*D2*/ 4);

//#include "src/esp8266_ssd1306/SH1106.h"
//SH1106 display(0x3c, 5, 4);

String name[] = {"L","e","R","o","y"," ","M","i","l","l","e","r"};
//String name[] = {"H","e","l","l","o"," ","W","o","r","l","d"};
int half;
int wait = 250; //delay for display

void setup() {
  Serial.begin(115200);
   // Initialising the UI will init the display too.
  display.init();

  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
half = (sizeof(name)/sizeof(name[0]))/2;
Serial.print("Half ");
Serial.println(half);
//half = half / 2;
Serial.print("Half of Half: ");
Serial.println(half);
display.clear();
display.display();
display.setTextAlignment(TEXT_ALIGN_CENTER);
display.drawString(65,10, name[half]);
display.display();
//Serial.println(name[half]);
delay(wait);

display.drawString(60, 10, name[half-1]);
display.drawString(70, 10, name[half+1]);
display.display();
delay(wait);

display.drawString(55, 10, name[half-2]);
display.drawString(73, 10, name[half+2]);
display.display();
delay(wait);

display.drawString(50, 10, name[half-3]);
display.drawString(76, 10, name[half+3]);
display.display();
delay(wait);

display.drawString(45, 10, name[half-4]);
display.drawString(80, 10, name[half+4]);
display.display();
delay(wait);

display.drawString(40, 10, name[half-5]);
display.drawString(83, 10, name[half+5]);
display.display();
delay(wait);

display.drawString(35, 10, name[half-6]);
display.drawString(86, 10, name[half+6]);
display.display();
delay(wait);

}

void loop() {
/*display.clear();
display.display();
//display.setTextAlignment(TEXT_ALIGN_CENTER);
display.drawString(65,0, name[half]);
display.display();
//Serial.println(name[half]);
delay(wait);

for (int i=half-1; i>0; i--) {
  display.drawString(65 - (i*5), 0, name[i]);
//  Serial.print(name[i]);
//  Serial.print(" ");
  display.drawString(65 + (i*5), 0, name[i+2]);
//  Serial.println(name[i+2]);
  display.display();
  delay(wait);
}

delay(5000);
display.clear();
display.display();
*/
  /*
display.setTextAlignment(TEXT_ALIGN_LEFT);
display.drawString(0,0,"LeRoy Miller");

display.setTextAlignment(TEXT_ALIGN_CENTER);
display.drawString(64,0,"LeRoy Miller");

display.setTextAlignment(TEXT_ALIGN_RIGHT);
display.drawString(128,0, "LeRoy Miller");

display.display();
*/

}

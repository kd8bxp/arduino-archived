/*
 Modified by Lauren
 version 0.1
 
 Any suggestions are welcome.
 E-mail: Lauran.pan@gmail.com
 
 Editor     : Lauren from DFRobot
 Date       : 26.03.2012
 
 * This is a sample sketch for displaying a variable value on the LCD4884
 * This library and sample is compatible with the IDE V1.0 and earlier
 
 Connection:
 
 Directly stack the LCD4884 shield on your Arduino board
 
*/

#include &quot;LCD4884.h&quot;

#define MENU_X	10		// 0-83
#define MENU_Y	1		// 0-5

int counter = 0;
char string[10];

void setup()
{

  lcd.LCD_init();
  lcd.LCD_clear();

  //menu initialization
  init_MENU();
}

void init_MENU(void){

  byte i;
  lcd.LCD_clear();
  lcd.LCD_write_string(MENU_X, MENU_Y, &quot;test screen&quot;, MENU_HIGHLIGHT );

}

void loop(){

  if(++counter &lt; 1000){
    itoa(counter,string,10);
    lcd.LCD_write_string(MENU_X, MENU_Y + 1, string, MENU_NORMAL);
  }
  else  counter = 0,init_MENU();
  delay(10);

}



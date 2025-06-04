//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD Shield.  The output is displayed on the LCD panel ... reacting from the buttons pressed
Mark Bramwell, July 2010
Greatly modified by Ray Scott, RabbitRobots.com 2017

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;  // integer .. Does not denote a Pin number
int adc_key_in  = 0; // Integer .. Does not denote a Pin number
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor .. Reading analog register ADC0
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For board verzion V1.1 us this threshold
/* if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
*/
 // For board version V1.0 comment the other threshold and use the one below:
 if (adc_key_in < 50)   return btnRIGHT;   // typically 0
 if (adc_key_in < 195)  return btnUP;   // typically 99
 if (adc_key_in < 380)  return btnDOWN;   // typically 257
 if (adc_key_in < 555)  return btnLEFT;   // typically 410
 if (adc_key_in < 790)  return btnSELECT;   // typically 641


 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
}
 
void loop()
{
 lcd.setCursor(12,1);            // move cursor to second line "1" and 12 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up


 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT      ");
      lcd.setCursor(7,1); 
     lcd.print(adc_key_in);
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT        ");
     lcd.setCursor(7,1); 
     lcd.print(adc_key_in);
     break;
     }
   case btnUP:
     {
     lcd.print("UP         ");
     lcd.setCursor(7,1); 
     lcd.print(adc_key_in);
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN       ");
     lcd.setCursor(7,1); 
     lcd.print(adc_key_in);
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT     ");
     lcd.setCursor(7,1); 
     lcd.print(adc_key_in);
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE       ");
     break;
     }
 }

}

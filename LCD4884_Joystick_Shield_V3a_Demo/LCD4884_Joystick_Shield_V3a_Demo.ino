/*
Modified by Amy
version 0.4
 
Description:
  Update the library and sketch to compatible with IDE V1.0 and earlier
  Improve the key detection function to be compatible with the Arduino Leonardo
  Improve the waitOK function
  Move the bit map array to the main code
 
Any suggestions are welcome.
E-mail: Lauran.pan@gmail.com
 
Editor     : Amy from DFRobot
Date       : 20.12.2012
 
 
*/
 
#include "LCD4884.h"
 
/*------------------------------------------------------------------------------
; DFrobot bitmap - size 48x24 pixels, black/white image
------------------------------------------------------------------------------*/
unsigned char DFrobot_bmp[]=
 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x39,0x39,0x39,0x39,0x39,0x39,
0x01,0x01,0xFF,0xFF,0x01,0x01,0xC9,0xC9,0xC9,0xC9,0xC9,0xC9,0xFF,0xFF,0xFF,0xFF,
0xFF,0x19,0x19,0x19,0x19,0xFF,0xE6,0x00,0x7C,0xFE,0xFF,0xFF,0x83,0x01,0x01,0x01,
0x83,0xC6,0x7C,0x00,0x00,0xFF,0xFF,0xFF,0x19,0x19,0x19,0x99,0xFF,0xFE,0x00,0x38,
0xFE,0xFF,0xFF,0xC3,0x81,0x01,0x01,0x83,0x87,0xFE,0x3C,0x00,0x01,0x01,0xFF,0xFF,
0xFF,0xFF,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,
0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00
};
/******************************************************************************/
 
 
/*------------------------------------------------------------------------------
; DFrobot chinese - size 12x12  
------------------------------------------------------------------------------*/
unsigned char DFrobot_chinese[]=
{
  0x84,0x64,0xFF,0x24,0x44,0xFE,0x02,0x02,0xFF,0x02,0x00,0x00,0x01,0x00,0x07,0x04,
  0x02,0x01,0x00,0x00,0x07,0x04,0x07,0x00,
   
  0xA0,0xAF,0xA9,0xE9,0xAF,0x30,0xAF,0xA9,0xF9,0xAF,0xA0,0x00,0x00,0x07,0x04,0x04,
  0x07,0x00,0x07,0x04,0x04,0x07,0x00,0x00,
   
  0x00,0x00,0x00,0x80,0x60,0x1F,0x60,0x80,0x00,0x00,0x00,0x00,0x04,0x04,0x02,0x01,
  0x00,0x00,0x00,0x01,0x02,0x04,0x04,0x00,
   
  0x22,0x12,0x0A,0xBF,0x6A,0x52,0x4A,0x7F,0xCA,0x12,0x22,0x00,0x04,0x05,0x05,0x04,
  0x05,0x02,0x02,0x01,0x00,0x00,0x00,0x00,
   
  0x00,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x03,0x02,0x00,0x00,0x04,0x04,0x04,0x04,
  0x04,0x07,0x04,0x04,0x04,0x04,0x04,0x00,
   
  0x00,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x06,0x01,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
/******************************************************************************/
 
//keypad debounce parameter
#define DEBOUNCE_MAX 15
#define DEBOUNCE_ON  10
#define DEBOUNCE_OFF 3 
 
#define NUM_KEYS 5
 
#define NUM_MENU_ITEM   4
 
// joystick number
#define LEFT_KEY 0
#define CENTER_KEY 1
#define DOWN_KEY 2
#define RIGHT_KEY 3
#define UP_KEY 4
 
// menu starting points
 
#define MENU_X  10      // 0-83
#define MENU_Y  1       // 0-5
 
 
int  adc_key_val[5] ={
  50, 200, 400, 600, 800 };
 
// debounce counters
byte button_count[NUM_KEYS];
// button status - pressed/released
byte button_status[NUM_KEYS];
// button on flags for user program 
byte button_flag[NUM_KEYS];
 
// menu definition
char menu_items[NUM_MENU_ITEM][12]={
  "TEMPERATURE",
  "CHAR MAP",
  "BITMAP",
  "ABOUT"  
};
 
void (*menu_funcs[NUM_MENU_ITEM])(void) = {
  temperature,
  charmap,
  bitmap,
  about
};
 
char current_menu_item;
 
void setup()
{
 
  // reset button arrays
  for(byte i=0; i<NUM_KEYS; i++){
    button_count[i]=0;
    button_status[i]=0;
    button_flag[i]=0;
  }
 
  lcd.LCD_init();
  lcd.LCD_clear();
 
  //menu initialization
  init_MENU();
  current_menu_item = 0;    
 
  lcd.backlight(ON);//Turn on the backlight
  //lcd.backlight(OFF); // Turn off the backlight  
}
 
unsigned long buttonFlasher = 0;
 
/* loop */
 
void loop()
{
  byte i;
  for(i=0; i<NUM_KEYS; i++){
    if(button_flag[i] !=0){
 
      button_flag[i]=0;  // reset button flag
      switch(i){
 
      case UP_KEY:
        // current item to normal display
        lcd.LCD_write_string(MENU_X, MENU_Y + current_menu_item, menu_items[current_menu_item], MENU_NORMAL );
        current_menu_item -=1;
        if(current_menu_item <0)  current_menu_item = NUM_MENU_ITEM -1;
        // next item to highlight display
        lcd.LCD_write_string(MENU_X, MENU_Y + current_menu_item, menu_items[current_menu_item], MENU_HIGHLIGHT );
        break;
      case DOWN_KEY:
        // current item to normal display
        lcd.LCD_write_string(MENU_X, MENU_Y + current_menu_item, menu_items[current_menu_item], MENU_NORMAL );
        current_menu_item +=1;
        if(current_menu_item >(NUM_MENU_ITEM-1))  current_menu_item = 0;
        // next item to highlight display
        lcd.LCD_write_string(MENU_X, MENU_Y + current_menu_item, menu_items[current_menu_item], MENU_HIGHLIGHT );
        break;
      case LEFT_KEY:
        init_MENU();
        current_menu_item = 0;
        break;   
      case CENTER_KEY:
        lcd.LCD_clear();
        (*menu_funcs[current_menu_item])();
        //(*menu_funcs[0])();        
        Serial.println("hello");
        lcd.LCD_clear();
        init_MENU();
        current_menu_item = 0;         
        break;  
      }
 
    }
  }
  if(millis() - buttonFlasher > 5){
    update_adc_key();
    buttonFlasher = millis();
   }    
}
 
/* menu functions */
 
void init_MENU(void){
 
  byte i;
 
  lcd.LCD_clear();
 
  lcd.LCD_write_string(MENU_X, MENU_Y, menu_items[0], MENU_HIGHLIGHT );
 
  for (i=1; i<NUM_MENU_ITEM; i++){
    lcd.LCD_write_string(MENU_X, MENU_Y+i, menu_items[i], MENU_NORMAL);
  }
 
 
}
 
// waiting for center key press
void waitfor_OKkey(){
  byte i;
  byte key = 0xFE;
  for(byte i=0; i<NUM_KEYS; i++){
    button_count[i]=0;
    button_status[i]=0;
    button_flag[i]=0;
  }
  update_adc_key();
  while (key!= CENTER_KEY){
    for(i=0; i<NUM_KEYS; i++){
      if(button_flag[i] !=0){
        button_flag[i]=0;  // reset button flag
        if(i== CENTER_KEY) 
        {
          key=CENTER_KEY; 
        }
      }
      update_adc_key();
    }
  }
}
 
void temperature()
{
  lcd.LCD_write_string_big(10, 1, "+12.30", MENU_NORMAL);
  lcd.LCD_write_string(78, 2, "C", MENU_NORMAL);
  lcd.LCD_write_string(38, 5, "OK", MENU_HIGHLIGHT );
  waitfor_OKkey();
}
 
void charmap(){
  char i,j;
  for(i=0; i<5; i++){
    for(j=0; j<14; j++){
      lcd.LCD_set_XY(j*6,i);
      lcd.LCD_write_char(i*14+j+32, MENU_NORMAL);
    }
  }
 
 
  lcd.LCD_write_string(38, 5, "OK", MENU_HIGHLIGHT );
  waitfor_OKkey();   
}
 
void bitmap(){
  lcd.LCD_draw_bmp_pixel(0,0, DFrobot_bmp, 84,24);
  lcd.LCD_write_chinese(6,3, DFrobot_chinese,12,6,0,0);
  lcd.LCD_write_string(38, 5, "OK", MENU_HIGHLIGHT );
  waitfor_OKkey();
}
 
 
void about(){
 
  lcd.LCD_write_string( 0, 1, "LCD4884 Shield", MENU_NORMAL);
  lcd.LCD_write_string( 0, 3, "www.DFrobot.cn", MENU_NORMAL);
  lcd.LCD_write_string(38, 5, "OK", MENU_HIGHLIGHT );
  waitfor_OKkey();
 
 
}
 
// which includes DEBOUNCE ON/OFF mechanism, and continuous pressing detection
// Convert ADC value to key number
 
char get_key(unsigned int input)
{
  char k;
 
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
 
      return k;
    }
  }
 
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
 
  return k;
}
 
void update_adc_key(){
  int adc_key_in;
  char key_in;
  byte i;
 
  adc_key_in = analogRead(0);
  key_in = get_key(adc_key_in);
  for(i=0; i<NUM_KEYS; i++)
  {
    if(key_in==i)  //one key is pressed 
    { 
      if(button_count[i]<DEBOUNCE_MAX)
      {
        button_count[i]++;
        if(button_count[i]>DEBOUNCE_ON)
        {
          if(button_status[i] == 0)
          {
            button_flag[i] = 1;
            button_status[i] = 1; //button debounced to 'pressed' status
          }
 
        }
      }
 
    }
    else // no button pressed
    {
      if (button_count[i] >0)
      {  
        button_flag[i] = 0; 
        button_count[i]--;
        if(button_count[i]<DEBOUNCE_OFF){
          button_status[i]=0;   //button debounced to 'released' status
        }
      }
    }
  }
}
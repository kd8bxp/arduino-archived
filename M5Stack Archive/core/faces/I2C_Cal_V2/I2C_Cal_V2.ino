#include "src/M5Stack/M5Stack.h"

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

float Data1 = 0;
float Data2 = 0;
float Data3 = 0;
float Data4 = 0;
char Fun;
char Fum2;
char Fum3;
int CalNum = 0;
int SumFlag = 0;
float Sum;
String Datain;
uint8_t key_val;


void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextFont(4);
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
  CalNum = 1;
  Draw_Screen();
 }


void loop()
{
  if(digitalRead(KEYBOARD_INT) == LOW) 
   {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    
    while (Wire.available()) 
     { 
      uint8_t key_val = Wire.read();                  // receive a byte as character
      if(key_val != 0) 
       {    
        if(isDigit(key_val) || key_val == '.')
         {
          if( SumFlag == 1 )
           {
            StoreData();
           }
          Datain += (char)key_val;
          IsNumber(); 
         }

        if( key_val == 'A' || key_val == '+' || key_val == '-' || key_val == '*' || key_val == '/' || key_val == '=' )
         {
          FunKey(key_val); 
         }
      }
    }
  }   
}           
   

void FunKey( uint8_t key_in )  
 {   
   switch (key_in) 
    {
     case 'A':
      M5.Lcd.clear();
      Draw_Screen();
      Data1 = 0;
      Data2 = 0;
      Sum = 0;
      Datain = "                     ";
      CalNum = 1;
     break;

     case '=':
      if( Fun == '+' )
       {
        Sum = Data1 + Data2; 
       }

      if( Fun == '-' )
       {
        Sum = Data1 - Data2;
       }

      if( Fun == '*' )
       {
        Sum = Data1 * Data2;
       }

      if( Fun == '/' )
       {
        Sum = Data1 / Data2;
       }
 
      M5.Lcd.setCursor(105,190);
      M5.Lcd.print(Sum); 
      SumFlag = 1;
     break;

     case '+': 
      Fun = '+';
      CalNum += 1 ;
      Datain = "                 ";
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print("   ");
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print(Fun);
     break;  

     case '-':
      Fun = '-';
      CalNum += 1;
      Datain = "                 ";  
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print("   ");
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print(Fun);   
     break;

     case '*':
      Fun = '*';
      CalNum += 1; 
      Datain = "                 ";  
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print("   "); 
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print(Fun);
     break;

     case '/':
      Fun = '/';
      CalNum += 1; 
      Datain = "                 ";
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print("   ");
      M5.Lcd.setCursor(20, 140);
      M5.Lcd.print(Fun);
     break;

     default:
     break;
   } 
 }
 

void StoreData( void )    
{
 Data1 = Sum;
 Data2 = 0;
 SumFlag = 0;
 Datain = "                     ";
 M5.Lcd.setCursor(20, 140);
 M5.Lcd.print(Fun);
 M5.Lcd.setCursor(70, 100);
 M5.Lcd.print("                  ");
 M5.Lcd.setCursor(70, 140);
 M5.Lcd.print("                  ");
 M5.Lcd.setCursor(70, 100);
 M5.Lcd.print(Data1);
 M5.Lcd.setCursor(105,190);
 M5.Lcd.print("                 ");
}


void IsNumber( void )
 {
  if( CalNum == 1 )
   {
    Data1 = Datain.toFloat();
    M5.Lcd.setCursor(70,100);
    M5.Lcd.print(Data1);
   } 
         
  if( CalNum >= 2 )
   {
    Data2 = Datain.toFloat();
    M5.Lcd.setCursor(70, 140);
    M5.Lcd.print(Data2);
   }
 }

 
 void DrawBox( int LocX, int LocY, int W, int H )
  {
   M5.Lcd.drawLine( LocX, LocY, LocX + W, LocY, TFT_WHITE);
   M5.Lcd.drawLine( LocX, LocY + H, LocX + W, LocY + H, TFT_WHITE);
   M5.Lcd.drawLine( LocX, LocY, LocX, LocY + H, TFT_WHITE);
   M5.Lcd.drawLine( LocX + W, LocY, LocX + W, LocY + H, TFT_WHITE);
  }

 void Draw_Screen( void )
  {
   M5.Lcd.clear();
   M5.Lcd.drawLine( 1, 180, 320, 180, TFT_WHITE);
   M5.Lcd.setCursor( 15,20 );
   M5.Lcd.print( "FRACTION CALCULATOR" );
   DrawBox( 50, 95, 250, 32 );
   DrawBox( 10, 135, 30, 32 );
   DrawBox( 50, 135, 250, 32 );
   M5.Lcd.setCursor(10,190);
   M5.Lcd.print("SUM  =");
  }

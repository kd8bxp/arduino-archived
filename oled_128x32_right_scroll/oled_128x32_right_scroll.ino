#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiAvrI2c oled;

String msg= "Hello World!";

void setup() {
  // put your setup code here, to run once:
oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);  
  Serial.begin(9600);
  oled.clear();
  Scroll(msg);
 
}

void loop() {
  // put your main code here, to run repeatedly:

}


String Scroll(String& str) {
  int i;
  String temp;
  String temp1;
  int cursor;
  for (i=0; i< str.length()+1; i++) {
    cursor = ((80-str.length()+1) - i)/2;
    oled.setCursor(cursor,0);
    //temp = Left(str, i);
    //temp = Mid(str, 0 ,i);
    temp = Right(str,i);
    temp1 += temp;
    oled.print(temp1);
    temp1 = "";
    delay(150);
  }
}

String Mid(String& str, int pos1, int pos2)
{
    int i;
    String temp = "";
    for(i = pos1; i < pos2; i++)
    {
        temp += str[i];
    }

    return temp;
}

String vMid(String& str, int pos1, int length)
{
    int i;
    String temp = "";
    for(i = pos1; i < pos1+length; i++)
    {
        temp += str[i];
    }
    return temp;
}

String Left(String& str, int pos)
{
    int i;
    String temp = "";
    for(i = 0; i < pos; i++)
    {
        temp += str[i];
    }

    return temp;
}

String Right(String& str, int pos)
{
    int i;
    String temp = "";
    for( i = str.length()-pos; i < str.length(); i++)
      //i = strlen(str.c_str()); i > pos; i--)
    //pos; i < strlen(str.c_str()); i++)
    {
        temp += str[i];
    }
    return temp;
}



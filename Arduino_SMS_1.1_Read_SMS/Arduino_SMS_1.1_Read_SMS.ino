 /*
PLEASE READ FIRST

The incoming structure is like this: NUMBER\nTEXT
You have to separate the number and the text.
You can realize it with the function getnumberandtext.
This function saves the number in the char array innumber and the text in char array intext.
Copy the code on your Arduino and start the Serial Monitor, then start the Arduino SMS App and connect your phone with your Bluetooth module.
HAVE FUN
*/

#include <SoftwareSerial.h>
//                      Rx  Tx
SoftwareSerial mySerial(10, 8); //take your RX and TX pin of your Bluetooth Module

#define maxnumber  20           //maximum number length
#define maxtext   100           //maximum length of text

char innumber[maxnumber] = "";
char intext[maxtext] = "";

void setup()
{
 Serial.begin(9600);
 mySerial.begin(9600);
}

void loop()
{
 //is a message available? Save the number and text into two variable innumber and intext
 if(getnumberandtext(innumber, intext))
 {
  String str(intext);
  String str2(innumber);
  Serial.print(str2);
  Serial.print(": ");
  Serial.print(str);
  Serial.print("\n");
 }
 
}



unsigned char getnumberandtext(char* numberptr, char* textptr)
{
  int i = 0;
  char* ptr = 0;
  char instring[maxnumber + maxtext] = "";
  int  enable=0;
 
  //wait for incoming message and save it in instring[].
  while(mySerial.available() > 0)
  {
    instring[i] = mySerial.read();
    i++;
    enable = 1;
  }
 
  //is a message available?
  if(enable)
  {
    enable = 0;
    ptr = instring;
    for(i = 0; *ptr != '\n'; i++)
    {
      *numberptr = *ptr;
      numberptr++;
      ptr++;
    }
   
    for(i = i; i<maxnumber; i++)
    {
       *numberptr = 0;
       numberptr++;
    }
   
    ptr += 1;
   
    for(i = 0; *ptr != '\0'; i++)
    {
      *textptr = *ptr;
      textptr++;
      ptr++;
    }
   
     for(i = i; i<maxtext; i++)
    {
      *textptr = 0;
      textptr++;
    }
   
    return(1);
  }
}


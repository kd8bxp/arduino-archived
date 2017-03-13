 /*
PLEASE READ FIRST

You need to send the phone number and the text in the following structure: NUMBER/TEXT e.g: 1234/hello
Copy the code on your Arduino and start the Serial Monitor, then start the Arduino SMS App and connect your phone with your Bluetooth module.
HAVE FUN
*/


#include <SoftwareSerial.h>
 //                      Rx  Tx
 SoftwareSerial mySerial(10, 8); //take your Rx and Tx pin of your Bluetooth Module
 char numberortext=0;
 char enable=0;
 String intext;
 String innumber;
 void setup()
 {
     Serial.begin(9600);         //take your Baud rate
     mySerial.begin(9600);
     while (!Serial)
     {
     ; // wait for serial port to connect.
     }
     Serial.println("set phone number");
 }
 void loop()
 {
   if(!Serial.available()&&enable)
   {
     numberortext=~numberortext;
     enable=0;
   }
   if(Serial.available())
   {
     if(numberortext&&!enable)
     {
       intext = Serial.readString();             
       Serial.println("set phone number");
       mySerial.print(innumber + "/" + intext);   //send number and text to your phone
     }
     if(!numberortext&&!enable)
     {
      innumber = Serial.readString();
       Serial.println("set SMS text");
     }
      enable=1;
   }
 }

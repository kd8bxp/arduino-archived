#include "SoftwareSerial.h"
SoftwareSerial mySerial(10,11); // Pin 10 connected to TX on HC-06

/*
AT+VERSION returns the software version of the module
AT+BAUDx sets the baud rate of the module  1> 1200baud, 2>2400, 3>4800, 4>9600(default), 5>19200,6>38400,7>57600,8>115200
AT+NAMEabcdefg sets a name of the module
AT+PINxxxx sets the pin for the module
AT+PN sets the parity of the module 
*/

void setup() {
Serial.begin(9600);
Serial.println("starting!");
//mySerial.begin(4800);
mySerial.begin(9600);
//mySerial.begin(19200);
//mySerial.begin(23400);
//mySerial.begin(38400);
//mySerial.begin(57600);
//mySerial.begin(115200);
Serial.write(".");
//mySerial.begin(57600);
//mySerial.begin(115200);
delay(2000);
mySerial.print("AT+NAMEHC-06");
Serial.write(".");
delay(2000);
if (mySerial.available()) { 
Serial.write(mySerial.read());
}

mySerial.print("AT+PIN1234");
Serial.write(".");
delay(2000);
if (mySerial.available()) { 
Serial.write(mySerial.read());
}
/*
mySerial.print("AT+PIN1111");
Serial.write(".");
delay(2000);
if (mySerial.available()) { 
Serial.write(mySerial.read());
}
//mySerial.print(“AT+BAUD7”);// Set baudrate to 57600
//Serial.write(“.”);
//delay(2000);
*/
Serial.write("Done!");
}

void loop() {
	
}
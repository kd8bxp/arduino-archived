//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider 
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//
 
 
char c=' ';
boolean NL = true;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    Serial3.begin(9600);  
    Serial.println("Serial3 started at 9600");
    Serial3.print("AT+RESET\n\r");
}
 
void loop()
{
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (Serial3.available())
    {
        c = Serial3.read();
        //Serial.write(c);
        Serial.print(c);
    }
 
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        if (c!=10 & c!=13 ) 
        {  
             //Serial3.write(c);
             Serial3.print(c);
        }
 
        // Echo the user input to the main window. The ">" character indicates the user entered text.
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}

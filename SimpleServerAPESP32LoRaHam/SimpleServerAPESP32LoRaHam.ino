/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
#include <ESP32WebServer.h>

const char *ssid= "LoRaHam";
const char *password = "pass1234";

ESP32WebServer server(80);

String TO = "BEACON";
String FROM = "KD8BXP-10";
String MESSAGE1 = "This is a message 1.200 ESP32";
String RT = "";
String MYCALL = "KD8BXP";

void setup()
{
    Serial.begin(9600);
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.print(myIP);

    server.on("/", []()
      {
        String temp;
        temp = "<meta name=viewport content='width=200'><center><h1>LoRaHam</h1></center>Last Message:<br><br>To: " + TO +"<br>From: " +FROM+"<br>Message: "+MESSAGE1+"<br>RT: "+RT+"<br><br><a href=\"/msg\">Send Message!</a>";
        Serial.println(temp);
        server.send(200, "text/html", temp);
      });
      
    server.on("/cfg", []()
      {
        server.send(200, "text/html", "CFG Screen");
      });

    server.on("/msg", []()
      {
        String form = "<meta name=viewport content='width=200'><style>input, textarea {max-width:100%}</style><p><center><form action='msg2'><p></center>TO: <input type='text' name='TO' size=15 vaule='"+FROM+"' autofocus><br>Message: <input type='text' name='MSG' size=75><center><br><br> <input type='submit' value='Submit'></form></center>";
        server.send(200, "text/html", form);// And as regular external functions:
      });

    server.on("/msg2", []()
      {
        server.send(200,"text/html","Nothing Here Yet!");
      });
    server.begin();
}

void loop(){
   server.handleClient();
}


#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0x4C, 0x84 }; // Must be unique on local network

#define thingSpeakInterval 16000 // Time interval in milliseconds to get data from ThingSpeak (number of seconds * 1000 = interval)
// Variable Setup
long lastConnectionTime = 0;

String lastCommandString = "black";
boolean lastConnected = false;
int failedCounter = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, 6, NEO_GRB + NEO_KHZ800);

// Initialize Arduino Ethernet Client
EthernetClient client;

void setup() {
delay(100);
// Setup Serial
Serial.begin(9600);
delay(100);
Serial.flush();
delay(100);

strip.begin();
strip.show();

// Start Ethernet on Arduino
startEthernet();
}

void loop() {
// Process CheerLights Commands
if(client.available() > 0)
{
delay(100);
Serial.println(client.available());
String response;
char charIn;
do {
charIn = client.read(); // read a char from the buffer
response += charIn; // append that char to the string response
} while (client.available() > 0);
Serial.println(response.length());
Serial.println(response);

if (response.indexOf("white") > 0 || response.indexOf("warmwhite") > 0)
{
lastCommandString = "white";
colorWipe(strip.Color(255,255,255),50);
}
else if (response.indexOf("black") > 0 || response.indexOf("off") > 0)
{
lastCommandString = "black";
colorWipe(strip.Color(0,0,0),50);
}
else if (response.indexOf("red") > 0)
{
lastCommandString = "red";
colorWipe(strip.Color(255,0,0),50);
}
else if (response.indexOf("green") > 0)
{
lastCommandString = "green";
colorWipe(strip.Color(0, 255, 0), 50);
}
else if (response.indexOf("blue") > 0)
{
lastCommandString = "blue";
colorWipe(strip.Color(0,0,255),50);
}
else if (response.indexOf("cyan") > 0)
{
lastCommandString = "cyan";
colorWipe(strip.Color(0,255,255),50);
}
else if (response.indexOf("magenta") > 0)
{
lastCommandString = "magenta";
colorWipe(strip.Color(255,0,255),50);
}
else if (response.indexOf("yellow") > 0)
{
lastCommandString = "yellow";
colorWipe(strip.Color(255,255,0),50);
}
else if (response.indexOf("purple") > 0)
{
lastCommandString = "purple";
colorWipe(strip.Color(102,51,204),50);
}
else if (response.indexOf("orange") > 0)
{
lastCommandString = "orange";
colorWipe(strip.Color(255,153,0),50);
}
/*else if (response.indexOf("warmwhite") > 0)
{
lastCommandString = "warmwhite";
fadeToColor(lastCommand,WARMWHITE,stepdelay);
for (int i = 0; i < 3; i++) {
lastCommand[i] = WARMWHITE[i];
}
}
else if (response.indexOf("black") > 0)
{
lastCommandString = "black";
fadeToColor(lastCommand,BLACK,stepdelay);
for (int i = 0; i < 3; i++) {
lastCommand[i] = BLACK[i];
}
}*/
else
{
lastCommandString = "(no match)";
}
// Echo command
delay(200);
Serial.print("CheerLight Command Received: ");
Serial.println(lastCommandString);
delay(200);
}
// Disconnect from ThingSpeak
if (!client.connected() && lastConnected)
{
Serial.println("...disconnected");
client.stop();
}
// Subscribe to ThingSpeak Channel and Field
if(!client.connected() && (millis() - lastConnectionTime > thingSpeakInterval))
{
subscribeToThingSpeak();
}
// Check if Arduino Ethernet needs to be restarted
if (failedCounter > 3 ) {startEthernet();}
lastConnected = client.connected();
delay(100);
} // End loop
void subscribeToThingSpeak()
{
if (client.connect("api.thingspeak.com", 80))
{
Serial.println("Connecting to ThingSpeak...");
failedCounter = 0;
Serial.println("Sending Request");
client.println("GET /channels/1417/field/1/last.txt HTTP/1.0");
client.println();
lastConnectionTime = millis();
}
else
{
failedCounter++;
Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");
Serial.println();
lastConnectionTime = millis();
}
}
void startEthernet()
{
client.stop();
Serial.println("Connecting Arduino to network...");
Serial.println();
delay(1000);
// Connect to network amd obtain an IP address using DHCP
if (Ethernet.begin(mac) == 0)
{
Serial.println("DHCP Failed, reset Arduino to try again");
Serial.println();
}
else
{
Serial.println("Arduino connected to network using DHCP");
Serial.println();
}
delay(1000);
}

void colorWipe(uint32_t c, uint8_t wait) {
	for (int t=0; t<25; t++) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
strip.setPixelColor(i, 0,0,0);
delay(wait);
}
	}
	
}


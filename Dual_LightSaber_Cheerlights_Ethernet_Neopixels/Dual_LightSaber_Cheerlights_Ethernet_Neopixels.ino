
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>


static int8_t Send_buf[8] = {0} ;

#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
  #define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_SINGLE_CYCLE 0X19
  #define SINGLE_CYCLE_ON 0X00
  #define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22


// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0x4C, 0x84 }; // Must be unique on local network

#define thingSpeakInterval 16000 // Time interval in milliseconds to get data from ThingSpeak (number of seconds * 1000 = interval)
// Variable Setup
long lastConnectionTime = 0;

String lastCommandString = "black";
boolean lastConnected = false;
int failedCounter = 0;

int PIN = 3; //NeoPixel Pin Saber 1
int PIN2nd = 5; //NeoPixel Pin Saber 2
int totalLEDs = 30;
int ledFadeTime = 5;
int isOn = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLEDs, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(totalLEDs, PIN2nd, NEO_GRB + NEO_KHZ800);

// Initialize Arduino Ethernet Client
EthernetClient client;

void setup() {
delay(100);
// Setup Serial
Serial.begin(9600);
Serial1.begin(9600);
sendCommand(CMD_SEL_DEV, DEV_TF);
delay(100);
Serial.flush();
delay(100);

strip.begin();
strip2.begin();
strip.show();
strip2.show();

// Start Ethernet on Arduino
startEthernet();
}

void loop() {

if(client.available() > 0)
{
delay(100);
//Serial.println(client.available());
String response;
char charIn;
do {
charIn = client.read(); // read a char from the buffer
response += charIn; // append that char to the string response
} while (client.available() > 0);
//Serial.println(response.length());
Serial.println(response);

if (response == "white" || response == "warmwhite" || response == "oldlace")
{
	if (lastCommandString == "white") {rgbFadeInAndOut(255,255,255, ledFadeTime); } else {
lastCommandString = "white";
saberoff();
saberon(255,255,255,ledFadeTime);
//colorWipe(strip.Color(255,255,255), 50);
}
}
if (response == "black" || response == "off")
{
	if (lastCommandString == "black") { rgbFadeInAndOut(0,0,0, ledFadeTime); } else {
lastCommandString = "black";
saberoff();
saberon(0,0,0, ledFadeTime);
//colorWipe(strip.Color(0,0,0),50);
}
}
if (response == "red")
{
	if (lastCommandString == "red") { rgbFadeInAndOut(255,0,0, ledFadeTime); } else {
lastCommandString = "red";
saberoff();
saberon(255,0,0,ledFadeTime);
//colorWipe(strip.Color(255,0,0),50);
}
}
if (response == "green")
{ if (lastCommandString == "green") {rgbFadeInAndOut(0, 255, 0, ledFadeTime); } else {
lastCommandString = "green";
saberoff();
saberon(0,255,0,ledFadeTime);
//colorWipe(strip.Color(0, 255, 0), 50);
}
}
if (response == "blue")
{
	if (lastCommandString == "blue") {rgbFadeInAndOut(0, 0, 255, ledFadeTime); } else {
lastCommandString = "blue";
saberoff();
saberon(0,0,255, ledFadeTime);
//colorWipe(strip.Color(0,0,255),50);
}
}
if (response == "cyan")
{
	if (lastCommandString == "cyan") {rgbFadeInAndOut(0, 255,255, ledFadeTime); } else {
lastCommandString = "cyan";
saberoff();
saberon(0,255,255, ledFadeTime);
//colorWipe(strip.Color(0,255,255),50);
}
}
if (response == "magenta")
{ 
	if (lastCommandString == "magenta") {rgbFadeInAndOut(255, 0, 255, ledFadeTime); } else {
lastCommandString = "magenta";
saberoff();
saberon(255,0,255, ledFadeTime);
//colorWipe(strip.Color(255,0,255),50);
}
}
if (response == "yellow")
{
	if (lastCommandString == "yellow") {rgbFadeInAndOut(255,255,0, ledFadeTime);} else {
lastCommandString = "yellow";
saberoff();
saberon(255,255,0, ledFadeTime);
//colorWipe(strip.Color(255,255,0),50);
}
}
if (response == "purple")
{
	if (lastCommandString == "purple") {rgbFadeInAndOut(102, 51,204, ledFadeTime); } else {
lastCommandString = "purple";
saberoff();
saberon(102,51,204, ledFadeTime);
//colorWipe(strip.Color(102,51,204),50);
}
}
if (response == "orange")
{
	if (lastCommandString == "orange") {rgbFadeInAndOut(255,153,0, ledFadeTime); } else {
lastCommandString = "orange";
saberoff();
saberon(255,153,0,ledFadeTime);
//colorWipe(strip.Color(255,153,0),50);
}
}
if (response == "pink"){
	if (lastCommandString == "pink") {rgbFadeInAndOut(255,53,153, ledFadeTime); } else { 
	lastCommandString = "pink";
	saberoff();
	saberon(255,53,153, ledFadeTime);

}
}
//delay(200);
Serial.print("CheerLight Command Received: ");
Serial.println(lastCommandString);
//delay(200);
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
client.println("GET /channels/1417/field/1/last.txt");
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
strip2.setPixelColor(i, c);
strip.show();
strip2.show();
strip.setPixelColor(i, 0,0,0);
strip2.setPixelColor(i, 0,0,0);
delay(wait);
}
	}
	
}

void rgbFadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
 strip.setPixelColor(0, 255, 0,0);
 strip2.setPixelColor(0,0,0,255);
 //sendCommand(CMD_PLAY_W_VOL, 0X1E01);
 for (int ii = 0; ii<10; ii++) {
 	sendCommand(CMD_PLAY_W_VOL, 0X1001);
  for(uint8_t b = 75; b <255; b++) {
     for(uint8_t i=8; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
        strip2.setPixelColor(i, red*b/255, green * b/255, blue * b/255);
     }
	
     strip.show();
     strip2.show();
     delay(wait);
  };

  for(uint8_t b=255; b > 75; b--) {
     for(uint8_t i = 8; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
        strip2.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
     }
     strip.show();
     strip2.show();
     delay(wait);
  };
 };
};

void saberon(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
	strip.setPixelColor(0, 255, 0, 0);
	strip2.setPixelColor(0, 0, 0, 255);
	sendCommand(CMD_PLAY_W_VOL, 0X1004);
	if (isOn == 1) {
		rgbFadeInAndOut(red, green, blue, wait);
	}
	
	for (uint8_t i = 8; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, red, green, blue);
		strip2.setPixelColor(i, red, green, blue);
		strip.show();
		strip2.show();
		delay(25);
	}
	isOn == 1;
}

void saberoff() {
	sendCommand(CMD_PLAY_W_VOL, 0X1003);
	for (uint8_t i = strip.numPixels(); i > 7; i--) {
		strip.setPixelColor(i, 0, 0, 0);
		strip2.setPixelColor(i, 0, 0, 0);
		strip.show();
		strip2.show();
		delay(25);
	}
	
isOn = 0;

}

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    Serial1.write(Send_buf[i]) ;
  }
}

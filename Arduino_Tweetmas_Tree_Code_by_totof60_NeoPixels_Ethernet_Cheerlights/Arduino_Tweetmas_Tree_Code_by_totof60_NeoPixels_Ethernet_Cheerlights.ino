
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

// Local Network Settings
byte mac[] = { 0xBC, 0x2D, 0x41, 0x70, 0x07, 0x98 }; // Must be unique on local network
#define Brightness 5 //Set brightness to 5/10th
#define Full (255/Brightness)
#define thingSpeakInterval 18000 // Time interval in milliseconds to get data from ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0;

String lastCommandString = "black";
boolean lastConnected = false;
int failedCounter = 0;

int brightness = 50;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(90, 6, NEO_GRB + NEO_KHZ800);

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
lastCommandString = "white";
theaterChaseRainbow(50);
}
else if (response == "black" || response == "off")
{
lastCommandString = "black";
theaterChase(strip.Color(0,0,0),50);
}
else if (response == "red")
{
lastCommandString = "red";
//theaterChase(strip.Color(255,0,0),50);
CandyCane(30,8,50);
}
else if (response == "green")
{
lastCommandString = "green";
theaterChase(strip.Color(0, 255, 0), 50);
}
else if (response == "blue")
{
lastCommandString = "blue";
theaterChase(strip.Color(0,0,255),50);
}
else if (response == "cyan")
{
lastCommandString = "cyan";
theaterChase(strip.Color(0,255,255),50);
}
else if (response == "magenta")
{
lastCommandString = "magenta";
theaterChase(strip.Color(255,0,255),50);
}
else if (response == "yellow")
{
lastCommandString = "yellow";
theaterChase(strip.Color((brightness*255/255),(brightness*255/255),0),50);
//theaterChase(strip.Color(127,127,0),50);

}
else if (response == "purple")
{
lastCommandString = "purple";
theaterChase(strip.Color(102,51,204),50);
}
else if (response == "orange")
{
lastCommandString = "orange";
theaterChase(strip.Color(255,153,0),50);
theaterChase(strip.Color(0, 255, 0), 50);
}
else if (response == "pink")
{
lastCommandString = "pink";
theaterChase(strip.Color(255,53,153),50);
}

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
{subscribeToThingSpeak();
} 
checkcolor(lastCommandString);
delay(500);
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


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
  }
void CandyCane  (int sets,int width,int wait) {
  int L;
  for(int j=0;j<(sets*width);j++) {
    for(int i=0;i< strip.numPixels();i++) {
      L=strip.numPixels()-i-1;
      if ( ((i+j) % (width*2) )<width)
        strip.setPixelColor(L,Full,0,0);
      else
        strip.setPixelColor(L,Full,Full, Full);
    };
    strip.show();
    delay(wait);
  }

}
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<36; j++) {  //do 36 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  } 
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
}
}

//  rainbow wheel 
void rainbowCycle(uint8_t wait) {
int i, j;
for (j=0; j < 256 * 5; j++) { // 5 cycles of all 25 colors in the wheel
for (i=0; i < strip.numPixels(); i++) {

strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
}
strip.show(); // write all the pixels out
delay(wait);
}
}
void rainbow(uint8_t wait) {
int i, j;
for (j=0; j < 256; j++) { // 3 cycles of all 256 colors in the wheel
for (i=0; i < strip.numPixels(); i++) {
strip.setPixelColor(i, Wheel( (i + j) % 255));
}
strip.show(); // write all the pixels out
delay(wait);
}
}
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
uint32_t c;
c = r;
c <<= 8;
c |= g;
c <<= 8;
c |= b;
return c;
}


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
if (WheelPos < 85) {
return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
} else if (WheelPos < 170) {
WheelPos -= 85;
return Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else {
WheelPos -= 170;
return Color(0, WheelPos * 3, 255 - WheelPos * 3);
}	
	
}

void checkcolor(String colors) {
	if (colors == "white")
{
theaterChaseRainbow(50);
}
else if (colors == "black")
{
theaterChase(strip.Color(0,0,0),50);
}
else if (colors == "red")
{
//theaterChase(strip.Color(255,0,0),50);
CandyCane(30,8,50); //30 sets, 8 pixels wide, 50us delay
}
else if (colors == "green")
{
theaterChase(strip.Color(0, 255, 0), 50);
}
else if (colors == "blue")
{
theaterChase(strip.Color(0,0,255),50);
}
else if (colors == "cyan")
{
theaterChase(strip.Color(0,255,255),50);
}
else if (colors == "magenta")
{
theaterChase(strip.Color(255,0,255),50);
}
else if (colors == "yellow")
{
theaterChase(strip.Color(255,255,0),50);
}
else if (colors == "purple")
{
theaterChase(strip.Color(102,51,204),50);
}
else if (colors == "orange")
{
theaterChase(strip.Color(255,153,0),50);
theaterChase(strip.Color(0, 255, 0), 50);
}
else if (colors == "pink")
{
theaterChase(strip.Color(255,53,153),50);
}

}






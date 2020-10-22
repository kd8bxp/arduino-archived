
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <TimedAction.h>

String lastCommandString = "black";
int brightness = 100;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 2, NEO_GRB + NEO_KHZ800);

void getColor();
//TimedAction PixelAction = TimedAction(40, colorWipe(uint32_t c, uint8_t wait));
TimedAction getColorAction = TimedAction(15000, getColor);

const char ssid[]     = "Motel6";
const char password[] = "";

const char host[] = "api.thingspeak.com";

void setup() {
delay(100);
// Setup Serial
Serial.begin(9600);
delay(100);
Serial.flush();
delay(100);
strip.begin();
strip.show();
strip.setBrightness(brightness);

/*Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
*/
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
  //      Serial.print(".");
    }

   /* Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
*/
getColor();

}

void loop() {
  check();
}

void check(){
  getColorAction.check();
  //PixelAction.check();
}

void getColor() {

Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/channels/1417/field/2/last.txt";

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
            "Host: " + host + "\r\n" +
            "Connection: close\r\n\r\n");

 

// Process CheerLights Commands
delay(100);
String response;
char charIn;
do {
charIn = client.read(); // read a char from the buffer
response += charIn; // append that char to the string response
} while (client.available() > 0);
//response.remove(519,7);
response.remove(0,645);
response.remove(0,3);
response.remove(6);
Serial.println(response.length());
Serial.println(response);
//char charbuf[8];
//response.toCharArray(charbuf,8);
//long int rgb=strtol(response,0,16); //=>rgb=0x001234FE;
unsigned long color;
//color = strtoul(response.c_str()+1, 0, 16);
color = strtoul(response.c_str(),0,16);
int r=(color & 0xFF0000) >>16;
//int r=(color>>16);
//int g=(color>>8);
//int b=(color);
int g=(color & 0x00FF00) >>8;
int b=(color & 0x0000FF);
Serial.print("R: ");
Serial.println(r);
Serial.print("G: ");
Serial.println(g);
Serial.print("B: ");
Serial.println(b);

colorWipe(strip.Color(r,g,b),50);
/*
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

else
{
lastCommandString = "(no match)";
}

*/
}


void colorWipe(uint32_t c, uint8_t wait) {
	for (int t=0; t<250; t++) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
strip.setPixelColor(i, 0,0,0);
delay(10); //delay(wait);
check();
}
strip.setPixelColor(strip.numPixels(), 0,0,0);
strip.show();
	}
	
}



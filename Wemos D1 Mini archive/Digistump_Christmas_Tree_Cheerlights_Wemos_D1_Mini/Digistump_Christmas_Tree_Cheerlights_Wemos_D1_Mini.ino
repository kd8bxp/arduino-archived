#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

const char* ssid     = "";
const char* password = "";

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            D1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      9 //13 for dove, 9 for menorah or tree
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
bool firstColor = true;
String lastColor = "";

void setup() {
  Serial.begin(9600);
  pixels.begin(); 
   WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

uint8_t redIndex[13];
uint8_t greenIndex[13];
uint8_t blueIndex[13];

void setColor(String colorname){
  uint8_t newcolor[3];
  if(colorname == "red"){
    newcolor[0] = 255;
    newcolor[1] = 0;
    newcolor[2] = 0;
  }
  else if(colorname == "green"){
    newcolor[0] = 0;
    newcolor[1] = 128;
    newcolor[2] = 0;
  }
  else if(colorname == "blue"){
    newcolor[0] = 0;
    newcolor[1] = 0;
    newcolor[2] = 255;
  }
  else if(colorname == "cyan"){
    newcolor[0] = 0;
    newcolor[1] = 255;
    newcolor[2] = 255;
  }
  else if(colorname == "white"){
    newcolor[0] = 255;
    newcolor[1] = 255;
    newcolor[2] = 255;
  }
  else if(colorname == "oldlace"){
    newcolor[0] = 253;
    newcolor[1] = 245;
    newcolor[2] = 230;
  }
  else if(colorname == "warmwhite"){
    newcolor[0] = 253;
    newcolor[1] = 245;
    newcolor[2] = 230;
  }
  else if(colorname == "purple"){
    newcolor[0] = 128;
    newcolor[1] = 0;
    newcolor[2] = 128;
  }
  else if(colorname == "magenta"){
    newcolor[0] = 255;
    newcolor[1] = 0;
    newcolor[2] = 255;
  }
  else if(colorname == "yellow"){
    newcolor[0] = 255;
    newcolor[1] = 255;
    newcolor[2] = 0;
  }
  else if(colorname == "orange"){
    newcolor[0] = 255;
    newcolor[1] = 165;
    newcolor[2] = 0;
  }
  else if(colorname == "pink"){
    newcolor[0] = 255;
    newcolor[1] = 192;
    newcolor[2] = 203;
  }

  if(firstColor){
    firstColor = false;
    for(uint8_t x = 0; x<NUMPIXELS; x++){
      redIndex[x] = newcolor[0];
      greenIndex[x] = newcolor[1];
      blueIndex[x] = newcolor[2];
    }
  }
  else{
    for(uint8_t x = NUMPIXELS-1; x>0; x--){
      redIndex[x] = redIndex[x-1];
      greenIndex[x] = greenIndex[x-1];
      blueIndex[x] = blueIndex[x-1];
    }
    redIndex[0] = newcolor[0];
    greenIndex[0] = newcolor[1];
    blueIndex[0] = newcolor[2];
  }

  for(uint8_t x = 0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(redIndex[x],greenIndex[x],blueIndex[x])); 
  }
  
  pixels.show();
}

void loop() {


     HTTPClient http;

     http.begin("api.thingspeak.com", 80, "/channels/1417/field/1/last.txt"); //HTTP

        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode) {
            // file found at server
            if(httpCode == 200) {
                String payload = http.getString();
                if(payload != lastColor){
                  setColor(payload);
                }
            }
        } 
    

    //check every 30 seconds

    delay(30000);
}

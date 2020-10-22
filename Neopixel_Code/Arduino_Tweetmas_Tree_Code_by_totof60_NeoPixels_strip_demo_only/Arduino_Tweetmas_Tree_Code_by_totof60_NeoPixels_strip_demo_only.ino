

#include <Adafruit_NeoPixel.h>

// Local Network Settings

#define Brightness 30 //Set brightness to 5/10th
#define Full (255/Brightness)
#define thingSpeakInterval 18000 // Time interval in milliseconds to get data from ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0;

String lastCommandString = "black";
boolean lastConnected = false;
int failedCounter = 0;


//Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, 3, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, 3, NEO_RGB + NEO_KHZ400);

// Initialize Arduino Ethernet Client
void setup() {
delay(100);
// Setup Serial
Serial.begin(9600);
delay(100);
Serial.flush();
delay(100);

strip.begin();
strip.show();

}

void loop() {

theaterChase(strip.Color(102,51,204),500);
//delay(1000);
CandyCane(100,3,100);
//delay(1000);
theaterChaseRainbow(70);
//delay(1000);
rainbowCycle(5);
//delay(1000);

XMas(100,3,50);
//delay(1000);
//CandyCane(100,3,50);
} // End loop

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

//added XMas - LFM - color cycle RED/GREEN	
void XMas (int sets,int width,int wait) {
  int L;
  for(int j=0;j<(sets*width);j++) {
    for(int i=0;i< strip.numPixels();i++) {
      L=strip.numPixels()-i-1;
      if ( ((i+j) % (width*2) )<width)
        strip.setPixelColor(L,Full,0,0);
      else
        strip.setPixelColor(L,0,Full,0);
    };
    strip.show();
    delay(wait);
  }
	
}








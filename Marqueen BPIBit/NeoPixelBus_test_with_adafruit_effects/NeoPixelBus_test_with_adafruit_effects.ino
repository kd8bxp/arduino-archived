
//NeoPixelBus works better than FastLED or Adafruit Neopixel library for the BPI-BIT
//it doesn't appear to suffer from the flickering problem that the other libraries have.
//This is one of Adafruits demos ported to use NeoPixelBus - (it is really more of a merge of two demos)
//LeRoy Miller, 2018 (Dec 10, 2018)

//compiles with ESP32 Core 1.0.6

#include "src/NeoPixelBus/NeoPixelBus.h"

const uint16_t PixelCount = 4; //25; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = P15; //4;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128
#define ledPower -1 //P15 //2

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);


int red, green, blue;

#define LEDC_CHANNEL_0 0
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 450
#define Buzzer 25

void ledcAnalogWrite(uint8_t channel, uint32_t value)
{
    // calculate duty, 8191 from 2 ^ 13 - 1 (2 ^ LEDC_TIMER_13_BIT - 1)
    //value_MAX = 255
    uint32_t duty = (8191 / 255) * value;
    // write duty to LEDC
    ledcWrite(channel, duty);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(Buzzer, LEDC_CHANNEL_0);

    pinMode(ledPower,OUTPUT);

    Serial.println();
    Serial.println("Running...");
}


void loop()
{
    
 digitalWrite(ledPower,HIGH);//Open WS2812 Power
   
  colorWipe(255, 0, 0, 50); // Red
  colorWipe(0, 255, 0, 50); // Green
  colorWipe(0, 0, 255, 50); // Blue

  theaterChase(127, 127, 127, 50); // White
  theaterChase(127, 0, 0, 50); // Red
  theaterChase(0, 0, 127, 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);

}

// Fill the dots one after the other with a color
void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for(uint16_t i=0; i<PixelCount; i++) {
    strip.SetPixelColor(i, RgbColor(r,g,b));
    strip.Show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PixelCount; i++) {
      Wheel((i+j) & 255);
      strip.SetPixelColor(i, RgbColor(red, green, blue));
    }
    strip.Show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PixelCount; i++) {
      Wheel(((i * 256 / PixelCount) + j) & 255);
      strip.SetPixelColor(i, RgbColor(red,green,blue));
    }
    strip.Show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PixelCount; i=i+3) {
        strip.SetPixelColor(i+q, RgbColor(r,g,b));    //turn every third pixel on
      }
      strip.Show();

      delay(wait);

      for (uint16_t i=0; i < PixelCount; i=i+3) {
        strip.SetPixelColor(i+q, RgbColor(0,0,0));        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PixelCount; i=i+3) {
        Wheel( (i+j) % 255);
        strip.SetPixelColor(i+q, RgbColor(red, green, blue));    //turn every third pixel on
      }
      strip.Show();

      delay(wait);

      for (uint16_t i=0; i < PixelCount; i=i+3) {
        strip.SetPixelColor(i+q, RgbColor(0,0,0));        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    red = 255 - WheelPos * 3;
    green = 0;
    blue = WheelPos * 3;
    //return (255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if(WheelPos < 170) {
    //WheelPos -= 85;
    red = 0;
    green = (WheelPos - 85) * 3;
    blue = 255 - (WheelPos - 85) * 3;
    //return (0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
  WheelPos -= 170;
  red = WheelPos * 3;
  green = 255 - WheelPos * 3;
  blue = 0;
  }
   //return (WheelPos * 3, 255 - WheelPos * 3, 0);
}

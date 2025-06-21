/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE
  
  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel
  
  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look. 
  
  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide
  
  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/
#include "src/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#include "WS2812_Definitions.h"

#define PIN 2 //D1 //2
#define LED_COUNT 9 //7 //36

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int sine[] = {7,6,5,4,3,2,1,20,21,22,23,12,13,31,30,29,28,27,26,25,24,35,34,33,32,14,15,16,17,18,19,0,11,10,9,8};
int sinecount = 36;
int choice = 0;
unsigned long colorarray[] = {WHITE, RED, GREEN};


void setup()
{
  leds.begin();  // Call this to start up the LED strip.
  leds.setBrightness(20); //adjust brightness here
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop()
{

  choice = random(11);

  switch(choice)
  {
    case 1:
      infinitysequence2(RED);
      break;

    case 2:
      reef();
      break;

    case 3:
      randomsequence();
      break;

    case 4:
      colorsequence(GREEN);
      break;

    case 5:
      rgwsequence();
      break;
      
    case 6:
      colorsequence(WHITE);
      break;

    case 7:
      for (int i=0; i<LED_COUNT*5; i++)
      {
        rainbow(i);
        delay(50);  // Delay between rainbow slides
      }
      clearLEDs();
      leds.show();
      break;

    case 8:
      infinitysequence2(GREEN);
      break;

    case 9:
      colorsequence(RED);
      break;

    case 10:
      rollingwheels();
      break;

    default:
      infinitysequence2(WHITE);
      break;
  
    
  }

  clearLEDs();
  leds.show();
}




// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position. 
// ROY G BIV!
void rainbow(byte startPosition) 
{
  // Need to scale our rainbow. We want a variety of colors, even if there
  // are just 10 or so pixels.
  int rainbowScale = 192 / LED_COUNT;
  
  // Next we setup each pixel with the right color
  for (int i=0; i<LED_COUNT; i++)
  {
    // There are 192 total colors we can get out of the rainbowOrder function.
    // It'll return a color between red->orange->green->...->violet for 0-191.
    leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
  }
  // Finally, actually turn the LEDs on:
  leds.show();
}

void reef()
{
  for(int i=0;i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, GREEN);
    leds.show();
  }

  for(int i=0;i<LED_COUNT; i=i+12)
  {
    leds.setPixelColor(i, RED);
    leds.setPixelColor(i+11, RED);
    leds.show();
  }
  leds.show();
  //delay(10000);
  clearLEDs();
  leds.show();
  
}


void rgwsequence()
{

  int pause = 200;

  for(int j=0;j<20;j++)
  {
    
   for(int i=0;i<(LED_COUNT);i=i+3)
   {
      leds.setPixelColor(i, RED);
      leds.setPixelColor(i+1, GREEN);
      leds.setPixelColor(i+2, WHITE);
      leds.show();
   }
   delay(pause);
   for(int i=0;i<(LED_COUNT);i=i+3)
   {
      leds.setPixelColor(i, WHITE);
      leds.setPixelColor(i+1, RED);
      leds.setPixelColor(i+2, GREEN);
      leds.show();
   }
   delay(pause);
   for(int i=0;i<(LED_COUNT);i=i+3)
   {
      leds.setPixelColor(i, GREEN);
      leds.setPixelColor(i+1, WHITE);
      leds.setPixelColor(i+2, RED);
      leds.show();
   }
   delay(pause);
  } 

  clearLEDs();
  leds.show();
  
}

void infinitysequence2(unsigned long inColor)
{
  int pause = 50;

  for (int j=0;j<4;j++)
  {
    for(int i=0;i<sinecount;i++)
    {

      switch(i)
      {
        case 0:
          if (j == 0)
          {
            leds.setPixelColor(sine[i], inColor);
            leds.show();
            delay(pause);
          }
          else
          {
            leds.setPixelColor(sine[i], inColor);
            leds.setPixelColor(sine[sinecount-3],0);
            leds.show();
            delay(pause);
           }           
        break;
        case 1:
          if (j == 0)
          {
            leds.setPixelColor(sine[i], inColor);
            leds.show();
            delay(pause);
          }
          else
          {
            leds.setPixelColor(sine[i], inColor);
            leds.setPixelColor(sine[sinecount-2],0);
            leds.show();
            delay(pause);
          }
        break;
        case 2:
          if (j == 0)
          {
            leds.setPixelColor(sine[i], inColor);
            leds.show();
            delay(pause);
          }
          else
          {
            leds.setPixelColor(sine[i], inColor);
            leds.setPixelColor(sine[sinecount-1],0);
            leds.show();
            delay(pause);
          }

        default:
            leds.setPixelColor(sine[i], inColor);
            leds.setPixelColor(sine[i-3],0);
            leds.show();
            delay(pause);
        break;
        
      }
    }
  }
clearLEDs();
leds.show();
  
}



void randomsequence()
{
  for(int i=0;i<sinecount*4;i++)
  {
    int aa = random(sinecount);
    leds.setPixelColor(aa, GREEN);
    leds.show();
    delay(50);
    int bb = random(sinecount);
    leds.setPixelColor(bb, RED);
    leds.show();
    delay(50);
    int cc = random(sinecount);
    leds.setPixelColor(cc, WHITE);
    leds.show();
    delay(50);
  }
  clearLEDs();
  leds.show();
}

void rollingwheels()  //assumes that you are using 12 LED ring neopixels
{
  int pause = 100;
  
  for(int k=0; k<96; k++)
  {
    
    for(int j=0; j<LED_COUNT/4; j++)
    {
      for(int i=0; i<4; i++)
      {
        leds.setPixelColor(((j*4)+i+k)%LED_COUNT, colorarray[(j%3)]);
        leds.show();
      }
    }  

  delay(pause);  
  
  }
  
  clearLEDs();
  leds.show();  
}


void colorsequence(unsigned long inColor)
{
   int pause = 50;
    
   for(int i=0; i<sinecount;i++)
   {
      leds.setPixelColor(sine[i], inColor);
      leds.show();
      delay(pause);
   }

  for(int i=sinecount;i>0;i--)
   {
      leds.setPixelColor(sine[i], 0);
      leds.show();
      delay(pause);
   }

   for(int i=0; i<sinecount;i++)
   {
      leds.setPixelColor(sine[i], inColor);
      leds.show();
      delay(pause);
   }

  for (int i=0; i<10; i++)
  {
        clearLEDs();
        leds.show();
        delay(100);
        for(int i=0; i<LED_COUNT; i++)
        {
          leds.setPixelColor(i, inColor);
          leds.show();
        }
        delay(100);
  }
  clearLEDs();
  leds.show();
}

// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position) 
{
  // 6 total zones of color change:
  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
}

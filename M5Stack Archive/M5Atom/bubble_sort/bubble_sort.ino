#include "src/FastLED/FastLED.h"

//#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "math.h"

// Which pin on the Arduino is connected to the NeoLEDS?
#define NUM_LEDS 25
#define DATA_PIN 27
CRGB leds[NUM_LEDS];

int r, g, b;


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100); //init led strip 
}

void loop() {
  bubbleSort(); //call bubbleSort function
  while(1); //exit(0); //stop loop
}

void bubbleSort()
{ 
  int arr[NUM_LEDS]; //init an array with 30 elements
  for (int j = 0; j < NUM_LEDS; j++) 
  {
    int i = random(NUM_LEDS);
    arr[j] = i;
    setColor(i, j);
  }
  /* 
  give every led a randnom number between (0,30). The number will be used to generate a unique color to represent the value of the number
  */

  for (int m = 0; m < NUM_LEDS - 1; m++) {
    for (int n = 0; n < NUM_LEDS - 1 - m; n++) {
      if (arr[n] > arr[n + 1]) {
        setColor(arr[n + 1], n); // swap the larger number and smaller number
        setColor(arr[n], n + 1);
        int temp = arr[n + 1];
        arr[n + 1] = arr[n];
        arr[n] = temp;
      }
      // bubble sort algorithm
    }
  }
}
void setColor(int color, int led) { 
  // use an int number between (0,30) to determine the color of the specified led 
  if (color < NUM_LEDS / 3) {
    r = 255;
    g = floor(250 * 3 * color / NUM_LEDS);
    b = 0;
  } else if (color < NUM_LEDS / 2) {
    r = floor(750 - color * (250 * 6 / NUM_LEDS));
    g = 255;
    b = 0;
  } else if (color < NUM_LEDS * 2 / 3) {
    r = 0;
    g = 255;
    b = floor(color * (250 * 6 / NUM_LEDS) - 750);
  } else if (color < NUM_LEDS * 5 / 6) {
    r = 0;
    g = floor(1250 - color * (250 * 6 / NUM_LEDS));
    b = 255;
  } else {
    r = floor(150 * color * (6 / NUM_LEDS) - 750);
    g = 0;
    b = 255;
  } 
  leds[led].setRGB(g, r, b);
  FastLED.show(); // This sends the updated piaXel color to the hardware.
  delay(75);
}

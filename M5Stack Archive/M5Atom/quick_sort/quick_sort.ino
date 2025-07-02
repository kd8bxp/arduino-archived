#include "src/FastLED/FastLED.h"

//#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "math.h"

// Which pin on the Arduino is connected to the NeoLEDS?
// On a Trinket or Gemma we suggest changing this to 1
#define NUM_LEDS 25
#define DATA_PIN 27
CRGB leds[NUM_LEDS];

int r, g, b;

void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(100); //init the led strip with 100 brightness
}

void loop()
{
  int arr[NUM_LEDS];
  for (int j = 0; j < NUM_LEDS; j++)
  {
    int i = random(NUM_LEDS);
    arr[j] = i;
    setColor(i, j);
  } //set the led strip with 30 random unique color representing different values
  quickSort(arr, 0, 24); // call quicksort function
     while(1); // exit(0);
}

void quickSort(int num[], int l, int r)
{
  if (l >= r) 
    return;
  int m = l, n = r, x = num[l];
  while (m < n)
  {
    while (m < n && num[n] >= x) 
      n--;
    if (m < n)
    {
      setColor(num[n], m);
      num[m++] = num[n]; 
    }

    while (m < n && num[m] <= x) 
      m++;
    if (m < n)
    {
      setColor(num[m], n);
      num[n--] = num[m];
    }
  }
  num[m] = x;               //put the inital x into m element
  setColor(x, m);
  quickSort(num, l, m - 1); //use m as mid value, use recursive call
  quickSort(num, m + 1, r);
}
void setColor(int color, int led)
{
  if (color < NUM_LEDS / 3)
  {
    r = 255;
    g = floor(250 * 3 * color / NUM_LEDS);
    b = 0;
  }
  else if (color < NUM_LEDS / 2)
  {
    r = floor(750 - color * (250 * 6 / NUM_LEDS));
    g = 255;
    b = 0;
  }
  else if (color < NUM_LEDS * 2 / 3)
  {
    r = 0;
    g = 255;
    b = floor(color * (250 * 6 / NUM_LEDS) - 750);
  }
  else if (color < NUM_LEDS * 5 / 6)
  {
    r = 0;
    g = floor(1250 - color * (250 * 6 / NUM_LEDS));
    b = 255;
  }
  else
  {
    r = floor(150 * color * (6 / NUM_LEDS) - 750);
    g = 0;
    b = 255;
  }
  // convert an int value to rgb array
  leds[led].setRGB(g, r, b);
  FastLED.show(); // This sends the updated piaXel color to the hardware.
  delay(75);
}

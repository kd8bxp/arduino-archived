#include <Adafruit_NeoPixel.h>
#define PIN 6
int pixels=26;
int wait = 600; //delay for pixel display

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixels, PIN, NEO_GRB + NEO_KHZ800);

uint32_t colorArray[26] = {
  strip.Color(105,105,105), //A - white
  strip.Color(0, 0, 105),   //B - blue
  strip.Color(105, 0, 0),   //C - red
  strip.Color(0, 80, 105),     //D - light blue
  strip.Color(0, 0, 105),      //E - blue
  strip.Color(105, 105, 25),   //F - yellow
  strip.Color(105, 0, 0),      //G - red
  strip.Color(0, 0, 105),      //H - blue
  strip.Color(0, 0, 105),      //I - blue
  strip.Color(105, 0, 0),      //J - red
  strip.Color(0, 0, 105),     //K - blue
  strip.Color(105, 105, 105), //L - white
  strip.Color(105, 105, 25),  //M - yellow
  strip.Color(105, 0, 0),     //N - red
  strip.Color(105, 0, 0),     //O - red
  strip.Color(0, 80, 105),    //P - light blue
  strip.Color(105, 0, 0),     //Q - red
  strip.Color(0, 80, 105),    //R - light blue
  strip.Color(105, 105, 105), //S - white
  strip.Color(105, 105, 25),  //T - yellow
  strip.Color(0, 0, 105),     //U - blue
  strip.Color(105, 0, 0),     //V - red
  strip.Color(0, 0, 105),     //W - blue
  strip.Color(105, 105, 25),  //X - yellow
  strip.Color(105, 0, 0),     //Y - red
  strip.Color(105, 0, 0),     //Z - red
};

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
  for (int i=0; i<=pixels; i++) {
    strip.setPixelColor(i, colorArray[i]);
  }
  strip.show();
  delay(1000);
  display('R');
  display('I');
  display('G');
  display('H');
  display('T');
  delay(wait * 2);
  display('H');
  display('E');
  display('R');
  display('E');
}

void loop() {
  
  for (int i=0; i<=pixels; i++) {
    strip.setPixelColor(i, colorArray[i]);
  }
  strip.show();

  delay(1000);
  display('R');
  display('I');
  display('G');
  display('H');
  display('T');
  delay(wait * 2);
  display('H');
  display('E');
  display('R');
  display('E');
  
}

void clearDisplay() {
  for (int i=0; i<=pixels; i++) {
    strip.setPixelColor(i, 0,0,0);
  }
  strip.show();
  }

void display(int letter) {
  clearDisplay();
  Serial.write(letter);
  Serial.print(" ");
  Serial.print(letter - 65);
  Serial.print(" ");
  Serial.println(letter);
  
  strip.setPixelColor(letter - 65, colorArray[letter - 65]);
  strip.show();
  delay(wait);
  strip.setPixelColor(letter - 65, 0,0,0);
  strip.show();
}


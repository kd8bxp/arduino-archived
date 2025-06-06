// Simple I2C test for ebay 128x64 oled.
// Use smaller faster AvrI2c class in place of Wire.
// Edit AVRI2C_FASTMODE in SSD1306Ascii.h to change the default I2C frequency.
//
#include "src/SSD1306Ascii/SSD1306Ascii.h"
#include "src/SSD1306Ascii/SSD1306AsciiWire.h"
#include <Wire.h>

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;
//------------------------------------------------------------------------------
void setup() {
Wire.begin(D1, D2);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();
  oled.println("Hello world!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);
}
//------------------------------------------------------------------------------
void loop() {}

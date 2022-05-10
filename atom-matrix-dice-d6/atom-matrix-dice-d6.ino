#include "src/M5Atom/M5Atom.h"

// The colour of the dots which show the number.
const CRGB g_foregroundColour{ 0xffffff };

// The colour of all the other pixels.
const CRGB g_backgroundColour{ 0x003300 };

// Get a random number between 1 and 6 (inclusive).
uint8_t getRandomDiceRoll()
{
  // Any disconnected GPIO pin should work here:
  return (analogRead(32) % 6) + 1;
}

// Display the specified dice number on the matrix, in the specified colour.
void drawNumber(const uint8_t number, const CRGB colour)
{
  switch (number)
  {
  case 1:
    M5.dis.drawpix(12, colour);
    break;
  
  case 2:
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(16, colour);
    break;

  case 3:
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(12, colour);
    M5.dis.drawpix(16, colour);
    break;

  case 4:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;

  case 5:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(12, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;

  case 6:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(11, colour);
    M5.dis.drawpix(13, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;
  }
}

void setup()
{
  // Enable the matrix display, and ensure it's initially empty.
  M5.begin(false, false, true);
  M5.dis.clear();
}

void loop()
{
  // Clear the matrix display when the button is pushed down.
  if (M5.Btn.wasPressed())
  {
    M5.dis.clear();
  }

  // Show a random number when the button is released.
  if (M5.Btn.wasReleased())
  {
    for (uint8_t i{ 0 }; i < 25; ++i)
      M5.dis.drawpix(i, g_backgroundColour);
    
    drawNumber(getRandomDiceRoll(), g_foregroundColour);
  }
  
  delay(25);
  M5.update();
}

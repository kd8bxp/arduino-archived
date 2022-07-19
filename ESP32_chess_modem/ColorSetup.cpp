#include "rgb_lcd.h"
#include "ValueSetup.h"
#include "ColorSetup.h"

ColorSetup::ColorSetup(String initial, rgb_lcd lcd) : ValueSetup<String>("Colour", initial, lcd) {}

void ColorSetup::onPressRight()
{
  _value = "white";
}

void ColorSetup::onPressLeft()
{
  _value = "black";
}

void ColorSetup::printInitialMessage()
{
  printMessage("Choose Colour:", "[>]WHITE[<]BLACK");
}
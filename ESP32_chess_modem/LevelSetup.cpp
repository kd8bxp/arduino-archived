#include "rgb_lcd.h"
#include "ValueSetup.h"
#include "LevelSetup.h"

LevelSetup::LevelSetup(int initial, rgb_lcd lcd) : ValueSetup<int>("Level", initial, lcd) {}

void LevelSetup::onPressLeft()
{
  int newValue = _value - 1;
  if (newValue >= 0)
    _value = newValue;
}

void LevelSetup::onPressRight()
{
  int newValue = _value + 1;
  if (newValue <= 15)
    _value = newValue;
}

void LevelSetup::printInitialMessage()
{
  printMessage("Set Level:", "1 to 15");
}
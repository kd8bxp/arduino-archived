#ifndef ColorSetup_h
#define ColorSetup_h

#include "rgb_lcd.h"
#include "ValueSetup.h"

class ColorSetup : public ValueSetup<String>
{
public:
  ColorSetup(String initial, rgb_lcd lcd);

private:
  void onPressLeft();
  void onPressRight();
  void printInitialMessage();
};

#endif
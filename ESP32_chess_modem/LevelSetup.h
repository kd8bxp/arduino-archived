#ifndef LevelSetup_h
#define LevelSetup_h

#include "rgb_lcd.h"
#include "ValueSetup.h"

class LevelSetup : public ValueSetup<int>
{
public:
  LevelSetup(int initial, rgb_lcd lcd);

private:
  void onPressLeft();
  void onPressRight();
  void printInitialMessage();
};

#endif
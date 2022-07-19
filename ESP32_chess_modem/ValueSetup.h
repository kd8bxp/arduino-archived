#ifndef ValueSetup_h
#define ValueSetup_h

#include "Arduino.h"
#include "rgb_lcd.h"

template <typename T>
class ValueSetup
{
public:
  ValueSetup(String valueName, T initial, rgb_lcd lcd);
  T doSetup();
  void setButtonLeft(String buttonLeft);
  void setButtonRight(String buttonRight);
  void SetButtonOK(String buttonOK);

protected:
  T _value;
  void printMessage(String first_part, String second_part);
  virtual void onPressLeft() = 0;
  virtual void onPressRight() = 0;
  virtual void printInitialMessage() = 0;

private:
  rgb_lcd _lcd;
  String _valueName;
  String _buttonLeft = "<";
  String _buttonRight = ">";
  String _buttonOK = "OK";
  void printValueSetToMessage();
  void printSetValueMessage();
};

#endif
#include "Arduino.h"
#include "rgb_lcd.h"
#include "ValueSetup.h"

template <typename T>
ValueSetup<T>::ValueSetup(String valueName, T initial, rgb_lcd lcd)
{
  _valueName = valueName;
  _value = initial;
  _lcd = lcd;
}

template <typename T>
T ValueSetup<T>::doSetup()
{
  printInitialMessage();
  while (true)
  {
    if (Serial.available() > 0)
    {
      String inputString = "";
      inputString = Serial.readStringUntil('\n');
      inputString.trim();

      if (inputString == _buttonRight)
      {
        onPressRight();
        printSetValueMessage();
      }
      else if (inputString == _buttonLeft)
      {
        onPressLeft();
        printSetValueMessage();
      }
      else if (inputString == _buttonOK)
      {
        printValueSetToMessage();
        return _value;
      }

      Serial.flush();
    }
  }
}

template <typename T>
void ValueSetup<T>::setButtonLeft(String buttonLeft)
{
  _buttonLeft = buttonLeft;
}

template <typename T>
void ValueSetup<T>::setButtonRight(String buttonRight)
{
  _buttonRight = buttonRight;
}

template <typename T>
void ValueSetup<T>::SetButtonOK(String buttonOK)
{
  _buttonOK = buttonOK;
}

template <typename T>
void ValueSetup<T>::printSetValueMessage()
{
  printMessage("Set " + _valueName + ":", String(_value));
}

template <typename T>
void ValueSetup<T>::printValueSetToMessage()
{
  printMessage(_valueName + " Set to:", _valueName + ":" + String(_value));
}

template <typename T>
void ValueSetup<T>::printMessage(String first_part, String second_part)
{
  _lcd.clear();
  _lcd.print(first_part);
  _lcd.setCursor(0, 1);
  _lcd.print(second_part);
}

//Need to declare the types, used in project, here because of C++ Linker Error
template class ValueSetup<int>;
template class ValueSetup<String>;
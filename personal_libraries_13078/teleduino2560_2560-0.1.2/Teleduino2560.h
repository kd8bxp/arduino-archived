/*
  Teleduino2560.h - Teleduino2560 library
  Version 2560-0.1.2
  Nathan Kennedy 2009 - 2014
  http://www.teleduino.org

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef Teleduino2560_h
#define Teleduino2560_h

#define TD2560_PINS 70
#define TD2560_SERVO_INSTANCES 48
#define TD2560_SHIFT_REGISTERS 4
#define TD2560_SHIFT_REGISTER_TIMERS 16

#include &lt;Arduino.h&gt;
#include &lt;EEPROM.h&gt;
#include &lt;Servo.h&gt;
#include &lt;Wire.h&gt;

// Used for Teleduino2560Class::getFreeMemory()
extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

typedef struct
{
  byte mode;
  byte expireAction;
  unsigned long expireTime;
} Pin;

typedef struct
{
  Servo servo;
  byte position;
} ServoInstance;

typedef struct
{
  byte clockPin;
  byte dataPin;
  byte enablePin;
  byte latchPin;
  byte outputs[32];
} ShiftRegister;

typedef struct
{
  byte shiftRegister;
  byte action;
  unsigned long expireTime;
  byte outputs[32];
} ShiftRegisterTimer;

class Teleduino2560Class
{
  protected:
    boolean _reset;
    Pin _pins[TD2560_PINS];
    ServoInstance _servoInstances[TD2560_SERVO_INSTANCES];
    ShiftRegister _shiftRegisters[TD2560_SHIFT_REGISTERS];
    ShiftRegisterTimer _shiftRegisterTimers[TD2560_SHIFT_REGISTER_TIMERS];
    unsigned int _serialBauds[10];
    byte _statusLedPin;
    char _version[17];
  boolean _wireDefined;
  public:
    Teleduino2560Class();
    boolean checkAnalogPin(byte);
    boolean checkDigitalPin(byte);
    boolean checkPwmPin(byte);
    void checkReset();
    boolean definePinMode(byte, byte);
    int getFreeMemory();
    byte hexDecode(byte);
    byte hexEncode(byte, boolean);
    byte hexEncode(byte);
    void instruction(byte*);
    void loadPresets();
    void pinTimers();
    void reset();
    boolean setDigitalOutput(byte, byte, unsigned int, byte);
    boolean setDigitalOutput(byte, byte, unsigned int);
    boolean setDigitalOutput(byte, byte);
    void setStatusLed(unsigned int, boolean, long);
    void setStatusLed(unsigned int, boolean);
    void setStatusLed(unsigned int, long);
    void setStatusLed(unsigned int);
    boolean setStatusLedPin(byte);
    void shiftRegisters();
    void shiftRegisterTimers();
};

extern Teleduino2560Class Teleduino2560;

#endif

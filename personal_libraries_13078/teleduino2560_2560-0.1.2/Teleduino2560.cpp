/*
  Teleduino2560.cpp - Teleduino2560 library
  Version 2560-0.1.2
  Nathan Kennedy 2009 - 2014
  http://www.teleduino.org

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include &quot;Teleduino2560.h&quot;

Teleduino2560Class::Teleduino2560Class()
{
  _reset = false;
  for(byte i = 0; i &lt; 100; i++)
  {
    if(i &lt; TD2560_PINS)
    {
      _pins[i].mode = 255;
      _pins[i].expireAction = 255;
      _pins[i].expireTime = 0;
    }
    if(i &lt; TD2560_SERVO_INSTANCES)
    {
      _servoInstances[i].position = 255;
    }
    if(i &lt; TD2560_SHIFT_REGISTERS)
    {
      _shiftRegisters[i].clockPin = 255;
      _shiftRegisters[i].dataPin = 255;
      _shiftRegisters[i].enablePin = 255;
      _shiftRegisters[i].latchPin = 255;
      for(byte j = 0; j &lt; 32; j++)
      {
        _shiftRegisters[i].outputs[j] = 0;
      }
    }
    if(i &lt; TD2560_SHIFT_REGISTER_TIMERS)
    {
      _shiftRegisterTimers[i].shiftRegister = 255;
      _shiftRegisterTimers[i].action = 255;
      _shiftRegisterTimers[i].expireTime = 0;
      for(byte j = 0; j &lt; 32; j++)
      {
        _shiftRegisterTimers[i].outputs[j] = 0;
      }
    }
  }
  _serialBauds[0] = 300;
  _serialBauds[1] = 1200;
  _serialBauds[2] = 2400;
  _serialBauds[3] = 4800;
  _serialBauds[4] = 9600;
  _serialBauds[5] = 14400;
  _serialBauds[6] = 19200;
  _serialBauds[7] = 28800;
  _serialBauds[8] = 38400;
  _serialBauds[9] = 57600;
  _statusLedPin = 255;
  strcpy(_version, &quot;2560-0.1.2&quot;);
  _wireDefined = false;
}

boolean Teleduino2560Class::checkAnalogPin(byte pin)
{
  if(pin &gt;= 54 &amp;&amp; pin &lt;= 69)
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean Teleduino2560Class::checkDigitalPin(byte pin)
{
  if(pin &gt;= 0 &amp;&amp; pin &lt;= 69)
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean Teleduino2560Class::checkPwmPin(byte pin)
{
  if((pin &gt;= 2 &amp;&amp; pin &lt;= 13) || (pin &gt;= 44 &amp;&amp; pin &lt;= 46))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Teleduino2560Class::checkReset()
{
  if(_reset)
  {
    reset();
  }
}

boolean Teleduino2560Class::definePinMode(byte pin, byte mode)
{
  if(checkDigitalPin(pin))
  {
    _pins[pin].mode = mode;
    pinMode(pin, mode);
    return true;
  }
  else
  {
    return false;
  }
}

int Teleduino2560Class::getFreeMemory()
{
  int freeMemory;
  if((int)__brkval == 0)
  {
    freeMemory = ((int)&amp;freeMemory) - ((int)&amp;__bss_end);
  }
  else
  {
    freeMemory = ((int)&amp;freeMemory) - ((int)__brkval);
  }
  return freeMemory;
}

byte Teleduino2560Class::hexDecode(byte c)
{
  if(c &gt;= '0' &amp;&amp; c &lt;= '9')
  {
    return c - '0';
  }
  else if(c &gt;= 'a' &amp;&amp; c &lt;= 'f')
  {
    return c - 'a' + 10;
  }
  else if(c &gt;= 'A' &amp;&amp; c &lt;= 'F')
  {
    return c - 'A' + 10;
  }
  else
  {
    return 0;
  }
}

byte Teleduino2560Class::hexEncode(byte n, boolean cap)
{
  if(n &gt;= 0 &amp;&amp; n &lt;= 9)
  {
    return n + '0';
  }
  else if(n &gt;= 10 &amp;&amp; n &lt;= 15)
  {
    if(cap)
    {
      return n - 10 + 'A';
    }
    else
    {
      return n - 10 + 'a';
    }
  }
  else
  {
    return '0';
  }
}

byte Teleduino2560Class::hexEncode(byte n)
{
  return hexEncode(n, true);
}

void Teleduino2560Class::instruction(byte* buffer)
{
  switch(buffer[0])
  {
    case 0x10: // reset
      if(buffer[1] == 0)
      {
        _reset = true;
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x11: // getVersion
      if(buffer[1] == 0)
      {
        buffer[0] = 1;
        byte length;
        for(length = 0; length &lt; 16 &amp;&amp; _version[length] &gt; 0; length++)
        {
          buffer[2 + length] = _version[length];
        }
        buffer[1] = length;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x12: // setStatusLedPin
      if(buffer[1] == 1 &amp;&amp;
      setStatusLedPin(buffer[2]))
      {
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x13: // setStatusLed
      if(buffer[1] == 1)
      {
        setStatusLed((unsigned int)buffer[2]);
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x14: // getFreeMemory
      if(buffer[1] == 0)
      {
        int value = getFreeMemory();
        buffer[0] = 1;
        buffer[1] = 2;
        buffer[2] = value / 256;
        buffer[3] = value % 256;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x15: // ping
      if(buffer[1] == 0)
      {
        if(_statusLedPin &gt; -1)
        {
          setDigitalOutput(_statusLedPin, HIGH);
          delay(10);
          setDigitalOutput(_statusLedPin, LOW);
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x16: // getUptime
      if(buffer[1] == 0)
      {
        unsigned long value = millis();
        buffer[0] = 1;
        buffer[1] = 4;
        buffer[2] = value / 16777216;
        buffer[3] = (value % 16777216) / 65536;
        buffer[4] = ((value % 16777216) % 65536) / 256;
        buffer[5] = ((value % 16777216) % 65536) % 256;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x17: // loadPresets
      if(buffer[1] == 0)
      {
        loadPresets();
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x20: // definePinMode
      if(buffer[1] == 2 &amp;&amp;
      checkDigitalPin(buffer[2]) &amp;&amp;
      buffer[3] &gt;= 0 &amp;&amp; buffer[3] &lt;= 1)
      {
        definePinMode(buffer[2], buffer[3]);
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x21: // setDigitalOutput
      if((buffer[1] == 2 || buffer[1] == 5 || buffer[1] == 6) &amp;&amp;
      checkDigitalPin(buffer[2]) &amp;&amp;
      buffer[3] &gt;= 0 &amp;&amp; buffer[3] &lt;= 2)
      {
        unsigned int time = 0;
        byte save = 0;
        if(buffer[1] == 5 || buffer[1] == 6)
        {
          time = ((unsigned long)buffer[4] * 65536) + ((unsigned long)buffer[5] * 256) + (unsigned long)buffer[6];
          if(buffer[1] == 6 &amp;&amp; buffer[7] == 1)
          {
            save = 1;
          }
        }
        if(buffer[3] == 2)
        {
          buffer[0] = setDigitalOutput(buffer[2], !digitalRead(buffer[2]), time, save);
        }
        else
        {
          buffer[0] = setDigitalOutput(buffer[2], buffer[3], time, save);
        }
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x22: // setPwmOutput
      if(buffer[1] == 2 &amp;&amp;
      checkPwmPin(buffer[2]))
      {
        analogWrite(buffer[2], buffer[3]);
        _pins[buffer[2]].expireAction = 255;
        _pins[buffer[2]].expireTime = 0;
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x23: // getDigitalInput
      if(buffer[1] == 1 &amp;&amp;
      checkDigitalPin(buffer[2]))
      {
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = digitalRead(buffer[2]);
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x24: // getAnalogInput
      if(buffer[1] == 1 &amp;&amp;
      checkAnalogPin(buffer[2]))
      {
        analogRead(buffer[2] - 54);
        delay(10);
        int value = analogRead(buffer[2] - 54);
        buffer[0] = 1;
        buffer[1] = 2;
        buffer[2] = value / 256;
        buffer[3] = value % 256;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x25: // getAllInputs
      if(buffer[1] == 0)
      {
        buffer[0] = 1;
        buffer[1] = 86;
        for(byte i = 0; i &lt; 54; i++)
        {
          buffer[2 + i] = digitalRead(i);
        }
        for(byte i = 0; i &lt; 16; i++)
        {
          analogRead(i);
          delay(10);
          int value = analogRead(i);
          buffer[56 + (i * 2)] = value / 256;
          buffer[56 + (i * 2) + 1] = value % 256;
        }
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x26: // setDigitalOutputs
      if(buffer[1] &gt;= 5 &amp;&amp; !((buffer[1] - 1) % 4) &amp;&amp;
      buffer[2] &lt; TD2560_PINS)
      {
        byte max = (buffer[1] - 1) / 4 &lt; TD2560_PINS - buffer[2] ? (buffer[1] - 1) / 4 : TD2560_PINS - buffer[2];
        for(byte i = 0; i &lt; max; i++)
        {
          if(buffer[(i * 4) + 3] &lt;= 2)
          {
            unsigned long time = ((unsigned long)buffer[(i * 4) + 4] * 65536) + ((unsigned long)buffer[(i * 4) + 5] * 256) + (unsigned long)buffer[(i * 4) + 6];
            if(buffer[(i * 4) + 3] == 2)
            {
              setDigitalOutput(buffer[2] + i, !digitalRead(buffer[2] + i), time);
            }
            else
            {
              setDigitalOutput(buffer[2] + i, buffer[(i * 4) + 3], time);
            }
          }
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x30: // defineShiftRegister
      if(buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SHIFT_REGISTERS &amp;&amp;
      checkDigitalPin(buffer[3]) &amp;&amp;
      checkDigitalPin(buffer[4]) &amp;&amp;
      checkDigitalPin(buffer[5]) &amp;&amp;
      (buffer[1] == 4 || (buffer[1] == 5 &amp;&amp; checkDigitalPin(buffer[6]))))
      {
        _shiftRegisters[buffer[2]].clockPin = buffer[3];
        definePinMode(_shiftRegisters[buffer[2]].clockPin, OUTPUT);
        _shiftRegisters[buffer[2]].dataPin = buffer[4];
        definePinMode(_shiftRegisters[buffer[2]].dataPin, OUTPUT);
        _shiftRegisters[buffer[2]].latchPin = buffer[5];
        definePinMode(_shiftRegisters[buffer[2]].latchPin, OUTPUT);
        if(buffer[1] == 5)
        {
          _shiftRegisters[buffer[2]].enablePin = buffer[6];
          definePinMode(_shiftRegisters[buffer[2]].enablePin, OUTPUT);
        }
        for(byte i = 0; i &lt; 32; i++)
        {
          _shiftRegisters[buffer[2]].outputs[i] = 0;
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x31: // setShiftRegister
      if(buffer[1] &gt;= 2 &amp;&amp; buffer[1] &lt;= 33 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SHIFT_REGISTERS &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].clockPin) &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].dataPin) &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].latchPin))
      {
        for(byte i = 0; i &lt; 32; i++)
        {
          _shiftRegisters[buffer[2]].outputs[i] = (i &lt; (buffer[1] - 1)) ? buffer[3 + i] : 0;
        }
        for(byte i = 0; i &lt; TD2560_SHIFT_REGISTER_TIMERS; i++)
        {
          if(_shiftRegisterTimers[i].shiftRegister == buffer[2])
          {
            _shiftRegisterTimers[i].shiftRegister = 255;
            _shiftRegisterTimers[i].action = 255;
            _shiftRegisterTimers[i].expireTime = 0;
            for(byte j = 0; j &lt; 32; j++)
            {
              _shiftRegisterTimers[i].outputs[j] = 0;
            }
          }
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x32: // mergeShiftRegister
      if(buffer[1] &gt;= 6 &amp;&amp; buffer[1] &lt;= 37 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SHIFT_REGISTERS &amp;&amp;
      buffer[3] &gt;= 0 &amp;&amp; buffer[3] &lt;= 2 &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].clockPin) &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].dataPin) &amp;&amp;
      checkDigitalPin(_shiftRegisters[buffer[2]].latchPin))
      {
        boolean proceed = true;
        unsigned long time = ((unsigned long)buffer[4] * 65536) + ((unsigned long)buffer[5] * 256) + (unsigned long)buffer[6];
        byte outputs[32];
        for(byte i = 0; i &lt; 32; i++)
        {
          outputs[i] = (i &lt; (buffer[1] - 5)) ? buffer[7 + i] : 0;
        }
        if(time &gt; 0)
        {
          proceed = false;
          for(byte i = 0; i &lt; TD2560_SHIFT_REGISTER_TIMERS; i++)
          {
            if(_shiftRegisterTimers[i].shiftRegister == 255)
            {
              proceed = true;
            }
          }
        }
        if(proceed)
        {
          for(byte i = 0; i &lt; 32; i++)
          {
            for(byte j = 0; j &lt; 8; j++)
            {
              byte currentBit = 1;
              currentBit = currentBit &lt;&lt; j;
              if((outputs[i] &amp; currentBit) &gt; 0)
              {
                switch(buffer[3])
                {
                  case 0:
                    if((_shiftRegisters[buffer[2]].outputs[i] &amp; currentBit) &gt; 0)
                    {
                      _shiftRegisters[buffer[2]].outputs[i] = _shiftRegisters[buffer[2]].outputs[i] - currentBit;
                    }
                    break;
                  case 1:
                    if((_shiftRegisters[buffer[2]].outputs[i] &amp; currentBit) == 0)
                    {
                      _shiftRegisters[buffer[2]].outputs[i] = _shiftRegisters[buffer[2]].outputs[i] + currentBit;
                    }
                    break;
                  case 2:
                    if((_shiftRegisters[buffer[2]].outputs[i] &amp; currentBit) &gt; 0)
                    {
                      _shiftRegisters[buffer[2]].outputs[i] = _shiftRegisters[buffer[2]].outputs[i] - currentBit;
                    }
                    else if((_shiftRegisters[buffer[2]].outputs[i] &amp; currentBit) == 0)
                    {
                      _shiftRegisters[buffer[2]].outputs[i] = _shiftRegisters[buffer[2]].outputs[i] + currentBit;
                    }
                    break;
                }
              }
            }
            for(byte j = 0; j &lt; TD2560_SHIFT_REGISTER_TIMERS; j++)
            {
              if(_shiftRegisterTimers[j].shiftRegister == buffer[2])
              {
                _shiftRegisterTimers[j].outputs[i] = _shiftRegisterTimers[j].outputs[i] - (_shiftRegisterTimers[j].outputs[i] &amp; outputs[i]);
              }
            }
          }
          if(time &gt; 0)
          {
            unsigned long expireTime = millis() + time;
            if(expireTime &lt; 1)
            {
              expireTime = 1;
            }
            for(byte i = 0; i &lt; TD2560_SHIFT_REGISTER_TIMERS; i++)
            {
              if(_shiftRegisterTimers[i].shiftRegister == 255)
              {
                _shiftRegisterTimers[i].shiftRegister = buffer[2];
                switch(buffer[3])
                {
                  case 0:
                    _shiftRegisterTimers[i].action = 1;
                    break;
                  case 1:
                    _shiftRegisterTimers[i].action = 0;
                    break;
                  case 2:
                    _shiftRegisterTimers[i].action = 2;
                    break;
                }
                _shiftRegisterTimers[i].expireTime = expireTime;
                for(byte j = 0; j &lt; 32; j++)
                {
                  _shiftRegisterTimers[i].outputs[j] = outputs[j];
                }
                break;
              }
            }
          }
          buffer[0] = 1;
          buffer[1] = 0;
        }
        else
        {
          buffer[0] = 0;
          buffer[1] = 0;
        }
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x33: // getShiftRegister
      if(buffer[1] == 1 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SHIFT_REGISTERS)
      {
        byte shiftRegister = buffer[2];
        buffer[0] = 1;
        buffer[1] = 32;
        for(byte i = 0; i &lt; 32; i++)
        {
          buffer[2 + i] = _shiftRegisters[shiftRegister].outputs[i];
        }
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x40: // defineSerial
      if(buffer[1] == 2 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt;= 3 &amp;&amp;
      buffer[3] &gt;= 0 &amp;&amp; buffer[3] &lt;= 10)
      {
        switch(buffer[2])
        {
          case 0:
            Serial.begin(buffer[3] == 10 ? 115200 : _serialBauds[buffer[3]]);
            break;
          case 1:
            Serial1.begin(buffer[3] == 10 ? 115200 : _serialBauds[buffer[3]]);
            break;
          case 2:
            Serial2.begin(buffer[3] == 10 ? 115200 : _serialBauds[buffer[3]]);
            break;
          case 3:
            Serial3.begin(buffer[3] == 10 ? 115200 : _serialBauds[buffer[3]]);
            break;
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x41: // setSerial
      if(buffer[1] &gt;= 2 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt;= 3)
      {
        for(byte i = 0; i &lt; (buffer[1] - 1); i++)
        {
          switch(buffer[2])
          {
            case 0:
              Serial.write(buffer[3 + i]);
              break;
            case 1:
              Serial1.write(buffer[3 + i]);
              break;
            case 2:
              Serial2.write(buffer[3 + i]);
              break;
            case 3:
              Serial3.write(buffer[3 + i]);
              break;
          }
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x42: // getSerial
      if(buffer[1] == 2 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt;= 3 &amp;&amp;
      buffer[3] &gt;= 1)
      {
        byte port = buffer[2];
        byte requestedLength = buffer[3];
        byte length;
        buffer[0] = 1;
        switch(port)
        {
          case 0:
            for(length = 0; length &lt; requestedLength &amp;&amp; Serial.available(); length++)
            {
              buffer[2 + length] = Serial.read();
            }
            break;
          case 1:
            for(length = 0; length &lt; requestedLength &amp;&amp; Serial1.available(); length++)
            {
              buffer[2 + length] = Serial1.read();
            }
            break;
          case 2:
            for(length = 0; length &lt; requestedLength &amp;&amp; Serial2.available(); length++)
            {
              buffer[2 + length] = Serial2.read();
            }
            break;
          case 3:
            for(length = 0; length &lt; requestedLength &amp;&amp; Serial3.available(); length++)
            {
              buffer[2 + length] = Serial3.read();
            }
            break;
        }
        buffer[1] = length;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x43: // flushSerial
      if(buffer[1] == 1 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt;= 3)
      {
        switch(buffer[2])
        {
          case 0:
            while(Serial.available())
            {
              Serial.read();
            }
            break;
          case 1:
            while(Serial1.available())
            {
              Serial1.read();
            }
            break;
          case 2:
            while(Serial2.available())
            {
              Serial2.read();
            }
            break;
          case 3:
            while(Serial3.available())
            {
              Serial3.read();
            }
            break;
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x50: // defineServo
      if(buffer[1] == 2 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SERVO_INSTANCES &amp;&amp;
      checkDigitalPin(buffer[3]))
      {
        buffer[0] = 1;
        buffer[1] = 0;
        _servoInstances[buffer[2]].servo.attach(buffer[3]);
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x51: // setServo
      if(buffer[1] == 2 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SERVO_INSTANCES &amp;&amp;
      buffer[3] &gt;= 0 &amp;&amp; buffer[3] &lt;= 180 &amp;&amp;
      _servoInstances[buffer[2]].servo.attached())
      {
        buffer[0] = 1;
        buffer[1] = 0;
        _servoInstances[buffer[2]].position = buffer[3];
        _servoInstances[buffer[2]].servo.write(buffer[3]);
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x52: // getServo
      if(buffer[1] == 1 &amp;&amp;
      buffer[2] &gt;= 0 &amp;&amp; buffer[2] &lt; TD2560_SERVO_INSTANCES &amp;&amp;
      _servoInstances[buffer[2]].servo.attached())
      {
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = _servoInstances[buffer[2]].position;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x60: // resetEeprom
      if(buffer[1] == 1 &amp;&amp;
      buffer[2] &lt;= 3)
      {
        buffer[0] = 1;
        buffer[1] = 0;
        for(int i = 0; i &lt; 1024; i++)
        {
          if(EEPROM.read(((int)buffer[2] * 1024) + i) != 255)
          {
            EEPROM.write(((int)buffer[2] * 1024) + i, 255);
          }
        }
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x61: // setEeprom
      if(buffer[1] &gt;= 3 &amp;&amp;
      buffer[2] &lt;= 15)
      {
        int offset = (buffer[2] * 256) + buffer[3];
        for(byte i = 0; i &lt; (buffer[1] - 2); i++)
        {
          if(EEPROM.read(offset + i) != buffer[4 + i])
          {
            EEPROM.write(offset + i, buffer[4 + i]);
          }
        }
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x62: // getEeprom
      if(buffer[1] == 3 &amp;&amp;
      buffer[2] &lt;= 15 &amp;&amp;
      buffer[4] &gt;= 1)
      {
        int offset = (buffer[2] * 256) + buffer[3];
        buffer[0] = 1;
        buffer[1] = buffer[4];
        for(byte i = 0; i &lt; buffer[1]; i++)
        {
          buffer[2 + i] = EEPROM.read(offset + i);
        }
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x70: // defineWire
      if(buffer[1] == 0)
      {
        _wireDefined = true;
        Wire.begin();
        buffer[0] = 1;
        buffer[1] = 0;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x71: // setWire
      if(buffer[1] &gt;= 2 &amp;&amp;
      buffer[2] &lt;= 127 &amp;&amp;
      _wireDefined)
      {
        Wire.beginTransmission(buffer[2]);
        for(byte i = 0; i &lt; (buffer[1] - 1); i++)
        {
          Wire.write(buffer[3 + i]);
        }
        buffer[0] = 1;
        buffer[1] = 1;
        buffer[2] = Wire.endTransmission();
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    case 0x72: // getWire
      if(buffer[1] == 2 &amp;&amp;
      buffer[2] &lt;= 127 &amp;&amp;
      buffer[3] &gt;= 1 &amp;&amp;
      _wireDefined)
      {
        Wire.requestFrom(buffer[2], buffer[3]);
        byte requestedLength = buffer[3];
        byte length;
        buffer[0] = 1;
        for(length = 0; length &lt; requestedLength &amp;&amp; Wire.available(); length++)
        {
          buffer[2 + length] = Wire.read();
        }
        buffer[1] = length;
      }
      else
      {
        buffer[0] = 0;
        buffer[1] = 0;
      }
      break;
    default:
      buffer[0] = 0;
      buffer[1] = 0;
      break;
  }
}

void Teleduino2560Class::loadPresets()
{
  if(EEPROM.read(0) == '#' &amp;&amp;
  EEPROM.read(1) == 1 &amp;&amp;
  EEPROM.read(2) == '#')
  {
    // Pin presets
    if(EEPROM.read(2) == '#' &amp;&amp;
    EEPROM.read(73) == '#' &amp;&amp;
    EEPROM.read(144) == '#')
    {
      for(byte i = 0; i &lt; 70; i++)
      {
        if(i &lt; TD2560_PINS)
        {
          switch(EEPROM.read(3 + i))
          {
            case 0:
              definePinMode(i, INPUT);
              break;
            case 1:
              definePinMode(i, OUTPUT);
              switch(EEPROM.read(74 + i))
              {
                case 0:
                  setDigitalOutput(i, LOW);
                  break;
                case 1:
                  setDigitalOutput(i, HIGH);
                  break;
              }
              break;
            case 2:
              if(checkPwmPin(i))
              {
                analogWrite(i, EEPROM.read(74 + i));
              }
              break;
          }
        }
      }
    }
    // Shift register presets
    if(EEPROM.read(144) == '#' &amp;&amp;
    EEPROM.read(161) == '#' &amp;&amp;
    EEPROM.read(290) == '#')
    {
      if(TD2560_SHIFT_REGISTERS &gt;= 1)
      {
        if(checkDigitalPin(EEPROM.read(145)) &amp;&amp;
        checkDigitalPin(EEPROM.read(146)) &amp;&amp;
        checkDigitalPin(EEPROM.read(147)))
        {
          _shiftRegisters[0].clockPin = EEPROM.read(145);
          definePinMode(_shiftRegisters[0].clockPin, OUTPUT);
          _shiftRegisters[0].dataPin = EEPROM.read(146);
          definePinMode(_shiftRegisters[0].dataPin, OUTPUT);
          _shiftRegisters[0].latchPin = EEPROM.read(147);
          definePinMode(_shiftRegisters[0].latchPin, OUTPUT);
          if(checkDigitalPin(EEPROM.read(148)))
          {
            _shiftRegisters[0].enablePin = EEPROM.read(148);
            definePinMode(_shiftRegisters[0].enablePin, OUTPUT);
          }
          for(byte i = 0; i &lt; 32; i++)
          {
            _shiftRegisters[0].outputs[i] = EEPROM.read(162 + i);
          }
        }
      }
      if(TD2560_SHIFT_REGISTERS &gt;= 2)
      {
        if(checkDigitalPin(EEPROM.read(149)) &amp;&amp;
        checkDigitalPin(EEPROM.read(150)) &amp;&amp;
        checkDigitalPin(EEPROM.read(151)))
        {
          _shiftRegisters[1].clockPin = EEPROM.read(149);
          definePinMode(_shiftRegisters[1].clockPin, OUTPUT);
          _shiftRegisters[1].dataPin = EEPROM.read(150);
          definePinMode(_shiftRegisters[1].dataPin, OUTPUT);
          _shiftRegisters[1].latchPin = EEPROM.read(151);
          definePinMode(_shiftRegisters[1].latchPin, OUTPUT);
          if(checkDigitalPin(EEPROM.read(152)))
          {
            _shiftRegisters[1].enablePin = EEPROM.read(152);
            definePinMode(_shiftRegisters[1].enablePin, OUTPUT);
          }
          for(byte i = 0; i &lt; 32; i++)
          {
            _shiftRegisters[1].outputs[i] = EEPROM.read(194 + i);
          }
        }
      }
      if(TD2560_SHIFT_REGISTERS &gt;= 3)
      {
        if(checkDigitalPin(EEPROM.read(153)) &amp;&amp;
        checkDigitalPin(EEPROM.read(154)) &amp;&amp;
        checkDigitalPin(EEPROM.read(155)))
        {
          _shiftRegisters[2].clockPin = EEPROM.read(153);
          definePinMode(_shiftRegisters[2].clockPin, OUTPUT);
          _shiftRegisters[2].dataPin = EEPROM.read(154);
          definePinMode(_shiftRegisters[2].dataPin, OUTPUT);
          _shiftRegisters[2].latchPin = EEPROM.read(155);
          definePinMode(_shiftRegisters[2].latchPin, OUTPUT);
          if(checkDigitalPin(EEPROM.read(156)))
          {
            _shiftRegisters[2].enablePin = EEPROM.read(156);
            definePinMode(_shiftRegisters[2].enablePin, OUTPUT);
          }
          for(byte i = 0; i &lt; 32; i++)
          {
            _shiftRegisters[2].outputs[i] = EEPROM.read(226 + i);
          }
        }
      }
      if(TD2560_SHIFT_REGISTERS &gt;= 4)
      {
        if(checkDigitalPin(EEPROM.read(157)) &amp;&amp;
        checkDigitalPin(EEPROM.read(158)) &amp;&amp;
        checkDigitalPin(EEPROM.read(159)))
        {
          _shiftRegisters[3].clockPin = EEPROM.read(157);
          definePinMode(_shiftRegisters[3].clockPin, OUTPUT);
          _shiftRegisters[3].dataPin = EEPROM.read(158);
          definePinMode(_shiftRegisters[3].dataPin, OUTPUT);
          _shiftRegisters[3].latchPin = EEPROM.read(159);
          definePinMode(_shiftRegisters[3].latchPin, OUTPUT);
          if(checkDigitalPin(EEPROM.read(160)))
          {
            _shiftRegisters[1].enablePin = EEPROM.read(160);
            definePinMode(_shiftRegisters[3].enablePin, OUTPUT);
          }
          for(byte i = 0; i &lt; 32; i++)
          {
            _shiftRegisters[3].outputs[i] = EEPROM.read(258 + i);
          }
        }
      }
    }
    // Serial presets
    if(EEPROM.read(290) == '#' &amp;&amp;
    EEPROM.read(295) == '#')
    {
      if(EEPROM.read(291) &lt;= 10)
      {
        Serial.begin(EEPROM.read(291) == 10 ? 115200 : _serialBauds[EEPROM.read(291)]);
      }
      if(EEPROM.read(292) &lt;= 10)
      {
        Serial1.begin(EEPROM.read(292) == 10 ? 115200 : _serialBauds[EEPROM.read(292)]);
      }
      if(EEPROM.read(293) &lt;= 10)
      {
        Serial2.begin(EEPROM.read(293) == 10 ? 115200 : _serialBauds[EEPROM.read(293)]);
      }
      if(EEPROM.read(294) &lt;= 10)
      {
        Serial3.begin(EEPROM.read(294) == 10 ? 115200 : _serialBauds[EEPROM.read(294)]);
      }
    }
    // Servo presets
    if(EEPROM.read(295) == '#' &amp;&amp;
    EEPROM.read(344) == '#' &amp;&amp;
    EEPROM.read(393) == '#')
    {
      for(byte i = 0; i &lt; 48; i++)
      {
        if(i &lt; TD2560_SERVO_INSTANCES)
        {
          if(checkDigitalPin(EEPROM.read(296 + i)))
          {
            _servoInstances[i].servo.attach(EEPROM.read(296 + i));
            if(EEPROM.read(345 + i) &lt;= 180)
            {
              _servoInstances[i].servo.write(EEPROM.read(345 + i));
            }
          }
        }
      }
    }
    // Wire preset
    if(EEPROM.read(393) == '#' &amp;&amp;
    EEPROM.read(395) == '#')
    {
      if(EEPROM.read(394) == 1)
      {
        _wireDefined = true;
        Wire.begin();
      }
    }
  }
}

void Teleduino2560Class::pinTimers()
{
  for(byte i = 0; i &lt; TD2560_PINS; i++)
  {
    if(_pins[i].expireAction &lt; 255 &amp;&amp; _pins[i].expireTime &lt;= millis() &amp;&amp; (millis() - _pins[i].expireTime) &lt;= 3600000 &amp;&amp; (_pins[i].expireTime - millis()) &gt;= 3600000)
    {
      setDigitalOutput(i, _pins[i].expireAction);
      _pins[i].expireAction = 255;
      _pins[i].expireTime = 0;
    }
  }
}

void Teleduino2560Class::reset()
{
  delay(100);
  for(byte i = 0; i &lt; TD2560_PINS; i++)
  {
    setDigitalOutput(i, LOW);
  }
  void(* teleduino2560ArduinoReset) (void) = 0;
  teleduino2560ArduinoReset();
}

boolean Teleduino2560Class::setDigitalOutput(byte pin, byte value, unsigned int time, byte save)
{
  if(checkDigitalPin(pin) &amp;&amp; _pins[pin].mode == OUTPUT)
  {
    if(save)
    {
      if(time == 0 &amp;&amp;
      EEPROM.read(0) == '#' &amp;&amp;
      EEPROM.read(1) == 1 &amp;&amp;
      EEPROM.read(2) == '#')
      {
        if(EEPROM.read(73) != '#')
        {
          EEPROM.write(73, '#');
        }
        if(EEPROM.read(144) != '#')
        {
          EEPROM.write(144, '#');
        }
        if(EEPROM.read(3 + pin) != 1)
        {
          EEPROM.write(3 + pin, 1);
        }
        if(EEPROM.read(74 + pin) != value)
        {
          EEPROM.write(74 + pin, value);
        }
      }
      else
      {
        return false;
      }
    }
    digitalWrite(pin, value);
    _pins[pin].expireAction = 255;
    _pins[pin].expireTime = 0;
    if(time &gt; 0)
    {
      _pins[pin].expireAction = !value;
      _pins[pin].expireTime = millis() + time;
      if(_pins[pin].expireTime &lt; 1)
      {
        _pins[pin].expireTime = 1;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

boolean Teleduino2560Class::setDigitalOutput(byte pin, byte value, unsigned int time)
{
  return setDigitalOutput(pin, value, time, 0);
}

boolean Teleduino2560Class::setDigitalOutput(byte pin, byte value)
{
  return setDigitalOutput(pin, value, 0, 0);
}

void Teleduino2560Class::setStatusLed(unsigned int count, boolean forever, long timeout)
{
  while(1)
  {
    for(unsigned int i = 0; i &lt; count; i++)
    {
      if(_statusLedPin &gt; -1)
      {
        setDigitalOutput(_statusLedPin, HIGH);
        delay(100);
        setDigitalOutput(_statusLedPin, LOW);
        delay(200);
      }
      else
      {
        delay(300);
      }
    }
    delay(500);
    if(!forever)
    {
      if(timeout &lt;= 0)
      {
        break;
      }
      else
      {
        timeout = timeout - (count * 300) - 500;
      }
    }
  }
}

void Teleduino2560Class::setStatusLed(unsigned int count, boolean forever)
{
  setStatusLed(count, forever, 0);
}

void Teleduino2560Class::setStatusLed(unsigned int count, long timeout)
{
  setStatusLed(count, false, timeout);
}

void Teleduino2560Class::setStatusLed(unsigned int count)
{
  setStatusLed(count, false, 0);
}

boolean Teleduino2560Class::setStatusLedPin(byte pin)
{
  if(checkDigitalPin(pin))
  {
    _statusLedPin = pin;
    definePinMode(_statusLedPin, OUTPUT);
    return true;
  }
  else
  {
    return false;
  }
}

void Teleduino2560Class::shiftRegisters()
{
  for(byte i = 0; i &lt; TD2560_SHIFT_REGISTERS; i++)
  {
    if(checkDigitalPin(_shiftRegisters[i].clockPin) &amp;&amp;
    checkDigitalPin(_shiftRegisters[i].dataPin) &amp;&amp;
    checkDigitalPin(_shiftRegisters[i].latchPin))
    {
      for(char j = 31; j &gt;= 0; j--)
      {
        shiftOut(_shiftRegisters[i].dataPin, _shiftRegisters[i].clockPin, MSBFIRST, _shiftRegisters[i].outputs[j]);
      }
      setDigitalOutput(_shiftRegisters[i].latchPin, HIGH);
      setDigitalOutput(_shiftRegisters[i].latchPin, LOW);
      if(checkDigitalPin(_shiftRegisters[i].enablePin))
      {
        setDigitalOutput(_shiftRegisters[i].enablePin, HIGH);
      }
    }
  }
}

void Teleduino2560Class::shiftRegisterTimers()
{
  for(byte i = 0; i &lt; TD2560_SHIFT_REGISTER_TIMERS; i++)
  {
    if(_shiftRegisterTimers[i].shiftRegister &lt; 255)
    {
      boolean blank = true;
      for(byte j = 0; j &lt; 32; j++)
      {
        if(_shiftRegisterTimers[i].outputs[j] &gt; 0)
        {
          blank = false;
          break;
        }
      }
      if(blank)
      {
        _shiftRegisterTimers[i].shiftRegister = 255;
        _shiftRegisterTimers[i].action = 255;
        _shiftRegisterTimers[i].expireTime = 0;
        for(byte j = 0; j &lt; 32; j++)
        {
          _shiftRegisterTimers[i].outputs[j] = 0;
        }
      }
    }
    if(_shiftRegisterTimers[i].shiftRegister &lt; 255 &amp;&amp; _shiftRegisterTimers[i].expireTime &lt;= millis() &amp;&amp; (millis() - _shiftRegisterTimers[i].expireTime) &lt;= 3600000 &amp;&amp; (_shiftRegisterTimers[i].expireTime - millis()) &gt;= 3600000)
    {
      for(byte j = 0; j &lt; 32; j++)
      {
        for(byte k = 0; k &lt; 8; k++)
        {
          byte currentBit = 1;
          currentBit = currentBit &lt;&lt; k;
          if((_shiftRegisterTimers[i].outputs[j] &amp; currentBit) &gt; 0)
          {
            switch(_shiftRegisterTimers[i].action)
            {
              case 0:
                if((_shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] &amp; currentBit) &gt; 0)
                {
                  _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] = _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] - currentBit;
                }
                break;
              case 1:
                if((_shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] &amp; currentBit) == 0)
                {
                  _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] = _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] + currentBit;
                }
                break;
              case 2:
                if((_shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] &amp; currentBit) &gt; 0)
                {
                  _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] = _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] - currentBit;
                }
                else if((_shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] &amp; currentBit) == 0)
                {
                  _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] = _shiftRegisters[_shiftRegisterTimers[i].shiftRegister].outputs[j] + currentBit;
                }
                break;
            }
          }
        }
      }
      _shiftRegisterTimers[i].shiftRegister = 255;
      _shiftRegisterTimers[i].action = 255;
      _shiftRegisterTimers[i].expireTime = 0;
      for(byte j = 0; j &lt; 32; j++)
      {
        _shiftRegisterTimers[i].outputs[j] = 0;
      }
    }
  }
}

Teleduino2560Class Teleduino2560;

/*
  Teleduino2560Wire.ino - Teleduino2560Wire example
  Version 2560-0.1.2
  Nathan Kennedy 2009 - 2014
  http://www.teleduino.org

  This sketch is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include &lt;EEPROM.h&gt;
#include &lt;Servo.h&gt;
#include &lt;Wire.h&gt;
#include &lt;Teleduino2560.h&gt;

// User configurable variables
byte address = 1;

// Other required variables
byte data[257];
byte dataWire[32];
int dataWireLength;
int offset;
byte page;
byte pages;
unsigned long lastRefresh = 0;
byte stage = 0;

void setup()
{
  // Load presets
  Teleduino2560.loadPresets();

  // Start wire interface
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
  // Process refreshes every 50ms
  if(millis() - lastRefresh &gt;= 50)
  {
    Teleduino2560.pinTimers();
    Teleduino2560.shiftRegisterTimers();
    Teleduino2560.shiftRegisters();
    lastRefresh = millis();
  }

  // Check to see if reset has been requested
  Teleduino2560.checkReset();
}

void receiveEvent(int byteCount)
{
  stage = 1;
  for(int i = 0; i &lt; 257 &amp;&amp; Wire.available(); i++)
  {
    data[i] = Wire.read();
  }
  Teleduino2560.instruction(data);
  stage = 2;
}

void requestEvent()
{
  switch(stage)
  {
    case 0:
      dataWire[0] = 1;
      dataWire[1] = 1;
      dataWire[2] = 0;
      dataWire[3] = 0;
      Wire.write(dataWire, 4);
      break;
    case 1:
      dataWire[0] = 1;
      dataWire[1] = 1;
      dataWire[2] = 2;
      dataWire[3] = 0;
      Wire.write(dataWire, 4);
      break;
    default:
      page = stage - 1;
      pages = (data[1] % 30 ? 2 : 1) + (data[1] / 30);
      offset = (page - 1) * 30;
      dataWire[0] = page;
      dataWire[1] = pages;
      for(dataWireLength = 2; dataWireLength &lt; 32 &amp;&amp; dataWireLength - 2 + offset &lt; int(data[1]) + 2; dataWireLength++)
      {
        dataWire[dataWireLength] = data[offset + dataWireLength - 2];
      }
      Wire.write(dataWire, dataWireLength);
      if(page == pages)
      {
        stage = 0;
      }
      else
      {
        stage++;
      }
      break;
  }
}

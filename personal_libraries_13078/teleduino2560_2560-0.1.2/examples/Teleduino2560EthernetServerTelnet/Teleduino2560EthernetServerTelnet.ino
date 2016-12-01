/*
  Teleduino2560EthernetServerTelnet.ino - Teleduino2560EthernetServerTelnet example
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

#include &lt;SPI.h&gt;
#include &lt;Ethernet.h&gt;

// User configurable variables
byte useDhcp = true;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress deviceIp(192, 168, 1, 100); // Only if useDhcp is false
IPAddress gatewayIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress dnsIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress subnet(255, 255, 255, 0); // Only if useDhcp is false
unsigned int serverPort = 23;

// Other required variables
byte data[257];
byte dataLength;
byte hexStage;
unsigned long lastRefresh = 0;
byte stage = 0;

// Declare server object
EthernetServer Server(serverPort);

void setup()
{
  // Load presets
  Teleduino2560.loadPresets();
  
  // Start network
  if(useDhcp)
  {
    if(!Ethernet.begin(mac))
    {
      Teleduino2560.reset();
    }
  }
  else
  {
    Ethernet.begin(mac, deviceIp, dnsIp, gatewayIp, subnet);
  }
  delay(1000);
  Server.begin();
}

void loop()
{
  EthernetClient Client = Server.available();
  
  if(Client)
  {
    // What we need to do depends on which 'stage' we are at
    switch(stage)
    {
      case 0: // Wait for start byte
        if(Client.available())
        {
          char c = Client.read();
          if(c == '?')
          {
            stage++;
          }
        }
        break;
      case 1: // Reset variables
        dataLength = 0;
        hexStage = 0;
        stage++;
        break;
      case 2: // Instruction byte
        if(Client.available())
        {
          char c = Client.read();
          if(c == '?')
          {
            stage = 1;
            break;
          }
          else if(c == '\r' || c == '\n' || c == '.')
          {
            stage = 0;
            break;
          }
          if(!hexStage)
          {
            data[0] = Teleduino2560.hexDecode(c) * 16;
          }
          else
          {
            data[0] += Teleduino2560.hexDecode(c);
          }
          hexStage = !hexStage;
          if(!hexStage)
          {
            stage++;
          }
        }
        break;
      case 3: // Data length byte
        if(Client.available())
        {
          char c = Client.read();
          if(c == '?')
          {
            stage = 1;
            break;
          }
          else if(c == '\r' || c == '\n' || c == '.')
          {
            stage = 0;
            break;
          }
          if(!hexStage)
          {
            data[1] = Teleduino2560.hexDecode(c) * 16;
          }
          else
          {
            data[1] += Teleduino2560.hexDecode(c);
          }
          hexStage = !hexStage;
          if(!hexStage)
          {
            stage++;
          }
        }
        break;
      case 4: // Data
        if(Client.available())
        {
          char c = Client.read();
          if(c == '?')
          {
            stage = 1;
            break;
          }
          else if(c == '\r' || c == '\n' || c == '.')
          {
            if(dataLength == data[1])
            {
              stage++;
              break;
            }
            else
            {
              stage = 0;
              break;
            }
          }
          if(!hexStage)
          {
            data[2 + dataLength] = Teleduino2560.hexDecode(c) * 16;
          }
          else
          {
            data[2 + dataLength] += Teleduino2560.hexDecode(c);
          }
          hexStage = !hexStage;
          if(!hexStage)
          {
            dataLength++;
          }
        }
        break;
      case 5: // Execute instruction and return result
        Teleduino2560.instruction(data);
        Client.print('!');
        for(int i = 0; i &lt; data[1] + 2; i++)
        {
          Client.write(Teleduino2560.hexEncode(data[i] / 16));
          Client.write(Teleduino2560.hexEncode(data[i] % 16));
        }
        Client.println();
        stage = 0;
        break;
    }
  }
  
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

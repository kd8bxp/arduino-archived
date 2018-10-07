/*
  Teleduino2560EthernetClientProxy.ino - Teleduino2560EthernetClientProxy example
  Version 2560-0.1.2
  Nathan Kennedy 2009 - 2014
  http://www.teleduino.org

  This sketch is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include <Teleduino2560.h>

#include <SPI.h>
#include <Ethernet.h>

// User configurable variables
byte useDhcp = true;
byte useDns = true;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress deviceIp(192, 168, 1, 100); // Only if useDhcp is false
IPAddress gatewayIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress dnsIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress subnet(255, 255, 255, 0); // Only if useDhcp is false
IPAddress serverIp(173, 230, 152, 173); // Only if useDns is false
char serverName[] = "us01.proxy.teleduino.org"; // Only if useDns is true
unsigned int serverPort = 5353; // Can be set to either 53 or 5353
byte statusLedPin = 8;

// User configurable key, this is used to authenticate with the proxy server
// This is checked against the EEPROM on boot and written if necessary
// The proxy server retreives the key from the EEPROM
byte key[] = { 0x40, 0x79, 0x0B, 0x07,
               0x98, 0xE1, 0xA1, 0xCF,
               0x07, 0x0A, 0xE3, 0xEA,
               0xE5, 0xEF, 0x07, 0x04 };

// Other required variables
byte data[257];
byte dataLength;
byte hexStage;
unsigned long lastInstruction = 0;
unsigned long lastRefresh = 0;
byte stage = 0;

// Declare client object
EthernetClient Client;

void setup()
{
  // This is here purely to add a bit of padding to the sketch to fix a compiling bug
  // http://arduino.cc/forum/index.php/topic,60649.0.html
  EEPROM.read(0);
  
  // Load presets
  Teleduino2560.loadPresets();
  
  // Set status LED pin
  Teleduino2560.setStatusLedPin(statusLedPin);

  Teleduino2560.setStatusLed(1); // Initialisation
  // Check the EEPROM header and check to see if the key is correct
  // This is to ensure the key is not cleared from the EEPROM
  if(EEPROM.read(0) != '#')
  {
    EEPROM.write(0, '#');
  }
  if(EEPROM.read(1) != 1)
  {
    EEPROM.write(1, 1);
  }
  if(EEPROM.read(2) != '#')
  {
    EEPROM.write(2, '#');
  }
  if(EEPROM.read(395) != '#')
  {
    EEPROM.write(395, '#');
  }
  for(byte i = 0; i < 16; i++)
  {
    if(EEPROM.read(396 + i) != key[i])
    {
      EEPROM.write(396 + i, key[i]);
    }
  }
  if(EEPROM.read(412) != '#')
  {
    EEPROM.write(412, '#');
  }

  // Start network and attempt to connect to proxy server
  Teleduino2560.setStatusLed(2); // Network configuration
  if(useDhcp)
  {
    if(!Ethernet.begin(mac))
    {
      Teleduino2560.setStatusLed(2, false, 10000);
      Teleduino2560.reset();
    }
  }
  else
  {
    Ethernet.begin(mac, deviceIp, dnsIp, gatewayIp, subnet);
  }
  delay(1000);

  Teleduino2560.setStatusLed(3); // Connect to server
  if((useDns && !Client.connect(serverName, serverPort)) || (!useDns && !Client.connect(serverIp, serverPort)))
  {
    Teleduino2560.setStatusLed(3, false, 10000);
    Teleduino2560.reset();
  }
  lastInstruction = millis();
}

void loop()
{
  if(Client.connected())
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
        Client.write('!');
        for(int i = 0; i < data[1] + 2; i++)
        {
          Client.write(Teleduino2560.hexEncode(data[i] / 16));
          Client.write(Teleduino2560.hexEncode(data[i] % 16));
        }
        Client.write('\n');
        lastInstruction = millis();
        stage = 0;
        break;
    }
  }
  else
  {
    Teleduino2560.setStatusLed(10);
    Teleduino2560.reset();
  }

  // Has the instruction timeout been reached?
  if(millis() - lastInstruction > 30000)
  {
    Client.flush();
    Client.stop();
    Teleduino2560.setStatusLed(9);
    Teleduino2560.reset();
  }

  // Process refreshes every 50ms
  if(millis() - lastRefresh >= 50)
  {
    Teleduino2560.pinTimers();
    Teleduino2560.shiftRegisterTimers();
    Teleduino2560.shiftRegisters();
    lastRefresh = millis();
  }

  // Check to see if reset has been requested
  Teleduino2560.checkReset();
}
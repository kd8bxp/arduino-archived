/*
  Teleduino2560EthernetServerWeb.ino - Teleduino2560EthernetServerWeb example
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
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress deviceIp(192, 168, 1, 100); // Only if useDhcp is false
IPAddress gatewayIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress dnsIp(192, 168, 1, 1); // Only if useDhcp is false
IPAddress subnet(255, 255, 255, 0); // Only if useDhcp is false
unsigned int serverPort = 80;

// Other required variables
unsigned int contentLength;
byte data[257];
byte dataLength;
byte hexStage;
unsigned long millisStart;
boolean newlineLastReceived;
unsigned long lastRefresh = 0;
byte stage = 0;
long timeout;

byte key[] = { 0x40, 0x79, 0x0B, 0x07,
               0x98, 0xE1, 0xA1, 0xCF,
               0x07, 0x0A, 0xE3, 0xEA,
               0xE5, 0xEF, 0x07, 0x04 };

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
    // Reset some variables
    newlineLastReceived = false;
    stage = 0;
    timeout = 500;
    
    // Keep looping while the client is connected
    while(Client.connected())
    {
      millisStart = millis();
      
      // If we have reached the timeout prior to getting to stage 6 jump to stage 108
      if(stage < 6 && timeout < 0)
      {
        stage = 108;
      }
      
      // What we need to do depends on which 'stage' we are at
      switch(stage)
      {
        case 0: // Wait for start byte
          if(Client.available())
          {
            char c = Client.read();
            if(c == '/')
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
            if(c == '\r' || c == '\n' || c == '.' || c == ' ')
            {
              stage = 100;
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
            if(c == '\r' || c == '\n' || c == '.' || c == ' ')
            {
              stage = 100;
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
            if(c == '\r' || c == '\n' || c == '.' || c == ' ')
            {
              if(dataLength == data[1])
              {
                if(c == '\n')
                {
                  newlineLastReceived = true;
                }
                stage++;
                break;
              }
              else
              {
                stage = 100;
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
        case 5: // Receive the remainder of the HTTP header
          if(Client.available())
          {
            char c = Client.read();
            if(c == '\n')
            {
              if(newlineLastReceived)
              {
                stage++;
              }
              else
              {
                newlineLastReceived = true;
              }
            }
            else if(c != '\r')
            {
              newlineLastReceived = false;
            }
          }
          break;
        case 6: // Execute instruction and return result
          Teleduino2560.instruction(data);
          contentLength = 6 + (data[1] * 2);
          Client.flush();
          Client.println("HTTP/1.1 200 OK");
          Client.println("Content-Type: text/html");
          Client.print("Content-Length: ");
          Client.println(contentLength, DEC);
          Client.println();
          for(int i = 0; i < data[1] + 2; i++)
          {
            Client.write(Teleduino2560.hexEncode(data[i] / 16));
            Client.write(Teleduino2560.hexEncode(data[i] % 16));
          }
          Client.println();
          stage++;
          break;
        case 7: // Disconnect client
          delay(5);
          Client.stop();
          break;
        case 100: // Bad request
          Client.flush();
          Client.println("HTTP/1.1 400 Bad Request");
          Client.println("Content-Type: text/html");
          Client.println("Content-Length: 124");
          Client.println();
          Client.println("Invalid request syntax.<br />Visit <a href=\"http://www.teleduino.org\">http://www.teleduino.org</a> for protocol reference.");
          stage = 7;
          break;
        case 108: // Request Timeout
          Client.flush();
          Client.println("HTTP/1.1 408 Request Timeout");
          Client.println("Content-Type: text/html");
          Client.println("Content-Length: 117");
          Client.println();
          Client.println("Request timeout.<br />Visit <a href=\"http://www.teleduino.org\">http://www.teleduino.org</a> for protocol reference.");
          stage = 7;
          break;
      }
      
      // Take the time off the timeout
      timeout = timeout - (millis() - millisStart);
    }
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
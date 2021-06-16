#include "ArduinoBASIC.h"
#include <EEPROM.h>

ArduinoBASIC arduinoBASIC = ArduinoBASIC ();
DebugUtilities debugUtils;

#define NUMBER_OF_STEPS 15
PSTRStrings eProgram = PSTRStrings(NUMBER_OF_STEPS);
bool paused;

void callback (int value)
{
  Serial.print ( "From callback value:" );
  Serial.println ( value );
}

unsigned long runTimeout;
void setup()
{  
  Serial.begin (115200);
  debugUtils.printPSTR ( PSTR ( "Arduino BASIC...enter ? to display keywords\nTo use the Arduino IDE\n select 'Carriage return' rather than 'no line ending' in the Serial Monitor\n" ));
  arduinoBASIC.init();
  arduinoBASIC.eepromProgram.callback = callback;
    
  eProgram.addString ( PSTR ( "setA=0" )); 
  eProgram.addString ( PSTR ( "ifA")); 
  eProgram.addString ( PSTR ( "ifA"));
  eProgram.addString ( PSTR ( "echooops A is set"));
  eProgram.addString ( PSTR ( "callback1"));
  eProgram.addString ( PSTR ( "endif"));
  eProgram.addString ( PSTR ( "else"));
  eProgram.addString ( PSTR ( "ifA=0"));
  eProgram.addString ( PSTR ( "echoA is not set"));
  eProgram.addString ( PSTR ( "endif"));
  eProgram.addString ( PSTR ( "endif"));
  eProgram.addString ( PSTR ( "echoAll done!"));

  runTimeout = millis() + 10000;
  Serial.println ( "Program will run in 10 seconds unless a key is pressed" );    
  paused = true;
}

void loop()
{
  static unsigned long runTimeout;
  if (runTimeout)
  {
    if (millis() > runTimeout)
    {
      arduinoBASIC.eepromProgram.run();
      runTimeout = 0;
      paused = false;
    }
  }
  
  if (!paused)  
    arduinoBASIC.eepromProgram.continueTest();
    
  if (Serial.available())
  {
    runTimeout = 0;
    paused = false;
    arduinoBASIC.handleChar(eProgram, Serial.read()); 
  }  
}

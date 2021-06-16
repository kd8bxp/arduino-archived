#ifndef ARDUINOBASIC_H
#define ARDUINOBASIC_H

#include <Arduino.h>
#include "PSTRStrings.h"
#include "ScriptEEPROM.h"
#include "DebugUtilities.h"

class ArduinoBASIC
{
public:     
  //typedef void (*VoidType)();  
  ArduinoBASIC();   
  void handleChar ( PSTRStrings & cannedProgram, char ch );
  
  PSTRStrings commands;   // BASIC commands
  PSTRStrings statements; // BASIC Statements
  ScriptEEPROM eepromProgram; 
  DebugUtilities debugUtils;
  void loadProgram (PSTRStrings &cannedProgram );
  void init();
  
private:
  void continueStatement ( char ch );
  int lastStatement;
};
#endif



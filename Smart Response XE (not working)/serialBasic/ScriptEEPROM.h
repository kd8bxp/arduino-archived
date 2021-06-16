#ifndef SCRIPTEEPROM_H
#define SCRIPTEEPROM_H

#include <Arduino.h>
//#include <I2C_eeprom.h>
#include "DebugUtilities.h"
#include "PSTRStrings.h"
#include "Components.h"
#include "SevenSegment.h"
#include "LiquidCrystal.h"

#define MAX_EEPROM_BUFFER 1024

typedef void (*VoidCallbackType)();
typedef void (*IntCallbackType)(int value);      


class ScriptEEPROM
{
  public:          
    // Constructor
    ScriptEEPROM(PSTRStrings * _statements, PSTRStrings * _commands);
    void showSteps ();
    int findStep ( int step ); 
    bool addCh (char ch, bool incrementHead);
    int numSteps (); 
    void reset(); // Stop test
    void continueTest();
    void executeStep (bool &stepDone);
    void clear();  // Clear out the script
    
    const prog_char * testStatus ();
    char * getCh ( char * parameter, char * ch );
    char * readHexFromChar (char * hex, int * total);
    char lcase ( char ch );
    void dump();
    int testState; // 0 = Idle, 1 = In Progress, 2 = Successfully completed, 3 = Test Failure Detected 
    void run(); // Start the program
    IntCallbackType callback;
    void removeLine(int stepNumber); // Delete a step
    // void change(int stepNumber, char c); // Change a step
    int readDec (char * ch);
	bool eepromMatch (char ch, int which, bool clearResults);
	int findLabel (int index, bool debugThis);
	void writeTestState (int newState);
	bool paused;
	bool nextCommand ( int &value, int &index );
    int maxStatements();


  private:
    int numStatements; // EEPROM index
    int testIndex;
	int nestingLevel;  // How deep are we into if statements
    int currentCommand; // Current Test Command
    DebugUtilities debugUtils;    
    // PROGMEM const char * decodeCommand (uint8_t command );
    // int showDecimal (int index);
    int readDecimal (int &testPointer);
    // Return an EEPROM pointer, that points to the next command=cmd
    int indexToStep (int indexValue);
    uint8_t readEEPROM ( int index );
	unsigned long waitTimeout;

    int T;
    int E;
    int A;
    int M;

    // The eeprom should be able to save 128K of bytes, but only 2 bytes of address? = 64K?
    unsigned long eepromTail;
    unsigned long eepromHead;
    PSTRStrings * statements;
    PSTRStrings * commands;
    Components components;
    void showStatus();
    int dumpStatement(int &index);
	void skipTo ( int value, int &index, bool consume );
    void printMatchString ( int which );
	void skipToEndElse ( int &index);
	int nextCommand (int &index);
	SevenSegment sevenSegment;
	LiquidCrystal liquidCrystal;
};
#endif

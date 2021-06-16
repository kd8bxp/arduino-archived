#include "ArduinoBASIC.h"
#define NUMBER_OF_COMMANDS 9
#define NUMBER_OF_STATEMENTS 26
 
ArduinoBASIC::ArduinoBASIC(): 
  commands(NUMBER_OF_COMMANDS), 
  statements(NUMBER_OF_STATEMENTS), 
  eepromProgram (&statements, &commands),
  debugUtils()
{
  lastStatement = -1;
  eepromProgram.callback = 0;
}

void ArduinoBASIC::init()
{
  commands.addString ( PSTR ( "?"));              //  0
  commands.addString ( PSTR ( "list" ));          //  1
  commands.addString ( PSTR ( "clear" ));         //  2
  commands.addString ( PSTR ( "dump" ));          //  3
  commands.addString ( PSTR ( "run" ));           //  4
  commands.addString ( PSTR ( "remove"));         //  5
  commands.addString ( PSTR ( "load"));           //  6
  commands.addString ( PSTR ( "stop"));           //  7
  commands.addString ( PSTR ( "showTestState"));  //  8
   
  statements.addString ( PSTR ( "endtest" ));        //  0
  statements.addString ( PSTR ( "wait" ));           //  1
  statements.addString ( PSTR ( "relay" ));          //  2
  statements.addString ( PSTR ( "echo" ));           //  3
  statements.addString ( PSTR ( "endif" ));          //  4
  statements.addString ( PSTR ( "if" ) );            //  5
  statements.addString ( PSTR ( "jump" ));           //  6
  statements.addString ( PSTR ( "lcdnotused" ));            //  7
  statements.addString ( PSTR ( "read" ));           //  8   
  statements.addString ( PSTR ( "set"));             //  9 
  statements.addString ( PSTR ( "callback"));        // 10
  statements.addString ( PSTR ( "thermistor"));      // 11
  statements.addString ( PSTR ( "startTimer"));      // 12
  statements.addString ( PSTR ( "delay"));           // 13
  statements.addString ( PSTR ( "addMatch"));        // 14
  statements.addString ( PSTR ( "showMatches"));     // 15
  statements.addString ( PSTR ( "checkMatch"));      // 16
  statements.addString ( PSTR ( ":"));               // 17
  statements.addString ( PSTR ( "clearMatches"));    // 18
  statements.addString ( PSTR ( "testState"));       // 19
  statements.addString ( PSTR ( "else"));            // 20
  statements.addString ( PSTR ( "7SegmentInit"));    // 21
  statements.addString ( PSTR ( "7SegmentValue"));   // 22
  statements.addString ( PSTR ( "lcdDisplayInit"));  // 23
  statements.addString ( PSTR ( "lcdDisplayClear")); // 24
  statements.addString ( PSTR ( "lcdDisplayPrint")); // 25
}

void ArduinoBASIC::continueStatement ( char ch )
{
  if (ch == 13)
  {
    eepromProgram.addCh (0, true);
    eepromProgram.addCh (0, false);
  }  
  else  
    eepromProgram.addCh (ch, true);
}

// Load a canned program
void ArduinoBASIC::loadProgram (PSTRStrings & cannedProgram  )
{
  int j;
  char ch;
  int len; 
  int start;
  int match;
  bool ok = true;
  
  eepromProgram.clear();
  debugUtils.printPSTR ( PSTR ( "Load program: (" ) );
  Serial.print ( cannedProgram.numStrings );
  debugUtils.printPSTR ( PSTR ( " steps)\n" ) );
  
  // Statements could be in the middle of a match
  statements.clearIndexes();

  for (int i=0; i<cannedProgram.numStrings; i++)
  {
    
    match = statements.intersect ( &cannedProgram, i);
    if (match > -1)  
    {
      eepromProgram.addCh (match, true);
      
      len = cannedProgram.stringLen(i);        
      start = statements.stringLen(match);
      if (start <= len)
        for (int j=start; j<len; j++)
        {
          ch = cannedProgram.charAt (i,j);
          eepromProgram.addCh (ch, true);
        } 
      eepromProgram.addCh (0, true);         
    }
    else
    {
       debugUtils.printPSTR ( PSTR ( "Could not match canned statement : " ) );
       cannedProgram.printString ( i );
       Serial.println ();
	   ok = false;
	   break;
    }  
  }
  eepromProgram.addCh (0, true); // Add program termination        
  if (ok)
  {  
    debugUtils.printPSTR ( PSTR ( "Show steps:\n" ) );
    eepromProgram.showSteps();
  }	
}

void ArduinoBASIC::handleChar ( PSTRStrings & cannedProgram, char ch  )
{
  int gotStatement = statements.matchString (ch, false);
  int gotCommand   = commands.matchString   (ch, false);
  char c;
  int value;
    
  if (lastStatement > -1)
  { 
    // Save uninterpretted parameters to EEPROM
    continueStatement (ch); 
    if (ch == 13) 
      lastStatement = -1; // done
  }
  else if (gotStatement > -1)
  { 
    debugUtils.printPSTR ( PSTR ( "Matched on statement: " ) );
    statements.printString ( gotStatement );
    eepromProgram.addCh (gotStatement, true);
    lastStatement = gotStatement;
  }
  
  else // Check for command
  {
        
    if (gotCommand > -1) 
    {    
      //debugUtils.printPSTR ( PSTR ( "gotCommand: " ) );
      //Serial.println ( gotCommand );
      //commands.printString ( gotCommand ); 
      //debugUtils.printPSTR ( PSTR ( "\n" ) );
       
      switch (gotCommand)
      {
        case 0: 
          debugUtils.printPSTR ( PSTR ( "Commands: \n" ));
          commands.show(1,NUMBER_OF_COMMANDS);
          debugUtils.printPSTR ( PSTR ( "\nstatements: \n" ));
          statements.show(1,NUMBER_OF_STATEMENTS);
          break;
        case 1:
          eepromProgram.showSteps();
          break;
        case 2:
          eepromProgram.clear();
          break;
        case 3:
          eepromProgram.dump();
          break;
        case 4: 
          eepromProgram.run();
          break;    
        case 5: // remove a line
          value = eepromProgram.readDec ( &c);
          eepromProgram.removeLine(value);
          break;  
        case 6: // Load program from canned list
          loadProgram ( cannedProgram );
          break;
        case 7: // Stop the program 
          eepromProgram.writeTestState (0); 
          debugUtils.printPSTR ( PSTR ( "*Test now idle*" ) );
          break; 
		case 8: // Show test state
          debugUtils.printPSTR ( PSTR ( "Test state: " ) );
          Serial.println ( eepromProgram.testState);
          break;		  
        default:
          break;
      }
    }  
  }
}

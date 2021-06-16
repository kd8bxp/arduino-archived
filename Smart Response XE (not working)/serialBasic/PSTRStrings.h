#ifndef PSTRSTRINGS_H
#define PSTRSTRINGS_H

#include <Arduino.h>
#include "DebugUtilities.h"

struct CommandStringType
{
  int index;
  int len;
  prog_char * ptr;
  int matched;
};

class PSTRStrings
{
  public:          
    // Constructor
    PSTRStrings(int _numberOfStrings);
    int matchString ( char ch, bool doDebug); 
    void printString ( int which );
    void addString (const prog_char * s);
    void show ( int startValue, int stopValue ); 
    void showAll ();
    int matchCharPointer ( char * ch ); 
    int numberOfStrings;
    char charAt ( int i, int j);
    void showMatches();
    void clearMatches();
    void clearIndexes();
    int stringLen( int which);
    int intersect ( PSTRStrings * targetList, int index );  
    bool checkMatch (int which);
	
    int numStrings;
    
  private:  
    CommandStringType * strings;
    DebugUtilities debugUtils;
};
#endif

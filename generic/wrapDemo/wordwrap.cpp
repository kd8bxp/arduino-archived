/*
 *	A word wrap library for Arduino
 *
 *	https://github.com/ncmreynolds/wordwrap
 *
 *	Released under LGPL-2.1 see https://github.com/ncmreynolds/wordwrap/LICENSE for full license
 *
 */
#ifndef wordwrap_cpp
#define wordwrap_cpp
#include "wordwrap.h"

//#define DEBUG_WORDWRAP


wordwrapClass::wordwrapClass() {	//Constructor function
}

wordwrapClass::~wordwrapClass() {	//Destructor function
}

String wordwrapClass::wrap(const String& textToConstrain, const uint16_t constrainedColumns) {
  uint16_t ignoreLines;
  return box(textToConstrain, 0xffff, constrainedColumns, ignoreLines);
}

String wordwrapClass::wrap(const String& textToConstrain, uint16_t constrainedColumns, uint16_t& rowsUsed) {
  return box(textToConstrain, 0xffff, constrainedColumns, rowsUsed);
}

String wordwrapClass::wrap(const char* textToConstrain, const uint16_t constrainedColumns) {
  uint16_t ignoreLines;
  return box(String(textToConstrain), 0xffff, constrainedColumns, ignoreLines);
}

String wordwrapClass::wrap(const char* textToConstrain, const uint16_t constrainedColumns, uint16_t& rowsUsed) {
  return box(String(textToConstrain), 0xffff, constrainedColumns, rowsUsed);
}

String wordwrapClass::box(const char* textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns) {
  uint16_t ignoreLines;
  return box(String(textToConstrain), constrainedRows, constrainedColumns, ignoreLines);
}

String wordwrapClass::box(const char* textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns, uint16_t& rowsUsed) {
	return box(String(textToConstrain), constrainedRows, constrainedColumns, rowsUsed);
}

String wordwrapClass::box(const String& textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns) {
	uint16_t ignoreLines;
	return box(textToConstrain, constrainedRows, constrainedColumns, ignoreLines);
}

String wordwrapClass::box(const String& textToConstrain, const uint16_t constrainedRows, const uint16_t constrainedColumns, uint16_t& rowsUsed) {
	overflowed = false;
	String wrappedText = "";
	wrappedText.reserve(constrainedColumns);
	String currentWord = "";
	currentWord.reserve(constrainedColumns);
	uint16_t currentLineLength = 0;
	if(textToConstrain.length() == 0) {
		rowsUsed = 0;
		return(textToConstrain);
	}
	rowsUsed = 1;
	//for(uint16_t position = 0; position < textToConstrain.length(); position++) {
	uint16_t position = 0;
	while(currentWord.length() > 0 || (position < textToConstrain.length() && rowsUsed <= constrainedRows)) {
		while(position < textToConstrain.length() && isSpace(textToConstrain.charAt(position)) == false) {
			//Read in the 'current' word
			currentWord += textToConstrain.charAt(position);
			position++;
		}
		//else
		while(currentWord.length() > 0)
		{
			if(currentWord.length() > constrainedColumns)
			{
				if(currentLineLength == 0) 
				{
					//Split overlong word at start of line
					#ifdef DEBUG_WORDWRAP
					Serial.print("Break overlong first word: \"");
					Serial.print(currentWord.substring(0,constrainedColumns));
					Serial.println('\"');
					#endif
					wrappedText += currentWord.substring(0,constrainedColumns);
					currentLineLength = constrainedColumns;
					currentWord = currentWord.substring(constrainedColumns);
				}
				else if(constrainedColumns - currentLineLength >= 3)
				{
					//Overlong word, which can fit at least two characters on the end of the line
					#ifdef DEBUG_WORDWRAP
					Serial.print("Break overlong next word: \"");
					Serial.print(currentWord.substring(0,constrainedColumns - (currentLineLength + 2)));
					Serial.println('\"');
					#endif
					wrappedText += " ";
					wrappedText += currentWord.substring(0,constrainedColumns - (currentLineLength + 2));
					currentWord = currentWord.substring(constrainedColumns - (currentLineLength + 2));
					currentLineLength = constrainedColumns;
				}
				else
				{
					//Force new line for overlong word so more fits, aiding legibility
					if(rowsUsed == constrainedRows) {
						if(ellipsis == true) {
							if(currentLineLength + 3 > constrainedColumns) {
								//Trim to the nearest space that will fit '...'
								wrappedText = wrappedText.substring(0,max(wrappedText.lastIndexOf(" ",wrappedText.length() - 3),wrappedText.lastIndexOf("\n")+1));
							}
							//wrappedText += String(currentLineLength,DEC);
							wrappedText += "...";
						}
						overflowed = true;
						return wrappedText;
					}
					if(crlf == true) {
						wrappedText += "\r\n";
					} else {
						wrappedText += "\n";
					}
					currentLineLength = 0;
					rowsUsed++;
				}
			}
			else if(currentLineLength == 0)
			{
				//First word on a line
				#ifdef DEBUG_WORDWRAP
				Serial.print("First word: \"");
				Serial.print(currentWord);
				Serial.println('\"');
				#endif
				//This fits so can be added
				wrappedText += currentWord;
				currentLineLength = currentWord.length();
				currentWord = "";
			}
			else if(currentLineLength + currentWord.length() < constrainedColumns)
			{
				//Word fits onto current line
				#ifdef DEBUG_WORDWRAP
				Serial.print("Add next word: \"");
				Serial.print(currentWord);
				Serial.println('\"');
				#endif
				wrappedText += " ";
				wrappedText += currentWord;
				currentLineLength += currentWord.length() + 1;
				currentWord = "";
			}
			else
			{
				//New line
				if(rowsUsed == constrainedRows) {
					if(ellipsis == true) {
						if(currentLineLength + 3 > constrainedColumns) {
							//Trim to the nearest space that will fit '...'
							wrappedText = wrappedText.substring(0,max(wrappedText.lastIndexOf(" ",wrappedText.length() - 3),wrappedText.lastIndexOf("\n")+1));
						}
						//wrappedText += String(currentLineLength,DEC);
						wrappedText += "...";
					}
					overflowed = true;
					return wrappedText;
				}
				#ifdef DEBUG_WORDWRAP
				Serial.print("New line to fit: \"");
				Serial.print(currentWord);
				Serial.println('\"');
				#endif
				if(crlf == true) {
					wrappedText += "\r\n";
				} else {
					wrappedText += "\n";
				}
				currentLineLength = 0;
				rowsUsed++;
			}
		}
		while(position < textToConstrain.length() && isSpace(textToConstrain.charAt(position)) == true)
		{
			//Gobble whitespace
			position++;
		}
	}
	//Handle any last word
	/*
	if(currentWord.length() > 0) {
		if(rowsUsed <= constrainedRows) {
			if(currentLineLength <= constrainedColumns) {
				wrappedText += currentWord;
			} else if(rowsUsed < constrainedRows) {
				if(crlf == true) {
					wrappedText += "\r\n";
				} else {
					wrappedText += "\n";
				}
				currentLineLength = 0;
				wrappedText += currentWord;
				currentWord = "";
				rowsUsed++;
			} else {
				if(ellipsis == true) {
					if(lastLineLength + 3 > constrainedColumns) {
						wrappedText = wrappedText.substring(0,wrappedText.length() - 3);
					}
					wrappedText += "...";
				}
			}
		} else {
			if(ellipsis == true) {
				if(lastLineLength + 3 > constrainedColumns) {
					wrappedText = wrappedText.substring(0,wrappedText.length() - 3);
				}
				wrappedText += "...";
			}
		}
	}
	*/
	return wrappedText;
}
wordwrapClass wordwrap;
#endif

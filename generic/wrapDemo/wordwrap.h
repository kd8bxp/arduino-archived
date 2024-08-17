/*
 *	A word wrap library for Arduino
 *
 *	https://github.com/ncmreynolds/wordwrap
 *
 *	Released under LGPL-2.1 see https://github.com/ncmreynolds/wordwrap/LICENSE for full license
 *
 */
#ifndef wordwrap_h
#define wordwrap_h
#include <Arduino.h>

class wordwrapClass {

	public:
		wordwrapClass();												//Constructor function
		~wordwrapClass();												//Destructor function
		bool ellipsis = true;											//Whether to add '...' when constraining text
		bool crlf = true;												//Whether to add cr/lf or just lf at the end of lines
		bool overflowed = false;										//Whether the last constrained text overflowed
		String wrap(const String& textToConstrain, uint16_t constrainedColumns);
		String wrap(const String& textToConstrain, uint16_t constrainedColumns, uint16_t& rowsUsed);
		String wrap(const char* textToConstrain, uint16_t constrainedColumns);
		String wrap(const char* textToConstrain, uint16_t constrainedColumns, uint16_t& rowsUsed);
		String box(const String& textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns);
		String box(const String& textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns, uint16_t& rowsUsed);
		String box(const char* textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns);
		String box(const char* textToConstrain, uint16_t constrainedRows, uint16_t constrainedColumns, uint16_t& rowsUsed);
	protected:
	private:
};
extern wordwrapClass wordwrap;
#endif

/*
 * This example sketch word wraps these opening words from Moby Dick into a variety of maximum columns
 * 
 * https://github.com/ncmreynolds/wordwrap
 * 
 */

#include "wordwrap.h"
String text = "Call me Ishmael. Some years ago—never mind how long precisely—having little or no money in my purse, and nothing particular to interest me on shore, I thought I would sail about a little and see the watery part of the world.";
uint16_t usedRows;
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.print("Text to wrap: ");
  Serial.println(text);
  //wordwrap.ellipsis = false;  //Uncomment to disable the added elipsis when text doesn't fit in the 'box'
  //wordwrap.crlf = false;  //Uncomment to use Linux style line breaks, instead of Windows style line breaks
  for(uint16_t maxColumns = 10; maxColumns <= 80; maxColumns+=5)
  {
    for(uint16_t i = 0; i < maxColumns; i++)
    {
      Serial.print(i%10);
    }
    Serial.print(" Maximum columns: ");
    Serial.println(maxColumns);
    Serial.println(wordwrap.wrap(text, maxColumns, usedRows));
    Serial.print("Used rows: ");
    Serial.println(usedRows);
    Serial.print("\r\n");
    delay(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

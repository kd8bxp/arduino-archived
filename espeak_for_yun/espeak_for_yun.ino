/*
  Arduino Meets Linux:
  The User's Guide to Arduino Yún Development
  Copyright (c) 2015, Bob Hammell.
  
  Disclaimer:
  This code is provided by ArduinoMeetsLinux.com on an "AS IS"
  basis. ArduinoMeetsLinux.com makes no warranties, express or
  implied, regarding use of the code alone or in combination with
  your projects.
  
  In no event shall ArduinoMeetsLinux.com be liable for any special,
  indirect, incidental, or consequential damages (including, but not
  limited to, loss of data and damage to systems) arising in any way
  out of the use, reproduction, modification and/or distribution of
  this code.
*/

#include <Bridge.h>

Process p;

void setup() {
  Bridge.begin();
  p.runShellCommand("espeak \"Hello from an Arduino sketch. Lets count down from five.\"");
  
  for (int t=5; t>0; t--) {
    String num = String(t);
    p.runShellCommand("espeak " + num);
    delay(500);
  }
  
  p.runShellCommand("espeak \"Hooray, we did it.\"");
  p.close();  
}

void loop() {
}

/***************************************************************************
Title: PCA9554 sketch (PCA9554) - Output Example
Created by: Mach-1 Electronics - AD0ND
For: Arduino Users Everywhere!

This file is free software; you can redistribute it and/or modify
it under the terms of either the GNU General Public License version 2
or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
***************************************************************************/

#include "Pca9554.h"  // Load the PCA9554 Library
#include "src/M5StickC/M5StickC.h"
#include <Wire.h>     // Load the Wire Library

void setup()
{
  M5.begin();
  Serial.begin(115200);
  Wire.begin();
  Pca9554.begin();
  Pca9554.pinMode(0, INPUT);
  Pca9554.pinPolarity(1, INVERTED);
  Pca9554.pinMode(1, INPUT);
  Pca9554.pinMode(2, INPUT);
  Pca9554.pinMode(3, INPUT);
  Pca9554.pinMode(4, INPUT);
  Pca9554.pinMode(5, INPUT);
  Pca9554.pinMode(6, INPUT);
  Pca9554.pinMode(7, INPUT);
}


void loop()
{
   Serial.print("Port 1 " ); Serial.println(Pca9554.digitalRead(1));
   Serial.print("Port 2 " ); Serial.println(Pca9554.digitalRead(2));
   Serial.print("Port 3 " ); Serial.println(Pca9554.digitalRead(3));
   Serial.print("Port 4 " ); Serial.println(Pca9554.digitalRead(4));
   Serial.print("Port 5 " ); Serial.println(Pca9554.digitalRead(5));
   Serial.print("Port 6 " ); Serial.println(Pca9554.digitalRead(6));
   delay(500);
}

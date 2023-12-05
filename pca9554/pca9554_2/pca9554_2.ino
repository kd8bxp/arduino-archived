/***************************************************************************
Title: PCA9554 sketch (PCA9554) - Output Example
Created by: Mach-1 Electronics - AD0ND
For: Arduino Users Everywhere!

This file is free software; you can redistribute it and/or modify
it under the terms of either the GNU General Public License version 2
or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
***************************************************************************/

#include "PCA9554.h"  // Load the PCA9554 Library
#include "src/M5StickC/M5StickC.h"
#include <Wire.h>     // Load the Wire Library

PCA9554 ioCon1(0x27);  // Create an object at this address
//  PCA9554 Addressing
//  Address     A2  A1  A0
//  0x20        L   L   L
//  0x21        L   L   H
//  0x22        L   H   L
//  0x23        L   H   H
//  0x24        H   L   L
//  0x25        H   L   H
//  0x26        H   H   L
//  0x27        H   H   H

void setup()
{
  M5.begin();
  Serial.begin(115200);
  Wire.begin();
  PCA9554(0x27, &Wire);
  int test = pca9554_return_t configMultiplePorts(1, 0xFF );

}



void loop()
{
   Serial.print("Port 1 " ); Serial.println(pca9554_return_t readOutputRegister(int res, 1));
   //Serial.print("Port 2 " ); Serial.println(ioCon1.digitalRead(2));
   //Serial.print("Port 3 " ); Serial.println(ioCon1.digitalRead(3));
   //Serial.print("Port 4 " ); Serial.println(ioCon1.digitalRead(4));
   //Serial.print("Port 5 " ); Serial.println(ioCon1.digitalRead(5));
   //Serial.print("Port 6 " ); Serial.println(ioCon1.digitalRead(6));
   delay(500);
}

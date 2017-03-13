#include <Wire.h>
#include "HMC5883L.h"

HMC5883L compas;

void setup(){
  Serial.begin(115200);
  
  Wire.begin();

  compas.registerA.setMesurementConfiguration(Normal);
  compas.registerA.setDataOutputRate(rate15Hz);
  compas.registerA.setSamplesAveraged(eight);  

  compas.registerB.setGain(gain1090);
  compas.registerMode.setOperatingMode(ContinuousMeasurement);
  compas.write();
}


void loop(){

  delay(1000);// the data output registers are updated with the new measurement. 
  Serial.println("------------TICK---------------------");
  compas.readAll();
  compas.print();
 
}



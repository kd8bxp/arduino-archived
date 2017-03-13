#include "HMC5883L.h"
#include "common.h"

String IdentificationRegister::toString(){
  String ret=ByteRegister::toString();
  ret+="'"; ret+=(char)data; ret+="' "; 
  ret+=String(data,DEC);
  if(data==defaultData) {ret+=" OK";}else{ret+=" ERROR";}
  return ret;
}

void HMC5883L::readAll(){
  //config
  registerA.read();
  registerB.read();
  registerMode.read();
  //values
  xAxis.read();
  zAxis.read();
  yAxis.read();
  //status
  statusRegisterMode.read();
  identificationRegisterA.read();
  identificationRegisterB.read();
  identificationRegisterC.read();
}

void HMC5883L::print(){
  Serial.println("::::::DUMP BEGIN:::::::::");
  Serial.println(registerA.toString());
  Serial.println(registerB.toString());
  Serial.println(registerMode.toString());
  
  Serial.println(xAxis.toString());
  Serial.println(zAxis.toString());
  Serial.println(yAxis.toString());
  
  Serial.println(statusRegisterMode.toString());
  Serial.println(identificationRegisterA.toString());
  Serial.println(identificationRegisterB.toString());
  Serial.println(identificationRegisterC.toString());
  Serial.println("::::::DUMP END:::::::::");
}

void HMC5883L::readValues(){
  xAxis.read();
  zAxis.read();
  yAxis.read();
}

void HMC5883L::write(){
  registerA.write();
  registerB.write();
  registerMode.write();
}
  

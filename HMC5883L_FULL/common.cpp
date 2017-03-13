#include "common.h"
#include "Wire.h"
  
String byteToString(const byte & data){
  String ret="";
    //HEX
    char buf[4]={0,0,0,0};
    sprintf(buf, "%02X", data);
    ret+=" (0x"+String(buf)+")";
    //BIN
    ret+="0b";
    for(int i=7;i!=-1;i--){  ret+=(bitRead(data,i)==1) ? "1" : "0";  }
  return ret;
}

String intToString(const int & data){
  String ret="";
    //HEX
    char buf[8]={0,0,0,0,0,0,0,0};
    sprintf(buf, "%04X", data);
    ret+=" (0x"+String(buf)+")";
    //BIN
    ret+="0b";
    for(int i=16;i!=-1;i--){  ret+=(bitRead(data,i)==1) ? "1" : "0";  }  
  return ret;
}


int8_t readByte(const uint8_t & adress)
{
    Wire.beginTransmission(HMC5883L_Address);
        Wire.write(adress);
    Wire.endTransmission();

    Wire.beginTransmission(HMC5883L_Address);
    Wire.requestFrom(HMC5883L_Address, 2);
      while(!Wire.available()) {};
      uint8_t ret = Wire.read();
    Wire.endTransmission();
#ifdef DEBUG
    Serial.print("readByte->");
    Serial.print(" from: 0x");
    Serial.print(adress,HEX);
    Serial.print(" val: ");
    Serial.println(byteToString(ret));
#endif
    return ret;
}

int8_t  writeByte(const uint8_t &  adress, const uint8_t & value){
    Wire.beginTransmission(HMC5883L_Address);
        Wire.write(0x3C);
        Wire.write(adress);
        Wire.write(value);
    Wire.endTransmission();
#ifdef DEBUG
    Serial.print("writeByte<-");
    Serial.print(" from: 0x");
    Serial.print(adress,HEX);
    Serial.print(" val: ");
    Serial.println(byteToString(value));
#endif
}

int16_t readInt(const uint8_t & adress)
{
    Wire.beginTransmission(HMC5883L_Address);
        Wire.write(adress);
    Wire.endTransmission();

    Wire.beginTransmission(HMC5883L_Address);
    Wire.requestFrom(HMC5883L_Address, 2);
      while(!Wire.available()) {};
      uint8_t vha = Wire.read();
      uint8_t vla = Wire.read();
    Wire.endTransmission();
    int16_t ret =vha << 8 | vla;
#ifdef DEBUG
    Serial.print("readInt->");
    Serial.print(" from: 0x");
    Serial.print(adress,HEX);    
    Serial.print(",0x"); 
    Serial.print(adress+1,HEX);
    Serial.print(" val: ");
    Serial.println(intToString(ret));
#endif
    return ret;
}
  

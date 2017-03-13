
#ifndef COMMON_H
#define COMMON_H true

#include <Arduino.h>

//7-bit address (0x1E) 
//plus 1 bit read/write identifier, i.e.
//0x3D for read  (‭0b00111101‬) and
//0x3C for write (‭0b00111100‬)
#define HMC5883L_Address 0x1E
  
String byteToString(const byte & data);
String intToString(const int & data);

int8_t  readByte(const uint8_t &  adress);
int8_t  writeByte(const uint8_t &  adress,const uint8_t & val);
int16_t readInt(const uint8_t &  adress);

struct ByteRegister{
  
  const byte address;
  const byte defaultData;
  byte data;

  ByteRegister(byte ADDRES,byte DEFAULT_Data):
    address(ADDRES),
    defaultData(DEFAULT_Data)
  {data=0;}
  
  String toString(){
    return "0x"+String(address,HEX)+":"+byteToString(data)+" "+String(data,DEC);
  }

  void read(){
    data=readByte( address );
  }
  void write(){
    writeByte( address,data);
  }
};

struct IntRegister{

  int data;
  const byte address;

  IntRegister(byte a):address(a){};
  
  String toString(){
    return "0x"+String(address,HEX)+":"+intToString(data)+" "+String(data,DEC);
  }

  void read(){
    data=readInt( address );
  }
};

#endif

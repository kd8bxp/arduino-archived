#include "StatusRegisterMode.h"

//==============================================================================
DataOutputRegisterLock StatusRegisterMode::getDataOutputRegisterLock(){
  return DataOutputRegisterLock(
    bitRead(data,0) << 0 //SR0
    |
    bitRead(data,1) << 1 //SR1
  );
}
//==============================================================================

//==============================================================================
byte StatusRegisterMode::getReserved(){
  return (
    bitRead(data,2) << 0 //SR2
    |
    bitRead(data,3) << 1 //SR3
    | 
    bitRead(data,4) << 2 //SR4
    | 
    bitRead(data,5) << 3 //SR5
    | 
    bitRead(data,6) << 4 //SR6
    | 
    bitRead(data,7) << 5 //SR7
  );
}
//==============================================================================

//==============================================================================
String StatusRegisterMode::toString(){
  String ret=ByteRegister::toString();
  ret+=" status: ";
  switch(getDataOutputRegisterLock()){
    case RDY:  ret+="READY"; break;
    case LOCK: ret+="LOCK"; break;
    default: ret+="ERROR"; break;
  }
  ret+=" Reserved: ";
  ret+=String(getReserved(),DEC);
  return ret;
}
//==============================================================================

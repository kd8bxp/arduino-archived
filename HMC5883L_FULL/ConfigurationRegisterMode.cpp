#include "ConfigurationRegisterMode.h"

//==============================================================================
OperatingMode ConfigurationRegisterMode::getOperatingMode(){
  return OperatingMode(
    bitRead(data,0) << 0 //GN0
    |
    bitRead(data,1) << 1 //GN1
  );
}

void ConfigurationRegisterMode::setOperatingMode(OperatingMode om){
   bitWrite(data,0, bitRead(om,0) );//MD0
   bitWrite(data,1, bitRead(om,1) );//MD1
   bitWrite(data,7, 0 );//MD7 set to 0 when configuring mode register. 
}
//==============================================================================

//==============================================================================
byte ConfigurationRegisterMode::getReserved(){
  return(
    bitRead(data,2) << 0 //MR2
    |
    bitRead(data,3) << 1 //MR3
    |
    bitRead(data,4) << 2 //MR4
    |
    bitRead(data,5) << 3 //MR5
    |
    bitRead(data,6) << 4 //MR6
    | 
    bitRead(data,7) << 5 //MR7
  ); 
}
//==============================================================================

String ConfigurationRegisterMode::toString(){
  String ret=ByteRegister::toString();

  ret+=" OperatingMode: ";
  switch(getReserved()){
    case ContinuousMeasurement:  ret+="continuous-measurement";  break;
    case SingleMeasurement:      ret+="single-measurement(default)";  break;
    case Idle1:
    case Idle2:  ret+="idle";  break;
  };

  ret+=" Reserved: ";
  ret+=String(getReserved(),DEC);
  return ret;
}

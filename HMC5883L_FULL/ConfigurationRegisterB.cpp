#include "ConfigurationRegisterB.h"

//=============================================================================
Gain ConfigurationRegisterB::getGain(){
  return Gain(
    bitRead(data,5) << 0 //GN0
    |
    bitRead(data,6) << 1 //GN1
    |
    bitRead(data,7) << 2 //GN2
  );
}

void ConfigurationRegisterB::setGain(Gain gain){
   bitWrite(data,5, bitRead(gain,0) );//GN0
   bitWrite(data,6, bitRead(gain,1) );//GN1
   bitWrite(data,7, bitRead(gain,2) );//GN1
}
//==============================================================================


//==============================================================================
byte ConfigurationRegisterB::getReserved(){
  return(
    bitRead(data,0) << 0 //CRB0
    |
    bitRead(data,1) << 1 //CRB1
    |
    bitRead(data,2) << 2 //CRB2
    |
    bitRead(data,3) << 3 //CRB3
    | 
    bitRead(data,4) << 4 //CRB4
  ); 
}
//==============================================================================


float ConfigurationRegisterB::getMgPerDigit()
{
  switch(getGain())
  {
    case gain1370: return 0.073f;  
    case gain1090: return 0.92f;  
    case gain820:  return 1.22f;  
    case gain660:  return 1.52f;
    case gain440:  return 2.27f;  
    case gain390:  return 2.56f;
    case gain330:  return 3.03f;
    case gain220:  return 4.35f;
  }
  return 0.0f;
}

String ConfigurationRegisterB::toString(){
  String ret=ByteRegister::toString();

  ret+=" Reserved: ";
  switch(getReserved()){
    case 0:  ret+="0(default)";  break;
    default: ret+="ERROR"; break;
  };

  ret+=" Gain: ";
  switch(getGain()){
    case gain1370: ret+="1370"; break;
    case gain1090: ret+="1090(default)"; break;
    case gain820:  ret+="820"; break;
    case gain660:  ret+="660"; break;
    case gain440:  ret+="440"; break;
    case gain390:  ret+="390"; break;
    case gain330:  ret+="330"; break;
    case gain220:  ret+="220"; break;
  };

  return ret;
}



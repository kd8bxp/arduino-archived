#include "ConfigurationRegisterA.h"

//==============================================================================
MeasurementMode ConfigurationRegisterA::getMesurementConfiguration(){
  return MeasurementMode(
    bitRead(data,0) << 0 //MS0
    |
    bitRead(data,1) << 1 //MS1
  );
}

void ConfigurationRegisterA::setMesurementConfiguration(MeasurementMode measurementConf){
   bitWrite(data,0, bitRead(measurementConf,0) );//MS0
   bitWrite(data,1, bitRead(measurementConf,1) );//MS1
}
//==============================================================================


//==============================================================================
DataOutputRate ConfigurationRegisterA::getDataOutputRate(){
  return DataOutputRate(
    bitRead(data,2) << 0 //DO0
    |
    bitRead(data,3) << 1 //DO1
    |
    bitRead(data,4) << 2 //DO2
  );
}

void ConfigurationRegisterA::setDataOutputRate(DataOutputRate dor){
   bitWrite(data,2, bitRead(dor,0) );//DO0
   bitWrite(data,3, bitRead(dor,1) );//DO1
   bitWrite(data,4, bitRead(dor,2) );//DO2
} 
//==============================================================================


//==============================================================================
SamplesAveraged ConfigurationRegisterA::getSamplesAveraged(){
  return SamplesAveraged(
    bitRead(data,5) << 0 //MA0
    |
    bitRead(data,6) << 1 //MA1
  );      
}

void ConfigurationRegisterA::setSamplesAveraged(SamplesAveraged sa){
  bitWrite(data,5, bitRead(sa,0) );//MA0
  bitWrite(data,6, bitRead(sa,1) );//MA1
}
//==============================================================================


//==============================================================================  
bool ConfigurationRegisterA::getReservedBit(){
  return (bitRead(data,7)==1);
}
//==============================================================================

  
String ConfigurationRegisterA::toString(){
  String ret=ByteRegister::toString();

  ret +=" MeasurementMode: ";
  switch(getMesurementConfiguration()){
    case Normal       : ret+="normal (default)"; break;
    case PositiveBias : ret+="positiveBias"; break;
    case NegativeBias : ret+="negativeBias"; break;
    case reserver     : ret+="reserver"; break;
  };

  ret+=" DataOutputRate: ";
  switch(getDataOutputRate()){
    case rate0_75Hz: ret+="0.75Hz"; break;
    case rate1_5Hz : ret+="1.5Hz"; break;
    case rate3Hz   : ret+="3Hz"; break;
    case rate7_5Hz : ret+="7.5Hz"; break;
    case rate15Hz  : ret+="15Hz (default)"; break;
    case rate30Hz  : ret+="30Hz"; break;
    case rate75Hz  : ret+="75Hz"; break;
    case reserved  : ret+="Reserved"; break;
  };

  ret+=" SamplesAveraged: ";
  switch(getSamplesAveraged()){
    case zero:  ret+="0 (default)"; break;
    case two:   ret+="2"; break;
    case four:  ret+="4"; break;
    case eight: ret+="8"; break;
  };

  ret+=" ReservedBit: ";
  switch(getReservedBit()){
    case true:  ret+="on";  break;
    case false: ret+="off (default)"; break;
  }

  return ret;
}


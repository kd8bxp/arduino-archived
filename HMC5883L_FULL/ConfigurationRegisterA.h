#ifndef ConfigurationRegisterA_H
#define ConfigurationRegisterA_H true

#include "common.h"


/* 
 *  PURPOSE: Select measurement modes 
 *  SIZE: 2bit
 *  ADDRESS: CRA1,CRA0
 *  SOURCE: Table 6: Measurement Modes
 *  0 0 Normal measurement configuration (Default).  In normal measurement configuration the device follows normal measurement flow.  The positive and negative pins of the resistive load are left floating and high impedance.  
 *  0 1 Positive bias configuration for X, Y, and Z axes. In this configuration, a positive current is forced across the resistive load for all three axes. 
 *  1 0 Negative bias configuration for X, Y and Z axes. In this configuration, a negative current is forced across the resistive load for all three axes.. 
 *  1 1 This configuration is reserved.
*/
enum MeasurementMode {
  Normal       = B00000000,//default
  PositiveBias = B00000001,
  NegativeBias = B00000010,
  reserver     = B00000011
};

/* 
 *  PURPOSE: Typical Data Output Rate (Hz) 
 *  SIZE: 3bit
 *  ADDRESS: CRA2,CRA3,CRA4
 *  SOURCE: Table 5: Data Output Rates
 * 0 0 0 0.75 
 * 0 0 1 1.5 
 * 0 1 0 3 
 * 0 1 1 7.5 
 * 1 0 0 15 (Default) 
 * 1 0 1 30 
 * 1 1 0 75 
 * 1 1 1 Reserved
 */
enum DataOutputRate {
  rate0_75Hz = B00000000,//0.75Hz
  rate1_5Hz  = B00000001,//1.5Hz
  rate3Hz    = B00000010,//3Hz
  rate7_5Hz  = B00000011,//7.5Hz
  rate15Hz   = B00000100,//15Hz (default)
  rate30Hz   = B00000101,//30Hz
  rate75Hz   = B00000110,//75Hz
  reserved   = B00000111,//Reserved 
};

/*
 * PURPOSE: Number of samples averaged (1 to 8) per measurement output.
 * SIZE: 2bit
 * ADDRESS: CRA5,CRA6
 * SOURCE: Table 4: Configuration Register A Bit Designations
 */
enum SamplesAveraged{
  zero  = B00000000,
  two   = B00000001,//Default
  four  = B00000010,
  eight = B00000011
};

/*
 * PURPOSE: Reserved for future function.
 * SIZE: 1bit
 * ADDRESS: CRA7
 * SOURCE: Table 4: Configuration Register A Bit Designations
 */


/*
 * The configuration register is used to configure the device for setting the data output rate and measurement configuration.  
 * CRA0 through CRA7 indicate bit locations, with CRA denoting the bits that are in the configuration register. 
 * CRA7 denotes the first bit of the data stream.  
 * default is 0x10. 
 * 
 * data structure (one byte)
 * |    CRA7   |        CRA6     |     CRA5       |       CRA4       |       CRA3      |     CRA2        |        CRA1       |       CRA0       |
 * [ reserved  || sampleAverate1 | sampleAverate0 || DataOutputRate2 | DataOutputRate1 | DataOutputRate0 || MeasurementConf1 | MeasurementConf0 ]
 */
struct ConfigurationRegisterA: ByteRegister
{
  ConfigurationRegisterA(): ByteRegister(0x00,0x10) {}

  ConfigurationRegisterA& operator=(const byte&b) {data=b;};
  
  MeasurementMode getMesurementConfiguration();
  void setMesurementConfiguration(MeasurementMode);
  
  DataOutputRate getDataOutputRate();
  void setDataOutputRate(DataOutputRate);
  
  SamplesAveraged getSamplesAveraged();
  void setSamplesAveraged(SamplesAveraged);

  bool getReservedBit();

  String toString();
};

#endif

#ifndef ConfigurationRegisterB_H
#define ConfigurationRegisterB_H true

#include "common.h"

/*
 * PURPOSE: These bits must be cleared for correct operation. 
 * ADDRESS: CRB0,CRB1,CRB2,CRB3 
 */


/*
 * PURPOSE: Gain Configuration Bits. 
 * ADDRESS: CRB5,CRB6,CRB7
GN2 GN1 GN0 | Recommended Sensor Field Range | Gain (LSb/ Gauss) |Digital Resolution (mG/LSb) | Output Range 
 0   0   0  | ± 0.88 Ga                      | 1370              | 0.73                       | 0xF800–0x07FF (-2048–2047 ) 
 0   0   1  | ± 1.3  Ga                      | 1090              | 0.92                       | 0xF800–0x07FF (-2048–2047 ) (default)
 0   1   0  | ± 1.9  Ga                      | 820               | 1.22                       | 0xF800–0x07FF (-2048–2047 ) 
 0   1   1  | ± 2.5  Ga                      | 660               | 1.52                       | 0xF800–0x07FF (-2048–2047 ) 
 1   0   0  | ± 4.0  Ga                      | 440               | 2.27                       | 0xF800–0x07FF (-2048–2047 ) 
 1   0   1  | ± 4.7  Ga                      | 390               | 2.56                       | 0xF800–0x07FF (-2048–2047 ) 
 1   1   0  | ± 5.6  Ga                      | 330               | 3.03                       | 0xF800–0x07FF (-2048–2047 ) 
 1   1   1  | ± 8.1  Ga                      | 230               | 4.35                       | 0xF800–0x07FF (-2048–2047 ) 
 */
enum Gain{
  gain1370 = 0b00000000,
  gain1090 = 0b00000001,//(default)
  gain820  = 0b00000010,
  gain660  = 0b00000011,
  gain440  = 0b00000100,
  gain390  = 0b00000101,
  gain330  = 0b00000110,
  gain220  = 0b00000111,
};

/*
 * The configuration register B for setting the device gain.  
 * CRB0 through CRB7 indicate bit locations, with CRB denoting the bits that are in the configuration register. 
 * CRB7 denotes the first bit of the data stream.  
 * default is 0x20. 
 */
struct ConfigurationRegisterB : ByteRegister{

  ConfigurationRegisterB():       ByteRegister(0x01,0x20){}

  ConfigurationRegisterB& operator=(const byte&b) {data=b;};
  //==============================================================================
  //  CRB7 to CRB5 GN2 to GN0 
  //Gain Configuration Bits. 
  //These bits configure the gain for the device. 
  //The gain configuration is common for all channels.
  Gain getGain();
  void setGain(Gain gain);
  //==============================================================================


  //==============================================================================
  //CRB4 to CRB0 0 These bits must be cleared for correct operation. 
  byte getReserved();
  //setReserved() //NO! These bits must be cleared for correct operation
  //=================================================================
  
  float getMgPerDigit();

  String toString();
};


#endif

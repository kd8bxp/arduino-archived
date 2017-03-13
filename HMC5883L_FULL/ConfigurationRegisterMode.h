#ifndef ConfigurationRegisterMode_H
#define ConfigurationRegisterMode_H true

#include "common.h"

/*
 * ADDRESS: MD1 MD0 Operating Mode.
 * PURPOSE: Mode Select Bits.
0 0 Continuous-Measurement Mode.  In continuous-measurement mode, the device continuously performs measurements and places the result in the data register.  RDY goes high when new data is placed in all three registers.  After a power-on or a write to the mode or configuration register, the first measurement set is available from all three data output registers after a period of 2/fDO and subsequent measurements are available at a frequency of fDO, where fDO is the frequency of data output. 
0 1 Single-Measurement Mode (Default).  When single-measurement mode is selected, device performs a single measurement, sets RDY high and returned to idle mode.  Mode register returns to idle mode bit values.  The measurement remains in the data output register and RDY remains high until the data output register is read or another measurement is performed.
1 0 Idle Mode.  Device is placed in idle mode. 
1 1 Idle Mode.  Device is placed in idle mode.
*/
enum OperatingMode {
  ContinuousMeasurement = 0b00000000,
  SingleMeasurement     = 0b00000001,//(Default)
  Idle1                 = 0b00000010,
  Idle2                 = 0b00000011
};

/*
 * ADDRESS: MD2,MD3,MD4,MD5,MD6
 * PURPOSE: -
*/

/*
 * ADDRESS: MD7 MD0 Operating Mode  
 * PURPOSE: Bit MR7 is set to 1 internally after each singlemeasurement operation. Set to 0 when configuring mode register. 
*/

/*
 * The mode register is an 8-bit register from which data can be read or to which data can be written.  
 * This register is used to select the operating mode of the device.  
 * MR0 through MR7 indicate bit locations, with MR denoting the bits that are in the mode register. 
 * MR7 denotes the first bit of the data stream.  
 * default is 0x01. 
 */
struct ConfigurationRegisterMode: ByteRegister{

  ConfigurationRegisterMode(): ByteRegister(0x02,0x01) {}

  ConfigurationRegisterMode& operator=(const byte&b) {data=b;};
  //==============================================================================
  OperatingMode getOperatingMode();
  void setOperatingMode(OperatingMode);
  //==============================================================================

  //==============================================================================
  byte getReserved();
  //setReserved() //NO! These bits must be cleared for correct operation  
  //==============================================================================

  String toString();
};

#endif


#ifndef StatusRegister_H
#define StatusRegister_H true

#include "common.h"

/*
 * PURPOSE: Status register.
 * ADDRESS: SR0,SR1
 * SOURCE: Table 17:  Status Register Bit Designations 
 * 
 *   SR1 LOCK  Data output register lock.      
 *   This bit is set when:    
 *   1.some but not all for of the six data output registers have been read, 
 *   2. Mode register has been read.   
 *   
 *   When this bit is set, the six data output registers are locked and any new data will not be placed in these register until one of these conditions are met: 
 *   1.all six bytes have been read, 
 *   2. the mode register is changed, 
 *   3. the measurement configuration (CRA) is changed, 
 *   4. power is reset. 
 *   
 *   SR0 RDY 
 *   Ready Bit.  
 *   Set when data is written to all six data registers.  
 *   Cleared when device initiates a write to the data output registers and after one or more of the data output registers are written to.  
 *   When RDY bit is clear it shall remain cleared for a 250 μs.  
*/
enum DataOutputRegisterLock{
  RDY  = 0b00000001,
  LOCK = 0b00000010,    
};

/*
 * PURPOSE: These bits are reserved.
 * ADDRESS: SR7 to SR2
 * SOURCE: Table 17:  Status Register Bit Designations 
 */


struct StatusRegisterMode: ByteRegister{

  StatusRegisterMode(): ByteRegister(0x09,0x00) {}

  StatusRegisterMode& operator=(const byte&b) {data=b;};

  //==============================================================================
  DataOutputRegisterLock getDataOutputRegisterLock();
  //==============================================================================

  //==============================================================================
  byte getReserved();
  //==============================================================================

  String toString();
};

#endif

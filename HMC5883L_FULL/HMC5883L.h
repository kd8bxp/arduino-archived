#ifndef HMC5883L_H
#define HMC5883L_H true

#include "common.h"
#include "ConfigurationRegisterA.h"
#include "ConfigurationRegisterB.h"
#include "ConfigurationRegisterMode.h"
#include "StatusRegisterMode.h"



//more info: 
//https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
//https://www.sparkfun.com/tutorials/301
//http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-magnetometr-hmc5883l.html
//https://github.com/jarzebski/Arduino-HMC5883L


/*
 * The default (factory) HMC5883L 8-bit slave address is 0x3C for write operations, or 0x3D for read operations.
 * 
 * reading from and writing to this device.  
 * The devices uses an address pointer to indicate which register location is to be read from or written to.  
 * These pointer locations are sent from the master to this slave device and succeed 
 * the 7-bit address (0x1E) ‭00011110‬ plus 1 bit read/write identifier, 
 * i.e. 0x3D for read  ‭00111101‬
 * and  0x3C for write ‭00111100‬
 * 
 * To minimize the communication between the master and this device, the address pointer updated automatically without master intervention. The register pointer will be incremented by 1 automatically after the current register has been read successfully.  
 * 
 * WTF: Send 0x3D 0x06 (Read all 6 bytes. If gain is changed then this data set is using previous gain) Convert three 16-bit 2’s compliment hex values to decimal values and assign to X, Z, Y, respectively. 
 * 
 * Address Location Name Access 
 * 00 Configuration Register A  Read/Write 
 *   Write CRA (00) – send 0x3C 0x00 0x70 (8-average, 15 Hz default, normal measurement)
 * 01 Configuration Register B  Read/Write 
 *   Write CRB (01) – send 0x3C 0x01 0xA0 (Gain=5, or any other desired gain) 
 * 02 Mode Register Read/Write 
 *   Write Mode (02) – send 0x3C 0x02 0x00 (Continuous-measurement mode) 
 *   write 0x3C 0x02 0x00 
 * 03 Data Output X MSB Register Read 
 *   read: 0x3C 0x03 (point to first data register 03)
 * 04 Data Output X LSB Register Read 
 * 05 Data Output Z MSB Register Read 
 * 06 Data Output Z LSB Register Read 
 * 07 Data Output Y MSB Register Read 
 * 08 Data Output Y LSB Register Read 
 * 09 Status Register Read
 * 10 Identification Register A Read   (For example, to move the address pointer to register 10, send 0x3C 0x0A. )
 * read: 0x9 0000‭1001‬
 * 11 Identification Register B Read 
 * read: 0xA ‭00001010‬
 * 12 Identification Register C Read 
 * read: 0xB ‭00001011
 * 
 * If (address pointer = 08) then address pointer = 03
 * Else if (address pointer >= 12) then address pointer = 0
 * Else (address pointer) = (address pointer) + 1
 * 
 * The register pointer will be incremented by 1 automatically after the current register has been read successfully.
 */


struct IdentificationRegister: ByteRegister {
  IdentificationRegister(byte ad,byte def):ByteRegister(ad,def){}
  
  IdentificationRegister& operator=(const byte&b) {data=b;};

  String toString();
};


struct HMC5883L{
  //configuration
  ConfigurationRegisterA registerA;
  ConfigurationRegisterB registerB;
  ConfigurationRegisterMode registerMode;

  //values
  IntRegister xAxis;
  IntRegister zAxis;
  IntRegister yAxis;

  //status
  StatusRegisterMode statusRegisterMode;
  IdentificationRegister identificationRegisterA;
  IdentificationRegister identificationRegisterB;
  IdentificationRegister identificationRegisterC;
  
  HMC5883L(): 
    xAxis(0x03), 
    zAxis(0x05),
    yAxis(0x07),
    identificationRegisterA(0xA,0x48),
    identificationRegisterB(0xB,0x34),
    identificationRegisterC(0xC,0x33)
  { }

  void readAll();
  void readValues();

  //dump all registers to serial
  void print();

  //write (writable registers)
  void write();
};
#endif

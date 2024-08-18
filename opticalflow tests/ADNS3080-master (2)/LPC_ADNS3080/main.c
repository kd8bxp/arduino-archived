
#include <intrinsics.h>
#include <stdio.h>
#include <NXP/iolpc2103.h>
#include "types.h"
#include "serial.h"
#include "delay.h"
#include "digital.h"
#include "spi.h"
#include "ADNS3080.h"

#define CSPIN 7   // P0.7 is the SS pin

unsigned int surface_quality;
int raw_dx;
int raw_dy;
boolean _motion;
boolean _overflow;

void write_register(byte address, byte value);
byte read_register(byte address);
boolean initOF(void);
boolean updateOF();

int main()
{
  ///////// System Clock Initialization /////////
  // Fosc = 12MHz
  // MSEL=4, PSEL=1
  // PLLCFG=0100100=0x24=36
  // cclk = 60MHz
  // Fcco = 240MHz
  PLLCFG = 0x24;
  // Enable PLL
  PLLCON = 0x1;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // Wait for PLL to achieve lock
  while(!(PLLSTAT & 0x400));
  // Connect PLL
  PLLCON=0x3;
  PLLFEED=0xAA;
  PLLFEED=0x55;

  ///////// Peripheral Clock Initialization /////////
  // Set periphery divider
  // 0x00 => /4
  // 0x01 => /1
  // 0x02 => /2
  APBDIV_bit.APBDIV = 0x02;

  serialInit(UART0, 115200);
  
  if (initOF() == false) {
    serialPuts(UART0, "SPI Init Error\n");
  }

  while (1) {    
    updateOF();  
    if (_motion == false)
      serialPuts(UART0, "no ");
    serialPuts(UART0, "motion\n");
    if (_overflow == true)
      serialPuts(UART0, "overflow\n");
    serialPuts(UART0, "squal: ");
    serialPuti(UART0, surface_quality);
    serialPutc(UART0, '\n');
    serialPuti(UART0, raw_dx);
    serialPutc(UART0, '\n');
    serialPuti(UART0, raw_dx);
    serialPutc(UART0, '\n');
    delay(1000);
  }
  
}



/** Read a register from the sensor */
byte read_register(byte address)
{
  byte result = 0;

  // take the chip select low to select the device
  digitalWrite(CSPIN, LOW);

  // send the device the register you want to read:
  SPI0_transfer(address);

  // small delay
  delay(50);

  // send a value of 0 to read the first byte returned:
  result = SPI0_transfer(0x00);

  // take the chip select high to de-select:
  digitalWrite(CSPIN, HIGH);

  return result;
}

/** write a value to one of the sensor's registers */
void write_register(byte address, byte value)
{
  // take the chip select low to select the device
  digitalWrite(CSPIN, LOW);

  // send register address
  SPI0_transfer(address | 0x80 );

  // small delay
  delay(50);

  // send data
  SPI0_transfer(value);

  // take the chip select high to de-select:
  digitalWrite(CSPIN, HIGH);
}


// init - initialise sensor
// initCommAPI parameter controls whether SPI interface is initialised (set to false if other devices are on the SPI bus and have already initialised the interface)
boolean initOF()
{
  int retry = 0;

  digitalWrite(CSPIN,HIGH);                 // disable device (Chip select is active low)
  SPI0_Init();

  // reset the device
  //reset();

  // check the sensor is functioning
  if( retry < 3 ) {
    if( read_register(ADNS3080_PRODUCT_ID) == 0x17 )
      return true;
    retry++;
  }

  return false;
}

/** update motion */
boolean updateOF()
{
  byte motion_reg;
  surface_quality = (unsigned int)read_register(ADNS3080_SQUAL);
  delay(50);  // small delay

  // check for movement, update x,y values
  motion_reg = read_register(ADNS3080_MOTION);
  _overflow = ((motion_reg & 0x10) != 0);  // check if we've had an overflow
  if( (motion_reg & 0x80) != 0 ) {
    raw_dx = ((char)read_register(ADNS3080_DELTA_X));
    delay(50);  // small delay
    raw_dy = ((char)read_register(ADNS3080_DELTA_Y));
    delay(50);
    _motion = true;
  }else{
    raw_dx = 0;
    raw_dy = 0;
  }
  //last_update = millis();

  return true;
}


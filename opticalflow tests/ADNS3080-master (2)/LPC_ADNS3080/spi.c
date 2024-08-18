#include <NXP/iolpc2103.h>
#include "types.h"

#define SPIF (1<<7)

/** Init SP0 for use with the ADNS3080 */
// TODO: Add speed option
// TODO: Add SPI1
void SPI0_Init()
{
 /* configure pin connect block
  * 15:14 == 00 ->> GPIO (master mode) P0.7
  * 13:12 == 01 ->> MOSI0, P0.6
  * 11:10 == 01 ->> MISO0, P0.5
  *  9:8  == 01 ->> SCK0,  P0.4
  */ 
  PINSEL0 |= 0x1500;
  // pin direction is controlled automatically if not GPIO
  IODIR |= (1<<7); // P0.7 ->> SS

 /* Set SPI speed in SPI Clock Counter Register
  * The SPI0 rate may be calculated as: PC LK / SPCCR0 value. The PCLK rate is
  * CCLK /APB divider rate as determined by the APBDIV register contents.
  * Assuming pclk = 60MHz/2 = 30MHz, /16 = 1.875MHz, 2Mhz max speed for ADNS3080
  * SPCCR must be even and > 8
  */
  S0SPCCR=16;

  /* Device selected as master in SPI Control Register */
  S0SPCR=0x20;  
}


/** Transfer a byte of data over SPI0 */
byte SPI0_transfer(byte data)
{
  /* write data out */
  S0SPDR=data;
  /* wait for transfer to compelte*/
  while(!(S0SPSR & SPIF));
  
  /* read data */
  return S0SPDR;
}
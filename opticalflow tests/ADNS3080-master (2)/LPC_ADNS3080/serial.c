#include <intrinsics.h>
#include <stdio.h>
#include <NXP/iolpc210x.h>

#define TEMT       (1<<6) 

void serialInit(unsigned char dev, unsigned int baud)
{ 
  unsigned char error=0;
  unsigned int fdr, dll, dlm;

  /* settings from http://prototalk.net/forums/showthread.php?t=11 */
  /* based on 30MHz PCLK */
  
  /* this is inelegant but who cares */
  switch (baud) {
  case 115200 : // verified on PC
    fdr=0xe5;
    dlm=0x0;
    dll=0x0c;
    break;
  case 57600 : // verified on PC
    fdr=0x75;
    dlm=0x0;
    dll=0x13;
    break;
  case 38400 : // verified on PC
    fdr=0x72;
    dlm=0x0;
    dll=0x26;
    break;
  case 28800 : // verified on PC
    fdr=0xd5;
    dlm=0x00;
    dll=0x2f;
    break;
  case 19200 : // verified on PC
    fdr=0x83;
    dlm=0x0;
    dll=0x47;
    break;
  case 14400 : // verified on PC
    fdr=0x52;
    dlm=0x0;
    dll=0x5d;
    break;
  case 9600 : // verified on PC
    fdr=0xb1;
    dlm=0;
    dll=0xb3;
    break;
  case 4800 : // verified on PC
    fdr=0x52;
    dlm=0x01;
    dll=0x17;
    break;
  case 2400 : // verified on PC
    fdr=0x31;
    dlm=0x02;
    dll=0x4a;
    break;
  case 1200 : // verified on PC
    fdr=0x21;
    dlm=0x04;
    dll=0x12;
    break;
  case 300 : // verified on PC
    fdr=0x10;
    dlm=0x18;
    dll=0x6a;
    break;
  default:
    error = 1;
    break;
  }
  
  if (error == 0) {
    switch (dev) {
    case 0 :
      /* Initialize Pin Select Block for Tx and Rx */ 
      PINSEL0=0x5; // 3:2,1:0 = %01
      /* Enable FIFO's and reset them */ 
      U0FCR=0x7;   
      /* Set DLAB and word length set to 8bits */ 
      U0LCR=0x83; // 8 bits, 1 stop, no parity, DLAB = 1         
      /* Set baud rate */
      U0FDR = fdr; 
      U0DLL = dll;      
      U0DLM = dlm;     
      /* Clear DLAB */     
      U0LCR=0x03; // 8 bits, 1 stop, no parity, DLAB = 0         
      break;
    case 1 :
      /* Initialize Pin Select Block for Tx and Rx */ 
      PINSEL0=0x50000; // 19:18, 17:16 = %01
      /* Enable FIFO's and reset them */ 
      U1FCR=0x7;   
      /* Set DLAB and word length set to 8bits */ 
      U1LCR=0x83; // 8 bits, 1 stop, no parity, DLAB = 1         
      /* Set baud rate */
      U1FDR = fdr; 
      U1DLL = dll;      
      U1DLM = dlm;     
      /* Clear DLAB */     
      U1LCR=0x03; // 8 bits, 1 stop, no parity, DLAB = 0         
    default :
      break;
    }
  }
  
} // serialInit()


unsigned char serialEmptyTX(unsigned char dev)
{
  unsigned char result;
  
  switch (dev) {
  case 0 :
    result = (U0LSR & TEMT);
    break;
  case 1 :
    result = (U1LSR & TEMT);
    break;
  default :
    break;
  }
  return result;
}


void serialPutc(unsigned char dev, char c)
{
  switch (dev) {
  case 0 :
    U0THR=c; 
    break;
  case 1 :
    U1THR=c;
  default :
    break;
  }
  while (!serialEmptyTX(dev))
  return;
}

void serialPuti(unsigned char dev, int i)
{
  char digit = 0;
  int place = 10000000;

  // Deal with negatives
  if (i < 0) {
    serialPutc(dev, '-');
    i = -i;
  }
  
  // Strip off the leading zeros
  while (i/place == 0 && place > 1)
    place /= 10;
    
  // Print the rest of the integer
  while (place) {
    digit = i / place;
    i %= place;
    serialPutc(dev, digit+'0');
    place /= 10;
  }
}


void serialPuts(unsigned char dev, char *c)
{
  while (*c)
    serialPutc(dev, *c++);
}



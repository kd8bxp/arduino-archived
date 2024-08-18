#ifndef __UART_H
#define __UART_H

#define UART0 0
#define UART1 1

void serialInit(unsigned char dev, unsigned int baud); 
void serialPutc(unsigned char dev, char c);
void serialPuti(unsigned char dev, int i);
void serialPuts(unsigned char dev, char *c);
unsigned char serialEmptyTX(unsigned char dev);

#endif
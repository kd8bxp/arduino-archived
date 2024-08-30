#ifndef ROBONII_MOTOR_H
#define ROBONII_MOTOR_H

/*
This Library is to be used with the Robonii product. 
Schematics can be downloaded from "www.robonii.co.za"
A sample of how to use this library is given at the end of "Driver_RFID.c"
The RFID chip can not be driven from USB power.
Ensure that the RFID antenna is plugged in, else the device might get stuck in an interupt trying to decode random pulses
*/




#include <avr/io.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>





//====================================================================
enum irpod_leds_state_e
{
  IRPOD_LED_OFF =0x00,
  IRPOD_LED_ON  =0x03,
  IRPOD_LED_FLASH_SLOW =  0x01,
  IRPOD_LED_FLASH_FAST  =0x02,
} ;

enum irpod_leds_e
{
  IRPOD_LED_YELLOW =0x00,
  IRPOD_LED_RED_RIGHT ,
  IRPOD_LED_RED_LEFT  ,     
  IRPOD_LED_BICOLOR_GREEN , 
  IRPOD_LED_BICOLOR_RED ,
} ;

typedef enum
{
  IRPOD_CMD_TEST = 1,
  IRPOD_CMD_GET_DEVICE,
  IRPOD_CMD_INIT,
  IRPOD_CMD_START_TX,
  IRPOD_CMD_SET_POD_LEDS,

  IRPOD_CMD_INIT_BS,
  IRPOD_CMD_SET_BS_LEDS,

  IRPOD_CMD_RESET,
  IRPOD_CMD_SET_CAL,
  IRPOD_CMD_GET_DATA,
} irPodCmd_e;

class Robonii_IRPod_Driver {
 public:
 unsigned char updated;
 Robonii_IRPod_Driver();
 void InitTWI(void);
 void IrpodReqVersion(void);
 void SetLED(irpod_leds_e led, irpod_leds_state_e state);
 unsigned char ReadData(unsigned char *chars);
 
 
 private:
 void InitTwiHeader(unsigned char irpodcmd);
  void IrPodSetLeds(uint8_t yellowLed,uint8_t leftMuzzleLed,uint8_t rightMuzzleLed,uint8_t greenLed,uint8_t redLed);
void IrPodSendCmd(irPodCmd_e cmd, unsigned char cmd_target, unsigned char data1, unsigned char data2, unsigned char data3);

 };
  
extern Robonii_IRPod_Driver Robonii_IRPod;

#endif

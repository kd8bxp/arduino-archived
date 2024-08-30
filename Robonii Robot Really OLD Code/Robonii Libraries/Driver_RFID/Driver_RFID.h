#ifndef DRIVER_RFID_H
#define DRIVER_RFID_H

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


// ====== Defines =====================================================

// Control CFE pin on RFID device
#define ENABLE_RFID_COIL	  PORTJ.OUTSET = 0x01;//digitalWrite(D1_Red, LOW);
#define DISABLE_RFID_COIL   PORTJ.OUTCLR = 0x01;//digitalWrite(D1_Red, HIGH)

#define RFID_DISABLE_INT_SAMPLE  TCF1.CTRLB &= ~(TC1_CCAEN_bm); TCF1.INTCTRLB &= ~ (0x03);//digitalWrite(D4_Green, HIGH)
#define RFID_ENABLE_INT_SAMPLE   TCF1.CTRLB = (TC1_CCAEN_bm); TCF1.INTCTRLB |= 0x03;//digitalWrite(D4_Green, LOW)


#define RFID_ENABLE_INT_EDGE  sbi(PORTJ.INT0MASK,1);//digitalWrite(D2_Red, LOW)
#define RFID_DISABLE_INT_EDGE cbi(PORTJ.INT0MASK,1);//digitalWrite(D2_Red, HIGH)


// Timing constants, used in powersave mode
// Units in 10ms
#define RFID_ON_THRESHOLD  20
#define RFID_PERIOD       100

// Sampling timer runs at peripheral clock / 64
// Peripheral clock = 32 Mhz -> sample at 500kHz, 256 ticks per bit
// Peripheral clock = 16 Mhz -> sample at 250kHz, 128 ticks per bit
// Peripheral clock = 15.5 Mhz -> sample at 242.1875 kHz, 124 ticks per bit
#define TICKS_PER_BIT   256
//#define SEARCH_MARGIN    20
#define SEARCH_MARGIN    50




enum e_rfid_state
  {
    RFID_DISABLED=1,
    RFID_ENABLED_ON,
    RFID_ENABLED_POWERSAVE
  } ; 

class Driver_RFID {
 public:
 unsigned char updated;
 Driver_RFID();
 void rfid_init(void);
 void driver_rfid_tmr_10ms(void);
 void rfid_task(void);
 void rfid_set_state(e_rfid_state new_state);
 void SaveRFID(void);
 void GetRFID(unsigned char *data);
 void rfid_reset_status(void);
 bool DataAvailable(void);
 
 volatile unsigned char rfid_flag_tag_data_updated;
volatile unsigned char rfid_flag_offtag_detected;
 };
  
extern Driver_RFID RFID;
  
#endif

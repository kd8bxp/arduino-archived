#ifndef DRIVER_RFID_H
#define DRIVER_RFID_H

#include <avr/io.h>
#include <compat/deprecated.h>

enum e_rfid_state
  {
    RFID_DISABLED=1,
    RFID_ENABLED_ON,
    RFID_ENABLED_POWERSAVE
  } ; 

 //extern Driver_RFID RFID
  
#endif

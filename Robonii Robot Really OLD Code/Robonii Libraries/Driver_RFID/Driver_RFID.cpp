#include "Driver_RFID.h"

//#include <avr/io.h>
//#include <compat/deprecated.h>


Driver_RFID RFID=Driver_RFID();
e_rfid_state rfid_state;

uint8_t rfid_disabled_counter;
uint8_t no_sync_timeout;
bool rfid_disabled_flag;

void rfid_reset_status(void);

volatile unsigned int rfid_int_time = 0x00;
volatile unsigned char rfid_sync_state = 0x00;
volatile unsigned char rfid_valid_signal_cntr = 0;
unsigned char rfid_sample = 0;

volatile unsigned char rfid_data[10], rfid_bufdata[5];
volatile unsigned char rfid_data_index = 0;
volatile unsigned char rfid_data_row_cntr = 0;
volatile unsigned char rfid_row_parity = 0;


//===================================================================
unsigned int time = 0;

SIGNAL(PORTJ_INT0_vect)
{
 // digitalWrite(D3_Green, LOW);
	// Capture elapsed time since last interrupt.
	rfid_int_time = TCF1.CNT;
time = rfid_int_time;
  // Clear Timer/Counter 0.
	TCF1.CNT = 0x0000;




  #ifdef TEST_INT_TIMING
    rfid_test_array[rfid_test_array_index] = rfid_int_time;
  	if(rfid_test_array_index++ > 19)
  		rfid_test_array_index = 0x00;
  #endif
  
  if(rfid_sync_state == 0)
  {
    //digitalWrite(D3_Red, LOW); //kom hier met of sonder tag...
    if((rfid_int_time >= TICKS_PER_BIT-SEARCH_MARGIN) && (rfid_int_time <= TICKS_PER_BIT+SEARCH_MARGIN))
    {
      //digitalWrite(D3_Red, LOW); // kom nie hier nie...
      rfid_valid_signal_cntr++;
      if(rfid_valid_signal_cntr == 7)
      {
        
        rfid_sync_state = 1;
        
        PORTJ.PIN1CTRL = PORT_ISC_FALLING_gc;
        // Clear possible interrupts caused by changing the sense control
        TCF1.INTFLAGS = 0x30;
      }
    }
    else
    {
      rfid_valid_signal_cntr = 0;
    }
    return;
  }
  else if(rfid_sync_state == 1)
  {
    PORTJ.PIN1CTRL = PORT_ISC_RISING_gc;
    // Clear possible interrupts caused by changing the sense control
    TCF1.INTFLAGS = 0x30;

    if(rfid_int_time < ((TICKS_PER_BIT/2) + SEARCH_MARGIN))
    {
      rfid_sync_state = 2;



      no_sync_timeout = 0; //clear sync timeout as a valid sync has been detected
    }
    else
      RFID.rfid_reset_status();

    return;
  }
  else if(rfid_sync_state == 2)
  {
    if(rfid_int_time > ((TICKS_PER_BIT/2) + SEARCH_MARGIN))
    {
      rfid_sync_state = 3;
      rfid_valid_signal_cntr = 0;
	  unsigned char i = 0;
      for( i = 0; i < 10; i++)
        rfid_data[i] = 0;

      // Should be reset.
      rfid_data_index = 0;
      rfid_row_parity = 0;

      // Disable edge interrupt.
//      cbi(PORTJ.INT0MASK,1);
      // Enable compare/capture A.
      RFID_ENABLE_INT_SAMPLE;


      // 1st sample interrupt should be 1/4 of bit from current point
      TCF1.CNT = TICKS_PER_BIT - (TICKS_PER_BIT/4);
      // Clear output compare interrupt flag.
      TCF1.INTFLAGS = 0x30;
    }
    else
      RFID.rfid_reset_status();

    return;
  }
  else if(rfid_sync_state == 3)
  {
    // Rising edge detected while sampling data
    if ((rfid_int_time  < (TICKS_PER_BIT/4 + SEARCH_MARGIN)))
    {
      // Rising edge at start of bit
      // 1st sample interrupt should be 3/4 of bit from current point
      TCF1.CNT = (TICKS_PER_BIT/4);
    }
    else
    {
      // Rising edge in middle of bit period
      // 1st sample interrupt should be 1/4 of bit from current point
      TCF1.CNT = TICKS_PER_BIT - (TICKS_PER_BIT/4);
    }
  }
}

// Sampling timer interrupt.
SIGNAL(TCF1_CCA_vect)
{
  //digitalWrite(D2_Green, LOW);
  if (rfid_sync_state == 0)
  {
    // Should not be here, exit
    RFID.rfid_reset_status();
  }
	// Reset counter;
	TCF1.CNT = 0x0000;
  
  if(rfid_data_index < 10)
  {
    if(bit_is_clear(PORTJ.IN,1))
    {
      // '1' bit read.
      rfid_sample = 1;
      rfid_data[rfid_data_index] |= 1 << (4 - rfid_data_row_cntr);
      rfid_row_parity++;
    }
    else
    {
      // '0' bit read.
    }

    rfid_data_row_cntr++;

    if((rfid_data_row_cntr) % 5 == 0)
    {
      // Subtract one from parity count if last bit = '1'.
      rfid_row_parity -= rfid_sample;
      
      // End of row. Calculate parity, reset row, increment array index..
      if(rfid_row_parity % 2 == rfid_sample)
      {
        // Row parity ok.
        rfid_row_parity = 0;
        rfid_data_row_cntr = 0;
        
        // Shift row to remove parity from data.
        rfid_data[rfid_data_index] >>= 1;
        rfid_data_index++;
      }
      else
      {
        // Row parity failed, abort read.
        RFID.rfid_reset_status();
      }
    }
  }
  else
  {
    RFID.SaveRFID();
    RFID.rfid_reset_status();
  }
}









Driver_RFID::Driver_RFID()
{
updated = 0;
}

void Driver_RFID::rfid_init(void)
{

 // return;

  // Set the CFE pin direction.
	sbi(PORTJ.DIR,0);

  // Enable interrupt 0 for PJ1.
	RFID_ENABLE_INT_EDGE;

#ifdef PRODUCTION_TESTING
  // Set the interrupt level to HI for interrupt 0.
  sbi(PORTJ.INTCTRL,1);
  sbi(PORTJ.INTCTRL,0);
#else
  sbi(PORTJ.INTCTRL,1);
  cbi(PORTJ.INTCTRL,0);   //VERY Important changed ISR level to medium for performance concerns
#endif	


  // Sample RFID input on rising edge only, set interrupt accordingly
	PORTJ.PIN1CTRL = PORT_ISC_RISING_gc;

	// Set sampling interval 
  // 124 ticks @ 242kHz = one RF ID read bit period (512 us)
  // Peripheral clock = 32 Mhz -> sample at 500kHz, 256 ticks per bit
	TCF1.CCA = TICKS_PER_BIT;

	// Disable compare/capture A.
	RFID_DISABLE_INT_SAMPLE;


  // Reset & enable TCF1
	TCF1.CNT = 0x0000;

  // Set the prescaler for sampling timer.
 // TCD1.CTRLA = ( TCD1.CTRLA & ~TC1_CLKSEL_gm ) | TC_CLKSEL_DIV1_gc; //no prescaler
 TCF1.CTRLB = 0;
 TCF1.CTRLC = 0;
 TCF1.CTRLD = 0;
	TCF1.CTRLA = TC_CLKSEL_DIV64_gc;
//TCF1.PER = 32000;


#ifdef PRODUCTION_TESTING
  rfid_set_state(RFID_DISABLED);  
#else
  rfid_set_state(RFID_ENABLED_POWERSAVE);
#endif

#ifdef FCC_TEST_MODES
  rfid_set_state(RFID_ENABLED_ON);
#endif

  rfid_reset_status();
}

void Driver_RFID::driver_rfid_tmr_10ms(void)
{
  if (rfid_disabled_counter == 0)
  {
    rfid_disabled_counter = RFID_PERIOD;
  }
  else
  {
    rfid_disabled_counter--;
  }

  if (rfid_disabled_flag == false && no_sync_timeout < 10)
  {
    if (++no_sync_timeout == 10) //if no sync has been detected then set the offtag flag
    {
      rfid_flag_offtag_detected = 1;
      rfid_reset_status();
    }
  }
}

void Driver_RFID::rfid_task(void)
{
  rfid_disabled_flag = false;
  #ifdef FCC_TEST_MODES
  if (rfid_state == RFID_DISABLED || fcc_rfid_enabled_flag == false)
  #else
  if (rfid_state == RFID_DISABLED)
  #endif
  {
    DISABLE_RFID_COIL;
    rfid_disabled_flag = true;
    // Disable edge interrupt

    rfid_reset_status();
    
    return;

  }
  if (rfid_state == RFID_ENABLED_ON)
  {
    ENABLE_RFID_COIL;
    // Enable edge interrupt
   // digitalWrite(D5_Green, LOW);
    return;

  }
  if (rfid_state == RFID_ENABLED_POWERSAVE)
  {
    // Called from main loop
    //if ((MOTORS_ACTIVE != 0) || (rfid_disabled_counter <= RFID_ON_THRESHOLD))
	if ((rfid_disabled_counter <= RFID_ON_THRESHOLD))
    {
      // Enable RFID pin
      ENABLE_RFID_COIL;
      return;
    }
    DISABLE_RFID_COIL;
    rfid_disabled_flag = true;
  }

}



void Driver_RFID::rfid_set_state(e_rfid_state new_state)
{
  rfid_state = new_state; 
  rfid_task();
}

void Driver_RFID::SaveRFID(void)
{
  rfid_bufdata[0]  =  (rfid_data[8] << 4) | rfid_data[9];
  rfid_bufdata[1]  =  (rfid_data[6] << 4) | rfid_data[7];
  rfid_bufdata[2]  =  (rfid_data[4] << 4) | rfid_data[5];
  rfid_bufdata[3]  =  (rfid_data[2] << 4) | rfid_data[3];
  rfid_bufdata[4]  =  (rfid_data[0] << 4) | rfid_data[1];
  rfid_flag_tag_data_updated = 1;

  PORTK.DIRSET = 0x0F; //TIAN
  

} 


void Driver_RFID::GetRFID(unsigned char *data)
{
  if (rfid_flag_tag_data_updated == 0)
  {
    //memset(data,0,5);
	 data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
    return;
  }
 // const void* rr = &rfid_bufdata;
 // memcpy(data, (&rfid_bufdata),5);
 
 data[0] = rfid_bufdata[0];
 data[1] = rfid_bufdata[1];
 data[2] = rfid_bufdata[2];
 data[3] = rfid_bufdata[3];
 data[4] = rfid_bufdata[4];
 
 
  rfid_flag_tag_data_updated = 0;
}

void Driver_RFID::rfid_reset_status(void)
{
  rfid_sync_state = 0;
  rfid_data_index = 0;
  rfid_row_parity = 0;
  rfid_data_row_cntr = 0;

  // Disable sampling interrupt.
  RFID_DISABLE_INT_SAMPLE;
  
  // Enable edge interrupt.
  RFID_ENABLE_INT_EDGE;


}

bool Driver_RFID::DataAvailable(void)
{
return (rfid_flag_tag_data_updated>0);
}


/*
This Sample indicates how to use the RFID library in its basic form, with the RFID on at all times.
Care should be taken as the RFID draws alot of power.
The RFID chip can not be driven from USB power.
*/
/*

#include <Driver_RFID.h>


void setup() {
  // put your setup code here, to run once:


  pinMode(D1_Green, OUTPUT);  
  pinMode(D2_Green, OUTPUT);  
  pinMode(D3_Green, OUTPUT);  
  pinMode(D4_Green, OUTPUT);  
  pinMode(D5_Green, OUTPUT);  
  pinMode(D1_Red, OUTPUT);  
  pinMode(D2_Red, OUTPUT);  
  pinMode(D3_Red, OUTPUT); 
    
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Green, HIGH);
  digitalWrite(D3_Green, HIGH);
  digitalWrite(D4_Green, HIGH);
  digitalWrite(D5_Green, HIGH);
  digitalWrite(D1_Red, HIGH);
  digitalWrite(D2_Red, HIGH);
  digitalWrite(D3_Red, HIGH);    
  
  RFID.rfid_init();  
  e_rfid_state ee = RFID_ENABLED_ON;
  RFID.rfid_set_state(ee);
  
  Serial.print("RDIF Setup Done: ");
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(D1_Green, HIGH);
  digitalWrite(D2_Red, HIGH);
  delay(2000);
  digitalWrite(D1_Green, LOW);
 
 // e_rfid_state ee = RFID_ENABLED_ON;
 //RFID.rfid_set_state(ee);

  //if (RFID.rfid_flag_tag_data_updated)
  if (RFID.DataAvailable())
  {
    digitalWrite(D2_Red, LOW);
    unsigned char rfid_TAG[10];
    RFID.GetRFID(rfid_TAG);
    Serial.print("TAG Read: ");
    Serial.print(rfid_TAG[0]);
    Serial.print(rfid_TAG[1]);
    Serial.print(rfid_TAG[2]);
    Serial.print(rfid_TAG[3]);
    Serial.println(rfid_TAG[4]);        
  }
}
*/





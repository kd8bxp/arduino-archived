#include "SmartResponseXE.h"

// variables used for wireless activity
volatile uint8_t rxhead, rxtail, rssiRaw, txbusy;
volatile uint16_t txlen;
uint8_t txbuf[284]; // allows for maximum STK500 packet size + a few extra bytes
uint8_t rxbuf[256];

// Initialize the RFA1's low-power 2.4GHz transciever.
// Sets up the state machine, and gets the radio into
// the RX_ON state. Interrupts are enabled for RX
// begin and end, as well as TX end.
uint8_t rfBegin(uint8_t channel)
{
  txlen = 0;
  rxhead = rxtail = 0;
  txbusy = 0;
  
  // Transceiver Pin Register -- TRXPR.
  // This register can be used to reset the transceiver, without
  // resetting the MCU.
  TRXPR |= (1<<TRXRST);   // TRXRST = 1 (Reset state, resets all registers)

  // Transceiver Interrupt Enable Mask - IRQ_MASK
  // This register disables/enables individual radio interrupts.
  // First, we'll disable IRQ and clear any pending IRQ's
  IRQ_MASK = 0;  // Disable all IRQs

  // Transceiver State Control Register -- TRX_STATE
  // This regiseter controls the states of the radio.
  // First, we'll set it to the TRX_OFF state.
  TRX_STATE = (TRX_STATE & 0xE0) | TRX_OFF;  // Set to TRX_OFF state
  delay(1);

  // Transceiver Status Register -- TRX_STATUS
  // This read-only register contains the present state of the radio transceiver.
  // After telling it to go to the TRX_OFF state, we'll make sure it's actually
  // there.
  if ((TRX_STATUS & 0x1F) != TRX_OFF) // Check to make sure state is correct
    return 0;    // Error, TRX isn't off

  // Transceiver Control Register 1 - TRX_CTRL_1
  // We'll use this register to turn on automatic CRC calculations.
  TRX_CTRL_1 |= (1<<TX_AUTO_CRC_ON);  // Enable automatic CRC calc. 

  // Enable RX start/end and TX end interrupts
  IRQ_MASK = (1<<RX_START_EN) | (1<<RX_END_EN) | (1<<TX_END_EN);

  // Transceiver Clear Channel Assessment (CCA) -- PHY_CC_CCA
  // This register is used to set the channel. CCA_MODE should default
  // to Energy Above Threshold Mode.
  // Channel should be between 11 and 26 (2405 MHz to 2480 MHz)
  if ((channel < 11) || (channel > 26)) channel = 11;  
  PHY_CC_CCA = (PHY_CC_CCA & 0xE0) | channel; // Set the channel

  // Finally, we'll enter into the RX_ON state. Now waiting for radio RX's, unless
  // we go into a transmitting state.
  TRX_STATE = (TRX_STATE & 0xE0) | RX_ON; // Default to receiver

  return 1;
} /* rfBegin() */



//
// This function sends a set of bytes out of the radio.
// It will set the first byte of the transmission packet to the total length
//
void rfWrite(void)
{
  uint8_t bLen;
  uint8_t *s = &txbuf[0];
  
  if (txlen == 0) return; // nothing to do
  // Transceiver State Control Register -- TRX_STATE
  // This regiseter controls the states of the radio.
  // Set to the PLL_ON state - this state begins the TX.
  TRX_STATE = (TRX_STATE & 0xE0) | PLL_ON;  // Set to TX start state
  while(!(TRX_STATUS & PLL_ON))
    ;  // Wait for PLL to lock

// Data may be more than 125 bytes, so we need to break it up
  while (txlen > 0)
  {
     while (txbusy) // wait for last packet to finish transmitting
     { };
     
     if (txlen > 125)
        bLen = 125;
     else
        bLen = txlen;
  // Start of frame buffer - TRXFBST
  // This is the first byte of the (up to 128 byte) frame. It should contain
  // the length of the transmission.
  TRXFBST = bLen + 2; // 16-bit CRC + data to send
  memcpy((void *)(&TRXFBST+1), s, bLen);
  s += bLen;
  txlen -= (uint16_t)bLen;
  // Transceiver Pin Register -- TRXPR.
  // From the PLL_ON state, setting SLPTR high will initiate the TX.
  TRXPR |= (1<<SLPTR);   // SLPTR high
  TRXPR &= ~(1<<SLPTR);  // SLPTR low
  txbusy = 1; // set busy flag which will get reset by the TRX24_TX_END interrupt
  } // while transmitting packets
  // After sending the bytes, set the radio back into the RX waiting state.
  TRX_STATE = (TRX_STATE & 0xE0) | RX_ON;
} /* rfWrite() */

// This interrupt is called when radio TX is complete. We'll just
ISR(TRX24_TX_END_vect)
{
  txbusy = 0; // indicate the packet has finished transmitting
}

// This interrupt is called the moment data is received by the radio.
// We'll use it to gather information about RSSI -- signal strength --
ISR(TRX24_RX_START_vect)
{
  rssiRaw = PHY_RSSI;  // Read in the received signal strength
}

//
// This interrupt is called at the end of data receipt.
// It will be placed in a circular buffer and transmitted to the local
// serial port as soon as it's received
//
ISR(TRX24_RX_END_vect)
{
  uint8_t length, *s;
  // The received signal must be above a certain threshold.
  if (rssiRaw & RX_CRC_VALID)
  {
    // The length of the message will be the first byte received.
    length = TST_RX_LENGTH;
    // The remaining bytes will be our received data.
    s = (uint8_t *)(void*)&TRXFBST;
    while (length > 2) // ignore 16-bit CRC at the end of the received data
    { // copy the bytes into our circular buffer
       rxbuf[rxhead++] = *s++; // 8-bit pointer will automatically wrap
       length--;
       // if the head wraps around and passes the tail, something else is very broken
    }
  }
}

void setup() {
  SRXEInit(0xe7, 0xd6, 0xa2); // Initialize the LCD
  
  rfBegin(26); // default to 802.15.4 channel 26 to upload code to the custom bootloader
  delay(3000); // wait a few seconds since the same serial port is used to update this device
  //SERIAL.begin(57600); // default Arduino IDE upload baud rate
} // setup


void loop() {

}

#include <IRremote.h>
#include <VirtualWire.h>

const int numberOfKeys = 1;

long irKeyCodes[numberOfKeys] = {
    0x18E758A7, //0 key
};

IRsend irsend;

void setup() {
  Serial.begin(9600);
  vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(12);
    vw_setup(4000);  // Bits per sec
    pinMode(13, OUTPUT);
    vw_rx_start();       // Start the receiver PLL running
}

void loop() {

  transmitCode();
  delay(500);
  receiveCode();
  //delay(2000);
  while(1);
}

void transmitCode() {
  irsend.sendSony(irKeyCodes[0],32);
  Serial.println("Sending....");
  
}

void receiveCode() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      int beacon;
       beacon = char(buf[0]) - 48;
      
      Serial.println(beacon);
      
    }   

}


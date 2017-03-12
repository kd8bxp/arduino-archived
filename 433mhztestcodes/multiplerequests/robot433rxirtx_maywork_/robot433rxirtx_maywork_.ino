#include <IRremote.h>
#include <VirtualWire.h>

const int numberOfKeys = 1;
char *volts;
int count = 0;
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
 Serial.println("Sending....");
  
for (int i=0; i<50; i++) {
  transmitCode();
  delay(30);
  receiveCode();
  //delay(2000);
}
  Serial.print("Return Count: ");
  Serial.println(count);

  while(1);
}

void transmitCode() {
  irsend.sendSony(irKeyCodes[0],32);
 
}

void receiveCode() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      int beacon;
       beacon = char(buf[0]) - 48;
       count ++;
       Serial.println(beacon);
          }
     /*     delay(50);
      if (vw_get_message(buf, &buflen)) {
      for (int i=1; i<buflen; i++) {
       volts[i] = char(buf[i]);
       Serial.println(volts);
          }        
      }*/
}


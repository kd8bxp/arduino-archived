#include <IRremote.h>
#include <VirtualWire.h>

const int numberOfKeys = 1;

long irKeyCodes[numberOfKeys] = {
    0x10AABBCCDD, //0 key
};

unsigned long txtime;
unsigned long rxtime;
float caltime;
//unsigned long distance;
unsigned long speed = 299792458; //0.189291667; //2.2715; //feet per second
signed long distance;

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
  delay(25);
  receiveCode();
  //delay(2000);
  if (rxtime != 0) {
  Serial.print("TXtime: ");
  Serial.println(txtime);
  Serial.print("RXtime: ");
  Serial.println(rxtime);
caltime = ((rxtime - txtime )); //millisseconds
  Serial.print("CalTime: ");
  Serial.println(caltime);
  distance = ((caltime) * speed)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  //while(1);
  }
  delay(3000);
}

void transmitCode() {
  txtime = millis();
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
      rxtime = millis();
      Serial.println(beacon);
      
    }   else {rxtime = 0;}

}


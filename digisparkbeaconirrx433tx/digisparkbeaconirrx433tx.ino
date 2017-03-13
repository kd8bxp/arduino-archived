#define TX_PIN 0
#include "VirtualWire.h"
#include "IRLib.h" //adds the library code to the sketch
const int irReceiverPin = 1;
char *controller = "1";
IRrecv irrecv(irReceiverPin); //create an IRrecv object
IRdecode decodedSignal; //stores results from IR detector
unsigned int Buffer[RAWBUF];

void setup() {
   irrecv.enableIRIn();
   decodedSignal.UseExtnBuf(Buffer);
 vw_set_ptt_inverted(true); 
  vw_set_tx_pin(TX_PIN);
  
  vw_setup(4000);// speed of data transfer Kbps
}

void loop() {
  irReceive();

}

void irReceive() {
  if (irrecv.GetResults(&decodedSignal) ) //this is true if a message has been received
    {
      decodedSignal.decode();
      txBeaconID();
      irrecv.resume();
      
    }
     
}

void txBeaconID() {
   
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // Wait until the whole message is gone
  
}


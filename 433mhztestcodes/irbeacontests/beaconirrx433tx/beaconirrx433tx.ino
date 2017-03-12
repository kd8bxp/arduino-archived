#define TX_PIN 12
#include <VirtualWire.h>
#include <IRremote.h> //adds the library code to the sketch
const int irReceiverPin = 2;
char *controller = "1";
IRrecv irrecv(irReceiverPin); //create an IRrecv object
decode_results decodedSignal; //stores results from IR detector

void setup() {
   irrecv.enableIRIn();
 vw_set_ptt_inverted(true); 
  vw_set_tx_pin(TX_PIN);
  
  vw_setup(4000);// speed of data transfer Kbps
}

void loop() {
  irReceive();

}

void irReceive() {
  if (irrecv.decode(&decodedSignal) ) //this is true if a message has been received
    {
      txBeaconID();
      irrecv.resume();
    }
     
}

void txBeaconID() {
   
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // Wait until the whole message is gone
  
}


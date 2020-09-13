/* This is a beacon for the LoRaHam protocol by KK4VCZ.
 * https://github.com/travisgoodspeed/loraham/
 *  
 */

//Please change these two to describe your hardware.
#define CALLSIGN "KD8BXDP-10"
#define COMMENTS "Change this."

#include <SPI.h>
//#include <RH_RF95.h>  //See http://www.airspayce.com/mikem/arduino/RadioHead/
#include <LoRa.h>
 
// for feather32u4 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define VBATPIN A9  
 
/* for feather m0  */
//#define RFM95_CS 8
//#define RFM95_RST 4
//#define RFM95_INT 3
//#define VBATPIN A7 /**/

//Uncomment this line to use the UART instead of USB in M0.
//#define Serial Serial1
 
/* for shield 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 7
*/
 
 
/* for ESP w/featherwing 
#define RFM95_CS  2    // "E"
#define RFM95_RST 16   // "D"
#define RFM95_INT 15   // "B"
*/
 
/* Feather 32u4 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)
*/
 
/* Feather m0 w/wing 
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
*/
 
/* Teensy 3.x w/wing 
#define RFM95_RST     9   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     4    // "C"
*/

 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 432300000
 
// Singleton instance of the radio driver
//RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13

    int       loraSpreadingFactor = 9;
  int       loraCodingRate      = 5;
  int       loraTxPower         = 17;
  uint32_t  loraBandwidth       = 62500; //125E6;


//Returns the battery voltage as a float.
float voltage(){
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}


void radiooff(){
 //arduino-LoRa library provides a sleep, and a idle mode (Sleep is what I beleive we need here) 
  //I'm not sure this is needed once deep sleep was turned on. More
  //Information is needed.
  LoRa.sleep();
  delay(10);
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
   LoRa.setPins(RFM95_CS,RFM95_RST,RFM95_INT);
  
  if (!LoRa.begin(RF95_FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 // LoRa.enableCrc(); //encable CRC checking - off by default
    LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
  digitalWrite(LED, LOW);
  beacon();
}

//! Uptime in seconds, correcting for rollover.
long int uptime(){
  static unsigned long rollover=0;
  static unsigned long lastmillis=millis();

  //Account for rollovers, every ~50 days or so.
  if(lastmillis>millis()){
    rollover+=(lastmillis>>10);
    lastmillis=millis();
  }

  return(rollover+(millis()>>10));
}

//Transmits one beacon and returns.
void beacon(){
  static int packetnum=0;
  //float vcc=voltage();
  float vcc=3.7;
  //Serial.println("Transmitting..."); // Send a message to rf95_server
  
  char radiopacket[100];
  snprintf(radiopacket,
           100,
           "%s<KD8BXP-2:%d %s",
           CALLSIGN,
           packetnum,
           COMMENTS
           );

  //radiopacket[sizeof(radiopacket)] = 0;
  
  LoRa.beginPacket();
  //LoRa.print(radiopacket);
  //String radiopacket;
  //root.printTo(radiopacket);
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  packetnum++;
}

//Handles retransmission of the packet.
bool shouldirt(uint8_t *buf, uint8_t len){
  /*
  //Don't RT any packet containing our own callsign.
  if(strcasestr((char*) buf, CALLSIGN)){
    //Serial.println("I've already retransmitted this one.\n");
    return false;
  }
  //Don't RT if the packet is too long.
  if(strlen((char*) buf)>128){
    //Serial.println("Length is too long.\n");
    return false;
    
  }
  
  //Random backoff if we might RT it.
  delay(random(10000));
  //Don't RT if we've gotten an incoming packet in that time.
  if(rf95.available()){
    //Serial.println("Interrupted by another packet.");
    return false;
  }

  //No objections.  RT it!
  return true;
  */
}

//If a packet is available, digipeat it.  Otherwise, wait.
void digipeat(){
 /* 
  //digitalWrite(LED, LOW);
  //Try to receive a reply.
  if (rf95.available()){
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    int rssi=0;
    float vcc=voltage();
    //*
    // * When we receive a packet, we repeat it after a random
    // * delay if:
    // * 1. It asks to be repeated.
    // * 2. We've not yet received a different packet.
    // * 3. We've waited a random amount of time.
    // * 4. The first word is not RT.
     //*
    if (rf95.recv(buf, &len)){
      rssi=rf95.lastRssi();
      //digitalWrite(LED, HIGH);
      //RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("Got: ");
      buf[len]=0;
      Serial.println((char*)buf);
      Serial.println("");

      if(shouldirt(buf,len)){
        // Retransmit.
        uint8_t data[RH_RF95_MAX_MESSAGE_LEN];
        snprintf((char*) data,
                 RH_RF95_MAX_MESSAGE_LEN,
                 "%s\n" //First line is the original packet.
                 "RT %s rssi=%d VCC=%d.%03d uptime=%ld", //Then we append our call and strength as a repeater.
                 (char*) buf,
                 CALLSIGN,  //Repeater's callsign.
                 (int) rssi, //Signal strength, for routing.
                 (int) vcc,
                 (int) (vcc*1000)%1000,
                 uptime()
                 );
        rf95.send(data, strlen((char*) data));
        rf95.waitPacketSent();
        Serial.println((char*) data);
        //digitalWrite(LED, LOW);
        Serial.println("");
      }else{
        //Serial.println("Declining to retransmit.\n");
      }
    }else{
      Serial.println("Receive failed");
    }
  }else{
    delay(10);
  }
  */
}

void loop(){
  static unsigned long lastbeacon=millis();
  
  //Only digipeat if the battery is in good shape.
  //if(voltage()>3.5){
    //Only digipeat when battery is high.
    //digipeat();

    //Every ten minutes, we beacon just in case.
    //if(millis()-lastbeacon>10*60000){
    if (millis()-lastbeacon>.5*60000) {
      beacon();
      lastbeacon=millis();
    }
  //}else{
    //Transmit a beacon every ten minutes when battery is low.
    //radiooff();
    //delay(10*60000);
    //radioon();
    //beacon();
  //};
  
}

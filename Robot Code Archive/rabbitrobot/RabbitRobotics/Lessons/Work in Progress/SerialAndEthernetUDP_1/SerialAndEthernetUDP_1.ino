#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
   0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
 EthernetUDP Udp;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "ack";       // a string to send back

int incomingByte = 0;

void setup() {
    // initialize serial:
   Serial.begin(9600);
   // start the Ethernet and UDP:
   Ethernet.begin(mac,ip);
   Udp.begin(localPort);

}

void loop() {

   int packetSize = Udp.parsePacket();
   if(packetSize)
   {
     Serial.print("Received packet of size ");
     Serial.println(packetSize);
     Serial.print("From ");
     IPAddress remote = Udp.remoteIP();
     for (int i =0; i < 4; i++)
     {
       Serial.print(remote[i], DEC);
       if (i < 3)
       {
         Serial.print(".");
       }
     }
     Serial.print(", port ");
     Serial.println(Udp.remotePort());

     // read the packet into packetBufffer
     Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
     Serial.println("Contents:");
     Serial.println(packetBuffer);
     
     
     // have received a UDP packet.. now respond to UDP sender.
     // send a reply, to the IP address and port that sent us the packet we received
     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
     //Udp.write(ReplyBuffer);
     Udp.write(packetBuffer);
     Udp.endPacket();
     for (int i =0; i < (UDP_TX_PACKET_MAX_SIZE-1); i++) 
     {
     packetBuffer[i]=0;
     } // cleared the incoming packet buffer...
    }


    // if there's any serial available, read it:
    if (Serial.available() > 0) {
           // read the incoming byte:
           incomingByte = Serial.read();
           // say what you got:
           Serial.print("I received code: ");
           Serial.println(incomingByte, DEC);
           
                  if (incomingByte==83){ // "S" for Stop
                      Serial.print("UDP Max Packet Size: "); // Test if Serial is working.
                      Serial.println(UDP_TX_PACKET_MAX_SIZE);
                  } else if (incomingByte==76){ // "L" for Local Port
                      Serial.print("Local Port: ");
                      Serial.println(localPort);
                  } else if (incomingByte==73){ // "I" for Local IP
                      Serial.print("Local IP: ");
                      for (int i =0; i < 4; i++){
                         Serial.print(ip[i], DEC);
                         if (i < 3) {
                           Serial.print(".");
                           }
                       }
                       Serial.println(" ");
                  } else if (incomingByte==102){ // "f" for ____________
                      Serial.println("I received f Code. ");
                  } else if (incomingByte==82){  // "R" for remote IP and Port
                      Serial.print("Remote IP: ");
                      IPAddress remote = Udp.remoteIP();
                      for (int i =0; i < 4; i++){
                      Serial.print(remote[i], DEC);
                      if (i < 3){
                         Serial.print(".");
                          }
                      }
                      Serial.print(", port ");
                      Serial.println(Udp.remotePort());
     
     
                  } else {
                      Serial.print("I received Bad Code: ");
                      Serial.println(incomingByte);
                     // don't take the code...
                   }
                   
    }
    
}



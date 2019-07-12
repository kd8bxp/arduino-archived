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

void setup() {
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
}
}



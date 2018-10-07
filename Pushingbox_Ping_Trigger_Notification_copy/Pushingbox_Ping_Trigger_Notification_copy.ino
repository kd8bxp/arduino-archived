////
//
// General code from http://www.pushingbox.com for Arduino + Ethernet Shield (ENC28J60)
// Library EtherCard from https://github.com/jcw/ethercard is under mit license
//
////
#include <EtherCard.h>
#include <NewPing.h>

  /////////////////
 // MODIFY HERE //
/////////////////
static byte mymac[] = {0x24,0x99,0x46,0xAD,0x30,0x31};   // Be sure this address is unique in your network

//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
                                              
char DEVID1[] = "PUTYOURPUSHINGBOXCODEHERE"; //Scenario from Pushingbox APi


int pingSensorIn;
int triggerDefault;
int triggerDistance;

// Debug mode
boolean DEBUG = true;
  ///////
 //End//
///////

char website[] PROGMEM = "api.pushingbox.com";
byte Ethernet::buffer[700];
Stash stash;
boolean pinDevid1State = false;  // Save the last state of the Pin for DEVID1

#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup () {
  Serial.begin(9600);
  
  if(DEBUG){Serial.println("\n[getDHCPandDNS]");}

setTriggerDistance();  

  //
  //***Depending of your Shield, you may have to try this line instead of the second***//
  //if(ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
  if(ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) 
    if(DEBUG){Serial.println( "Failed to access Ethernet controller");}
  
  // Wait until we have an IP from the DHCP
  while(!ether.dhcpSetup()){
    if(DEBUG){Serial.println("Error: DHCP failed. Can't get an IP address, let's retry...");}
  }
  
  if(DEBUG){
    ether.printIp("My IP: ", ether.myip);
    ether.printIp("Netmask: ", ether.mymask);
    ether.printIp("GW IP: ", ether.gwip);
    ether.printIp("DNS IP: ", ether.dnsip);
  }

  if (!ether.dnsLookup(website))
    if(DEBUG){Serial.println("DNS failed");}
  if(DEBUG){ether.printIp("Server: ", ether.hisip);}

  
 
}

void loop () {

    ether.packetLoop(ether.packetReceive());
    
    delay(50000);                      
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_IN); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println(" in");
  pingSensorIn = (uS / US_ROUNDTRIP_IN);
      ////
      // Listening for the pinDevid1 state
      ////
      if (pingSensorIn >= triggerDistance && pinDevid1State == false) // door is open 
      {
        if(DEBUG){Serial.println("Door Open!");}
        pinDevid1State = true;
        //Sending request to PushingBox when the pin is HIGHT
        ether.browseUrl(PSTR("/pushingbox?devid="), DEVID1, website, my_callback);
        delay(500);  
      }
       if (pingSensorIn <= triggerDefault && pinDevid1State == true) // door closed
      {
        if(DEBUG){Serial.println("Door Closed");}
        pinDevid1State = false;
        //Sending request to PushingBox when the pin is LOW
        //ether.browseUrl(PSTR("/pushingbox?devid="), DEVID1, website, my_callback);
        delay(500);
      }
    delay(50000);  
}

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");


}

void setTriggerDistance() {
	Serial.println("Press any key to read default: ");
 while (Serial.read()<=0) {
    // get the new byte:
    //char inChar = (char)Serial.read();
    
}
   unsigned int uS = sonar.ping();
   triggerDefault = (uS / US_ROUNDTRIP_IN);
 Serial.print("Distance ");
 Serial.print(triggerDefault);
 Serial.println(" in");
 
  Serial.println("Press any key to read Trigger: ");
    while (Serial.read()<=0) {
    // get the new byte:
    //char inChar = (char)Serial.read();
    
    }
  uS = sonar.ping();
  triggerDistance = (uS / US_ROUNDTRIP_IN);
  Serial.print("Distance ");
  Serial.print(triggerDistance);
  Serial.println(" in");
}
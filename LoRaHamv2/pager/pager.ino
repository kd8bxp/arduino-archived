//This sketch is written for the LoRa32-OLD board, It should be used as a PAGER, and will digipeat messages not addressed to this device
//It can also be used as a serial interface if connected to a computer, bluetooth, and WiFi AP. But would take some work to get it to gate to the MQTT broker
//This sketch should be able to be adapted for other ESP32 boards

#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <Wire.h>
#include "SSD1306.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>

#define OLED_RESET  16  // Pin 16 -RESET digital signal
#define OLED_SDA    4  // SDA-PIN for I2C OLED
#define OLED_SCL    15  // SCL-PIN for I2C OLED

#define CQMSG "LoRaHam net v2.0 pager"
String CALLSIGN="KD8BXP-1"; //this will be appended to the message when a packet is digipeated. This is also the callsign to Beacon an ID 

SSD1306 display(0x3c, OLED_SDA, OLED_SCL); // FOR I2C
WebServer server(80);

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      18
#define RST     14
#define DI0     26
#define BAND    432300000 // local frequencys, must match!

// Blinky on receipt
#define LED 25

//For this to work on a local level these parameters need to match
int       loraSpreadingFactor = 9;
int       loraCodingRate      = 5;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 62500; //125E6;
//This results in about 879bps if you believe this site https://unsigned.io/understanding-lora-parameters/
//or maybe 488bps from this website https://www.rfwireless-world.com/calculators/LoRa-Data-Rate-Calculator.html


String inputString;
bool stringComplete = false;
String call, msg;
String radiopacket;
String FROM;
char ssid[15]; //Create a Unique AP from MAC address * Creates an AP named "LoRaHam-xxxx' where xxxx is part of the MAC address of device.
const char *password = "pass1234"; //Access Point (AP) Password: not a great password, you may want to change it. 
static int serverCore = 0; //run web server on this core. Loop() runs in core 1

void createSSID() {
  
  uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid>>32);
  snprintf(ssid,15,"LoRaHam-%04X",chip);
  
}

void displayIntro() {
  display.clear();
  display.setColor(WHITE);
  display.drawString(0,0,"My Call: ");
  display.drawString(45, 0,CALLSIGN);
  display.drawString(35,13,"LoRaHam Pager");
  display.drawString(47,24,"by KK4VCZ.");
  display.drawString(32,34,"ESP32 Pager by");
  display.drawString(50,46,"KD8BXP.");
  display.display();
  delay(3000); //while delayed can't get messages
  display.clear();
  display.drawString(0,30,"SSID: ");
  display.drawString(30,30,ssid);
  display.display();
  delay(3000);
  display.clear();
  display.display();
  
}

void displaypacket(){
  
  display.clear();
  display.setColor(WHITE);
  //display.drawStringMaxWidth(0,0,110, pkt);
  //display.drawString(0,0, "To:");
  //display.drawString(15,0, TO);
  display.drawString(0, 10, "From:");
  display.drawString(27, 10, FROM);
  display.drawStringMaxWidth(0, 20,110, msg);
  display.println("");
  display.display();
  delay(5000); //need better way to do this, while delayed can't get new messages
  display.clear();
  display.display();
  /*if (RTCNT > 0) {
    display.drawString(0,60,"Re-Transmit By ");
    for (int xx=0; xx < RTCNT; xx++) {
      display.drawString(0,10*xx,RTCALL[xx]+" "+RTRSSI[xx]);
    }
    display.display();
    
    delay(3000); //need better way to do this, while delayed can't get new messages
    display.clear();
    display.display();
  }
  //Display Sending Device Info
  display.drawString(0,0,"DeviceID: " + TXDEVICEID);
  display.drawString(0,10,"Device Type: "+ TYPE);
  display.drawString(0,20,"Grid: "+GRID);
  display.drawString(0,30,"Other: "+OTHER);
  */
}


void radioon(){
 LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
 
}

void radiooff(){
  //arduino-LoRa library provides a sleep, and a idle mode (Sleep is what I beleive we need here) 
  //I'm not sure this is needed once deep sleep was turned on. More
  //Information is needed.
  LoRa.sleep();
  delay(10);
}

void rx() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
        digitalWrite(LED, LOW);
        String buf1;
        char temp;
        digitalWrite(LED, HIGH); //Packet Received
    // read packet
     while (LoRa.available()) {
    temp = (char)LoRa.read();
    buf1 += temp;
  }
  char buf[buf1.length()+1];
  buf1.toCharArray(buf, buf1.length()+1);
  checkPacket(buf);     
           }  
}

void checkPacket(String pkt) {

Serial.println(pkt); //debug to see if we got a packet

if (pkt.indexOf(CALLSIGN) > 0) {
  
     StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(pkt);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
 // For other devices this is were the message can be displayed for a Relay, this is where the message stops if addressed to the relay call sign (no one will probably ever see this message)
 // also check to see if this device retransmitted the message, if it retransmitted, then it should NOT display the message
 //Serial.println(pkt);
} else {
   //This is where we decode the message to get retransmit count, and to add our call if needed, and then to retransmit the message
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);
   int rtcount = root["R"];
   if (rtcount > 0) {
   root["PATH"][4-rtcount]=CALLSIGN;
   rtcount = rtcount - 1; 
   root["R"] = rtcount;
   
   String radiopacket;
  root.printTo(radiopacket);
  //Serial.println(radiopacket);
  digipeat();
  }
 }
}


void serialParse() {
  if (stringComplete) {
    inputString.trim();
    call=inputString.substring(0,inputString.indexOf(" "));
    call.toUpperCase();
    msg=inputString.substring(inputString.indexOf(" ")+1);//,inputString.length()-1);
   
    //Serial.println(call);
    //Serial.println(msg);
    //Serial.println(call+msg);
    //Serial.println(inputString);
    // clear the string:
    txMsg();
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      serialParse();
    }
  }
}

void txMsg(){
  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
  
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["TO"] = call; //TOCALL;
  root["FROM"] = CALLSIGN;
  
  
  root["MESSAGE"] = msg; //COMMENTS; //TEMP;
  root["R"] = 3; //retransmit count starts at 3 and decreases.
  JsonArray& path = root.createNestedArray("PATH");
  path.add(CALLSIGN);
  path.add("NOCALL1");
  path.add("NOCALL2");
  path.add("NOCALL3");
  //origin of message, each additional hop added to list 
  
  LoRa.beginPacket();
   
  root.printTo(radiopacket);
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  radiopacket="";
  call = "";
  msg = "";
  //packetnum++;
  digitalWrite(LED, LOW);
}

void digipeat() {

  digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH);
    
  LoRa.beginPacket();
    
  LoRa.print(radiopacket);
  Serial.println(radiopacket);
  LoRa.endPacket();
  radiopacket="";
  call = "";
  msg = "";
  digitalWrite(LED, LOW);
}

//Beacon function has changed from the RELAY sketch, ESP32 devices use FREERTOS
void beacon(void *pvParameters) {
vTaskDelay(60000 /portTICK_PERIOD_MS); //send 1st BEACON after about a minute
while(true) {
    call = "BEACON";
    msg = CQMSG;
    txMsg();
    vTaskDelay(600000 /portTICK_PERIOD_MS); //wait and send BEACON about every 10 mins.
 }
} 

void serverTask( void * pvParameters ){
      while(true){
        server.handleClient();
        vTaskDelay(10);
                }
 
}

void webRoot() {
        String temp;
        temp = "<meta name=viewport content='width=200'><center><h1>LoRaHam</h1></center>Last Message:<br><br><br>From: " +FROM+"<br><br>Message: "+msg+"<br><br><a href=\"/msg\">Send Message!</a><br><a href=\"/cfg\">Setup</a>";
        Serial.println(temp);
        server.send(200, "text/html", temp);
      }

void webEnterMSG() {
        String form = "<meta name=viewport content='width=200'><style>input, textarea {max-width:100%}</style><p><center><form action='msg2'><p></center>Previous MSG FROM: "+FROM+"<br>TO: <input type='text' name='TO' size=15 autofocus><br>Message: <input type='text' name='MSG1' size=75><center><br><br> <input type='submit' value='Submit'></form></center>";
        server.send(200, "text/html", form);// And as regular external functions:
      }

void webProcessMSG() {
        server.send(200,"text/html","<meta name=viewport content='width=200'> <meta HTTP-EQUIV=\"refresh\" CONTENT=\"20;http://192.168.4.1/\">Sent...");
        call = server.arg("TO");
        call.toUpperCase();
        msg = server.arg("MSG1");
        txMsg();
      }

void webCFG() {
        server.send(200, "text/html", "CFG Screen<br><br>Nothing Here Yet");
      }

void setup() {
  createSSID();
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(OLED_RESET,OUTPUT);
  digitalWrite(OLED_RESET, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(OLED_RESET, HIGH); // while OLED is running, must set GPIO16 in high
  Serial.setTimeout(10);
  Serial.begin(9600);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  displayIntro();
  SPI.begin(5,19,27,18);
  inputString.reserve(200);  
  
  WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", webRoot);
    server.on("/cfg", webCFG);
    server.on("/msg", webEnterMSG);
    server.on("/msg2", webProcessMSG);
    server.begin();
    disableCore0WDT();
    disableCore1WDT();
    xTaskCreatePinnedToCore(
                    serverTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    serverCore);  /* Core where the task should run */
   /* xTaskCreatePinnedToCore(
                   beacon, 
                   "beacon",
                   1000,
                   NULL,
                   1,
                   NULL,
                   serverCore);
                   */                
}

void loop() {
  serialEvent();
  rx();
  //added just to test things
   call = "BEACON";
    msg = CQMSG;
    txMsg();
  //above added just to test things  
  vTaskDelay(10);
}

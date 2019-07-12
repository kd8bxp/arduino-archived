/* Basic test of the ESP8266 to send UDP packet - ping and/or dht11 temp details

Be sure to bring CH_PD and RST high via 10k resistor. .. pulled to the 3.3VDC level.

USB power alone is not enough, module needs 200ma. So external power is a must. We are sing the 3.3VDC power supply mounted to the breadboard.

The ESP8266 needs to be connected to the TX/RX lines of the Arduino (hardware uart on serial port 1 (second available port)).
Use a level shifter (three resistors of 2200 ohms each) to convert from the 5v of the arduino to 3.3v of the module.
The PC is then connected to the normal 0/1 serial port.  Putting the esp8266 removes the communication errors during uploading.

connect GND pin of Mega2560 board to the gnd line of the breadboard and 3.3vdc power supply
pin 0 .. leave open for programming and Serial monitoring
pin 1 .. leave open for programming and serial monitoring

pin 2 .. Use level resistors to connecto to esp8266 Pin 4 .. WIFI_CH_PD 
pin 3 .. Use level resistors to connecto to esp8266 Pin 6 .. WIFI_RESET
.. these pins needs to be pulled high for normal operation of the esp8266 .. We use them for a hard reboot.

pin 7 .. Connected to resistor and red LED for "WiFi NOT Ready"
pin 8 .. Connected to resistor and green LED for "WiFi Ready"



pin 18 .. connect directly to esp8266 Pin 2
pin 19 .. use level resistors to connecto to esp8266 Pin 7


re-written to work best for the Mega2560

Ray Scott
Rabbit Robots
Middletown, OH 45042
ray@rabbitrobots.com

*/




#define SSID        "ScottFarAP"  // name of my WiFi netword
#define PASS        "Scott3503"   // password for access to my WiFi metwork
#define DEST_IP     "192.168.1.77" // my laptop IP address .. running UDP Test Tool 3.0 to see the packets arrive
// no port defined?


#define TIMEOUT     5000 // mS
#define CONTINUE    false
#define HALT        true
#define ENDWITHCLRF true
#define ENDWITHNOTHING false

#define ECHO_COMMANDS // Un-comment to echo AT+ commands to serial monitor

#define MyPCSerial Serial  // using hardware serial port0 .. pins 0 and 1  
#define esp8266Serial Serial1   // using hardware serial port1 .. pins 18 and 19

// #include <SoftwareSerial.h>
// SoftwareSerial Serial1(10, 11); // RX, TX


// pins number for WiFi enabled/disabled LEDs
#define WIFI_DISABLED_LED_PIN 7
#define WIFI_ACTIVE_LED_PIN 8
// arduino pin used to connect to CH_PD (Power Down) WiFi module pin
#define WIFI_CH_PD_PIN 2
// arduino pin used to connect to RESET WiFi module pin
#define WIFI_RESET_PIN 3

//#include "DHT.h"

//DHT dht;

// Print error message and loop stop.
void errorHalt(String msg)
{
  MyPCSerial.println(msg);
  MyPCSerial.println("HALT");
  while(true){
      delay(1000);
      MyPCSerial.println("HALTed");
    };  // stay here forever
}

// Read characters from WiFi module and echo to serial until keyword occurs or timeout.
boolean echoFind(String keyword, long timeout = TIMEOUT)
{
  byte current_char   = 0;
  byte keyword_length = keyword.length();
  
  // Fail if the target string has not been sent by deadline.
  long deadline = millis() + TIMEOUT;
  while(millis() < deadline)
  {
    if (esp8266Serial.available())
    {
      char ch = esp8266Serial.read();
      MyPCSerial.write(ch);
      if (ch == keyword[current_char])
        if (++current_char == keyword_length)
        {
          MyPCSerial.println();
          return true;
        }
    }
  }
  return false;  // Timed out
}

// Read and echo all available module output.
// (Used when we're indifferent to "OK" vs. "no change" responses or to get around firmware bugs.)
void echoFlush()
  {while(esp8266Serial.available()) MyPCSerial.write(esp8266Serial.read());}
  
// Echo module output until 3 newlines encountered.
// (Used when we're indifferent to "OK" vs. "no change" responses.)
void echoSkip()
{
  echoFind("\n");        // Search for nl at end of command echo
  echoFind("\n");        // Search for 2nd nl at end of response.
  echoFind("\n");        // Search for 3rd nl at end of blank line.
}

// Send a command to the module and wait for acknowledgement string
// (or flush module output if no ack specified).
// Echoes all data received to the serial monitor.
boolean echoCommand(String cmd, String ack, boolean halt_on_fail, boolean doTerminateCLRF = true, long timeout = TIMEOUT)
{
  if(doTerminateCLRF){
    esp8266Serial.println(cmd);
  }else{
    esp8266Serial.print(cmd);
  }
  #ifdef ECHO_COMMANDS
    MyPCSerial.print("--"); MyPCSerial.println(cmd);
  #endif
  
  // If no ack response specified, skip all available module output.
  if (ack == "")
    echoSkip();
  else
    // Otherwise wait for ack.
    if (!echoFind(ack))          // timed out waiting for ack string 
      if (halt_on_fail)
        errorHalt(cmd+" failed");// Critical failure halt.
      else
        return false;            // Let the caller handle it.
  return true;                   // ack blank or ack found
}



// Connect to the specified wireless network.
boolean connectWiFi()
{
  String cmd = "AT+CWJAP=\""; cmd += SSID; cmd += "\",\""; cmd += PASS; cmd += "\"";
  if (echoCommand(cmd, "OK", CONTINUE, ENDWITHCLRF, TIMEOUT*2)) // Join Access Point, takes longer than the other commands so lets give it al onger timeout
  {
    MyPCSerial.println("Connected to WiFi.");
    digitalWrite( WIFI_DISABLED_LED_PIN, LOW);
    digitalWrite( WIFI_ACTIVE_LED_PIN, HIGH);
    return true;
  }
  else
  {
    MyPCSerial.println("Connection to WiFi failed.");
    digitalWrite( WIFI_ACTIVE_LED_PIN, LOW);
    digitalWrite( WIFI_DISABLED_LED_PIN, HIGH);
    return false;
  }
}


int i=0;

void sendPing(){
  // Build submission.
  String cmd = "Rabbit Robots.. esp8266 PING " + String(i);
  
  i++;
  if(i>254) i=0;
  
  // Ready the module to receive raw data
  if (!echoCommand("AT+CIPSEND=0,"+String(cmd.length()), ">", CONTINUE))
  {
    //echoCommand("AT+CIPCLOSE", "", CONTINUE);
    MyPCSerial.println("Connection timeout.");
    return;
  }
  
  // Send the raw request
  echoCommand(cmd, "SEND OK", CONTINUE, ENDWITHNOTHING);  // GET
}


void sendUDPMSG(){
  char buffer[5];
  char buffer2[5];
  
  String cmd = "*Rabbit Robots*";
    
  // Ready the module to receive raw data
  if (!echoCommand("AT+CIPSEND=0,"+String(cmd.length()), ">", CONTINUE))
  {
    //echoCommand("AT+CIPCLOSE", "", CONTINUE);
    MyPCSerial.println("Connection timeout.");
    return;
  }
  
  // Send the raw request
  echoCommand(cmd, "SEND OK", CONTINUE, false);  // GET 
}



// ******** SETUP ********
void setup()  {



    // set pins used for WiFi status LEDs as OUTPUT
  pinMode( WIFI_ACTIVE_LED_PIN, OUTPUT);
  pinMode( WIFI_DISABLED_LED_PIN, OUTPUT);
      digitalWrite( WIFI_ACTIVE_LED_PIN, LOW);
      digitalWrite( WIFI_DISABLED_LED_PIN, HIGH);

    
  // Arduino pin connected to ESP8266 CH_PD pin is set to OUTPUT.
  // To keep the module active, this pin must be kept HIGH.
  pinMode( WIFI_CH_PD_PIN, OUTPUT);
  digitalWrite( WIFI_CH_PD_PIN, HIGH);
  // Arduino pin connected to ESP8266 RESET pin is set to OUTPUT.
  // To avoid random resents, we keep this HIGH.
  pinMode( WIFI_RESET_PIN, OUTPUT);
  digitalWrite( WIFI_RESET_PIN, HIGH);
  // perform a hardware reset (ESP8266 RESET pin goes LOW)
  digitalWrite( WIFI_RESET_PIN, LOW);
  delay( 500);
  // allow ESP8266 module to boot
  digitalWrite( WIFI_RESET_PIN, HIGH);
  
  MyPCSerial.begin(115200);         // Communication with PC monitor via USB
  esp8266Serial.begin(115200);        // Communication with ESP8266 via 5V/3.3V level shifter
  
  esp8266Serial.setTimeout(TIMEOUT);
  MyPCSerial.println("ESP8266 Demo");
  
  // dht.setup(9); // data pin 9
  
  delay(4000);

  echoCommand("AT+RST", "ready", HALT);    // Reset & test if the module is ready  
  MyPCSerial.println("Module is ready.");
      digitalWrite( WIFI_DISABLED_LED_PIN, LOW);
      digitalWrite( WIFI_ACTIVE_LED_PIN, HIGH);
  echoCommand("AT+GMR", "OK", CONTINUE);   // Retrieves the firmware ID (version number) of the module. 
  echoCommand("AT+CWMODE?","OK", CONTINUE);// Get module access mode. 
  
  // echoCommand("AT+CWLAP", "OK", CONTINUE); // List available access points - DOESN't WORK FOR ME
  
  echoCommand("AT+CWMODE=1", "", HALT);    // client mode
  echoCommand("AT+CIPMUX=1", "", HALT);    // Allow multiple connections (we'll only use the first).

  //connect to the wifi
  boolean connection_established = false;
  for(int i=0;i<5;i++)
  {
    if(connectWiFi())
    {
      connection_established = true;
      break;
    }
  }
  if (!connection_established) errorHalt("Connection failed");
  
  delay(5000);

  //echoCommand("AT+CWSAP=?", "OK", CONTINUE); // Test connection
  echoCommand("AT+CIFSR", "", HALT);         // Echo IP address. 
  //echoCommand("AT+CIPMUX=0", "", HALT);      // Set single connection mode
  
  
  // Establish UDP connection
  String cmd = "AT+CIPSTART=0,\"UDP\",\""; cmd += DEST_IP; cmd += "\",8080";
  if (!echoCommand(cmd, "OK", CONTINUE)) return;
  //delay(2000);
  
  // Get connection status 
  if (!echoCommand("AT+CIPSTATUS", "OK", CONTINUE)) return;
}




// ******** LOOP ********
void loop() 
{
  
  sendPing();
  // sendUDPMSG();
  
  // Loop forever echoing data received from destination server.
  while (esp8266Serial.available()){   MyPCSerial.write(esp8266Serial.read());  }
  
  delay(10000);   
}



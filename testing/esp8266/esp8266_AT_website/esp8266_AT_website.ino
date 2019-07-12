// refer to Arduino example from website
// http://allaboutee.com/2015/01/20/esp8266-android-application-for-arduino-pin-control/

// define the URL e.g. http://192.168.1.60:80/?pin=13 (to toggle pin 13 for example)
// this code successfully creates a http connection
// reads the request from client with specified pin number
// toggles the state of the declared pin number
// responds with a small web page that indicates the new pin state


// #include <SoftwareSerial.h>

#define DEBUG true  // change the "debug" value to "false" after the program is confirmed to work perfectly.


#define MyPC Serial  // using hardware serial port0 .. pins 0 and 1  
#define esp8266 Serial1   // using hardware serial port1 .. pins 18 and 19

// SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
// This means that you need to connect the TX line from the esp to the Arduino's pin 2
// and the RX line from the esp to the Arduino's pin 3

// pins number for WiFi enabled/disabled LEDs
#define WIFI_DISABLED_LED_PIN 7
#define WIFI_ACTIVE_LED_PIN 8
// arduino pin used to connect to CH_PD (Power Down) WiFi module pin
#define WIFI_CH_PD_PIN 2
// arduino pin used to connect to RESET WiFi module pin
#define WIFI_RESET_PIN 3

void setup()
{

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
  
  Serial.begin(115200);  // baud rate for communicating with the PC
  esp8266.begin(115200); // your esp's baud rate might be different

  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
delay (4000);

  sendCommand("AT+RST\r\n", 2000, DEBUG); // reset module
  delay(1000);
  sendCommand("AT+CWMODE=1\r\n", 1000, DEBUG); // configure as access point
  delay(1000);
  sendCommand("AT+CWJAP=\"ScottFarAP\",\"Scott3503\"\r\n", 3000, DEBUG);
  delay(10000);
  sendCommand("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
  delay(1000);
  sendCommand("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
  delay(1000);
  sendCommand("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80

  Serial.println("Server Ready");
}

void loop()
{
  if (esp8266.available()) // check if the esp is sending a message
  {


    if (esp8266.find("+IPD,"))
    {
      delay(1000); // wait for the serial buffer to fill up (read all the serial data)
      // get the connection id so that we can then disconnect
      int connectionId = esp8266.read() - 48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48

      esp8266.find("pin="); // advance cursor to "pin="

      int pinNumber = (esp8266.read() - 48); // get first number i.e. if the pin 13 then the 1st number is 1
      int secondNumber = (esp8266.read() - 48); // if the pin number is 13, then second digit is 3
      if (secondNumber >= 0 && secondNumber <= 9)
      {
        pinNumber *= 10;
        pinNumber += secondNumber; // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
      }

      digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin

      // build string that is send back to device that is requesting pin toggle
      String content;
      content = "Rabbit Robots Test <P> ";

      content += "Pin ";
      content += pinNumber;
      content += " is ";

      if (digitalRead(pinNumber))
      {
        content += "ON";
      }
      else
      {
        content += "OFF";
      }

      sendHTTPResponse(connectionId, content);



      // check if the connection is still open.  If connection is open, then send a close command

      // make close command
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";

      sendCommand(closeCommand, 1000, DEBUG); // close connection
    }
  }
}

/*
  Name: sendData
  Description: Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  int dataSize = command.length();
  char data[dataSize];
  command.toCharArray(data, dataSize);

  esp8266.write(data, dataSize); // send the read character to the esp8266
  if (debug)
  {
    Serial.println("\r\n====== HTTP Response From Arduino ======");
    Serial.write(data, dataSize);
    Serial.println("\r\n========================================");
  }

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }

  return response;
}

/*
  Name: sendHTTPResponse
  Description: Function that sends HTTP 200, HTML UTF-8 response
*/
void sendHTTPResponse(int connectionId, String content)
{

  // build HTTP response
  String httpResponse;
  String httpHeader;
  // HTTP Header
  httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n";
  httpHeader += "Content-Length: ";
  httpHeader += content.length();
  httpHeader += "\r\n";
  httpHeader += "Connection: close\r\n\r\n";
  httpResponse = httpHeader + content + " "; // There is a bug in this code: the last character of "content" is not sent, I cheated by adding this extra space
  sendCIPData(connectionId, httpResponse);
}

/*
  Name: sendCIPDATA
  Description: sends a CIPSEND=<connectionId>,<data> command

*/
// should check to see if the esp8266 is ready?

void sendCIPData(int connectionId, String data)
{
  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += data.length();
  cipSend += "\r\n";
  sendCommand(cipSend, 1000, DEBUG);
  sendData(data, 1000, DEBUG);
}

/*
  Name: sendCommand
  Description: Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/
String sendCommand(String command, const int timeout, boolean debug)
{
  String response = "";

  esp8266.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }

  return response;
}


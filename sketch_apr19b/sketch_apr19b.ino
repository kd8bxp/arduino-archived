/* 
 *  Modified for SPIFFS by LeRoy Miller Apr 19, 2020
 *  based on sketch from https://learn.sparkfun.com/tutorials/wifi-controlled-robot
 *  
 *  convert images to base64 for use on website using this tool:
 *  https://www.base64-image.de/
 *  
 *  
 *  Alex Wende SparkFun Electronics
 *  ESP32 Web Controlled Motor Test
 *  
 *  To use this code, download the ESP32WebServer library from:
 *  https://github.com/Pedroalbuquerque/ESP32WebServer
 *  
 *  In this Example we'll use the arrow keys from our keyboard to send commands to the Serial 
 *  Controlled Motor Driver. When the ESP32 connects to the WiFi network, the ESP32 sends the
 *  IP address over the Serial to the terminal window at 9600 baud. Copy and paste the IP 
 *  address into your brower's window to go to the ESp32's web page. From there, use the arrow keys
 *  to control the motors.
 *  
 *  UP Arrow - Drive Forward
 *  DOWN Arrow - Drive in Reverse
 *  LEFT Arrow - Turn Left
 *  RIGHT Arrow - Turn Righ
 *  
 *  If the motors aren't spinning in the correct direction, you'll need to to change the motor
 *  number and/or motor direction in the handleMotors() function.
 */

#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFi.h>
//#include <SPI.h>
//#include <SD.h>
#include "SPIFFS.h"

const char* ssid = "";
const char* password = "";

WebServer server(80);  //Default port number

void handleRoot()
{  
  /* we load the index.html from microSD */
  File myFile = SPIFFS.open("/index.html");
  
  if (myFile) 
  {  
    /* respond the content of file to client by calling streamFile()*/
    size_t sent = server.streamFile(myFile, "text/html");
    /* close the file */
    myFile.close();
  } 
  else
  {
    Serial.println("error opening index.html");
  }
}

//XML page to listen for motor commands
void handleMotors() 
{ 
  String motorState = "OFF";
  String t_state = server.arg("motorState"); //Refer  xhttp.open("GET", "setMotors?motorState="+motorData, true);

  Serial.print("D\r\n"); //Disable motors
  delay(50);

  if(t_state.startsWith("U"))  //Drive Forward (UP Arrow)
  {
    Serial.print("M0F70\r\n");
    delay(50);
    Serial.print("M1F70\r\n");
    delay(50);
    Serial.print("E\r\n");
  }
  else if(t_state.startsWith("D")) //Reverse (DOWN Arrow)
  {
    Serial.print("M0R70\r\n");
    delay(50);
    Serial.print("M1R70\r\n");
    delay(50);
    Serial.print("E\r\n");
  }
  else if(t_state.startsWith("R")) //Turn Right (Right Arrow)
  {
    Serial.print("M0F50\r\n");
    delay(50);
    Serial.print("M1R50\r\n");
    delay(50);
    Serial.print("E\r\n");
  }
  else if(t_state.startsWith("L")) //Turn Left (LEFT Arrow)
  {
    Serial.print("M0R50\r\n");
    delay(50);
    Serial.print("M1F50\r\n");
    delay(50);
    Serial.print("E\r\n");
  }

   server.send(200, "text/plain", motorState); //Send web page
}

// cannot handle request so return 404
void handleNotFound()
{
  server.send(404, "text/plain", "File Not Found\n\n");
}

void setup(){
  Serial.begin(115200); //SCMD + debug messages
  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }
  WiFi.begin(ssid, password); //WiFi network to connect to

  Serial.println();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /* register the callbacks to process client request */
  /* root request we will read the memory card to get 
  the content of index.html and respond that content to client */
  server.on("/", handleRoot);
  server.on("/setMotors", handleMotors);
  server.onNotFound(handleNotFound);
  server.begin(); //Start the web server

  Serial.println("HTTP server started");
  Serial.print("Initializing SD card...");

  /* initialize microSD */
  /*if (!SD.begin(33)) {
    Serial.println("initialization failed!");
    return;
  }*/
  Serial.println("initialization done.");
}

void loop(){
  server.handleClient();
}


server.arg

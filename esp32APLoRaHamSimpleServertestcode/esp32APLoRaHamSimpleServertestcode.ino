#include <WiFi.h>
#include <ESP32WebServer.h>

const char *ssid= "LoRaHam";
const char *password = "pass1234";

ESP32WebServer server(80);

String TO = "BEACON";
String FROM = "KD8BXP-10";
String MESSAGE1 = "This is a message 1.200 ESP32";
String RT = "";
String MYCALL = "KD8BXP";


static int serverCore = 0; //run web server on this core. Loop() runs in core 1
 
void serverTask( void * pvParameters ){
      while(true){
        server.handleClient();
    }
 
}
 
void setup() {
 
  Serial.begin(9600);
  delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.print(myIP);
server.on("/", []()
      {
        String temp;
        temp = "<meta name=viewport content='width=200'><center><h1>LoRaHam</h1></center>Last Message:<br><br>To: " + TO +"<br>From: " +FROM+"<br>Message: "+MESSAGE1+"<br>RT: "+RT+"<br><br><a href=\"/msg\">Send Message!</a>";
        Serial.println(temp);
        server.send(200, "text/html", temp);
      });
      
    server.on("/cfg", []()
      {
        server.send(200, "text/html", "CFG Screen");
      });

    server.on("/msg", []()
      {
        String form = "<meta name=viewport content='width=200'><style>input, textarea {max-width:100%}</style><p><center><form action='msg2'><p></center>TO: <input type='text' name='TO' size=15 vaule='"+FROM+"' autofocus><br>Message: <input type='text' name='MSG' size=75><center><br><br> <input type='submit' value='Submit'></form></center>";
        server.send(200, "text/html", form);// And as regular external functions:
      });

    server.on("/msg2", []()
      {
        server.send(200,"text/html","Nothing Here Yet!");
      });
    server.begin();
   
  delay(1000);
 
  Serial.print("Starting to create task on core ");
  Serial.println(taskCore);
 
  xTaskCreatePinnedToCore(
                    serverTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    serverCore);  /* Core where the task should run */
 
  Serial.println("Task created...");
}
 
void loop() {
 
  Serial.println("main loop...");
 delay(1000);
  //while (true){}
 
}

/*
 Created by Rui Santos
 Visit: http://randomnerdtutorials.com for more arduino projects

 Arduino with Ethernet Shield
 */

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1};                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
          // client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           //client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Project Turn on Stuff From Website</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Turn on Stuff from a Website</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>Arduino with Ethernet Shield</H2>");
           client.println("<br />");  
           client.println("<a href=\"/?button1on\"\">Turn On LED 1</a>");
           client.println("<a href=\"/?button1off\"\">Turn Off LED 1</a><br />");   
           client.println("<br />");     
           client.println("<br />"); 
            client.println("<a href=\"/?button2on\"\">Turn On LED 2</a>");
           client.println("<a href=\"/?button2off\"\">Turn Off LED 2</a><br />");   
           client.println("<br />");     
           client.println("<br />");
            client.println("<a href=\"/?button3on\"\">Turn On LED 3</a>");
           client.println("<a href=\"/?button3off\"\">Turn Off LED 3</a><br />");   
           client.println("<br />");     
           client.println("<br />");
            client.println("<a href=\"/?button4on\"\">Turn On LED 4</a>");
           client.println("<a href=\"/?button4off\"\">Turn Off LED 4</a><br />");   
           client.println("<br />");     
           client.println("<br />");
            client.println("<a href=\"/?button5on\"\">Turn On LED 5 </a>");
           client.println("<a href=\"/?button5off\"\">Turn Off LED 5</a><br />");   
           client.println("<br />");     
           client.println("<br />");
            client.println("<a href=\"/?button6on\"\">Turn On LED 6</a>");
           client.println("<a href=\"/?button6off\"\">Turn Off LED 6</a><br />");   
           client.println("<br />");     
           client.println("<br />");
         /*   client.println("<a href=\"/?7on\"\">Turn On LED 7</a>");
           client.println("<a href=\"/?7off\"\">Turn Off LED 7</a><br />");   
           client.println("<br />");     
           client.println("<br />");
            client.println("<a href=\"/?8on\"\">Turn On LED 8</a>");
           client.println("<a href=\"/?8off\"\">Turn Off LED 8</a><br />");   
           client.println("<br />");     
           client.println("<br />");
         */
           client.println("<p>Based on a sketch ");
           client.println("<p>Created by Rui Santos. Visit http://randomnerdtutorials.com for more projects!</p>");  
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0){
               digitalWrite(2, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(2, LOW);
           }
           
           if (readString.indexOf("?button2on") >0){
               digitalWrite(3, HIGH);
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(3, LOW);
           }
           
           if (readString.indexOf("?button3on") >0){
               digitalWrite(4, HIGH);
           }
           if (readString.indexOf("?button3off") >0){
               digitalWrite(4, LOW);
           }
           
           
           if (readString.indexOf("?button4on") >0){
               digitalWrite(5, HIGH);
           }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(5, LOW);
           }
           
           if (readString.indexOf("?button5on") >0){
               digitalWrite(6, HIGH);
           }
           if (readString.indexOf("?button5off") >0){
               digitalWrite(6, LOW);
           }
           
           if (readString.indexOf("?button6on") >0){
               digitalWrite(7, HIGH);
           }
           if (readString.indexOf("?button6off") >0){
               digitalWrite(7, LOW);
           }
           
           /*
           if (readString.indexOf("?7on") >0){
               digitalWrite(8, HIGH);
           }
           if (readString.indexOf("?7off") >0){
               digitalWrite(8, LOW);
           }
           
           if (readString.indexOf("?8on") >0){
               digitalWrite(9, HIGH);
           }
           if (readString.indexOf("?8off") >0){
               digitalWrite(9, LOW);
           }
           */
           
                      //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
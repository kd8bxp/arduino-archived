/*****************************************************************************
* | File        :   WIFI_Driver.c
* | Author      :   Waveshare team
* | Function    :   Use of WIFI function
* | Info        :   Used to open AP for users to connect 
*----------------
* | This version:   V1.0
* | Date        :   2023-11-22
* | Info        :

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "WIFI_Driver.h"

void intToIpAddress(uint32_t ip, char *result) {
    sprintf(result, "%d.%d.%d.%d", ip & 255,(ip >> 8) & 255,(ip >> 16) & 255,(ip >> 24) & 255);
}

void WIFI_AP_Init()
{
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }

  IPAddress myIP = WiFi.softAPIP();
  uint32_t ipAddress = WiFi.softAPIP();
  char ipAddressStr[16]; 
  intToIpAddress(ipAddress, ipAddressStr);
  Paint_DrawString_EN(0, 30, "Server started", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(0, 60, "IP:", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(55, 63, ipAddressStr, &Font20, 0x000f, 0xfff0);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
}

void WIFI_STA_Init()
{
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }

  IPAddress myIP = WiFi.softAPIP();
  uint32_t ipAddress = WiFi.softAPIP();
  char ipAddressStr[16]; 
  intToIpAddress(ipAddress, ipAddressStr);
  Paint_DrawString_EN(0, 30, "Server started", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(0, 60, "IP:", &Font24, 0x000f, 0xfff0);
  Paint_DrawString_EN(55, 63, ipAddressStr, &Font20, 0x000f, 0xfff0);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
}

void LCD_Control_Page(WiFiClient client)
{
  client.println("<html><head><meta charset='UTF-8'><style>");
  client.println("body {font-family: 'Arial', sans-serif; text-align: center; background-color: #e6e6e6; margin: 20px; padding: 20px; border: 1px solid #ddd; border-radius: 10px;}");
  client.println("h2 {color: #333;}");
  client.println("a {text-decoration: none; color: #333; padding: 15px 20px; margin: 10px; border-radius: 5px; display: inline-block; border: 2px solid #ddd;}");
  client.println("a#on {background-color: #fff; color: #000;}"); // Updated to white background with black text
  client.println("a#off {background-color: #000; color: #fff;}");
  client.println("a#red {background-color: #e74c3c;}");
  client.println("a#green {background-color: #2ecc71;}");
  client.println("a#blue {background-color: #3498db;}");
  client.println("a#picture {background-color: #9b59b6;}");      // Updated to purple background with white text
  client.println("a:hover {background-color: #333;}");
  client.println(".button-container {margin-top: 30px;}");
  client.println("</style></head><body>");

  client.println("<div style='border: 1px solid #ddd; padding: 10px; border-radius: 5px; background-color: #fff;'>");
  client.println("<h2>Welcome to ESP32S3 LCD Control</h2>");
  client.println("<p><a id='on' href='/H'><span></span>White</a> to turn ON the LCD.</p>");
  client.println("<p><a id='off' href='/L'><span></span>Black</a> to turn OFF the LCD.</p>");
  client.println("<p><a id='red' href='/R'><span></span>Red</a> to turn the LCD Red.</p>");
  client.println("<p><a id='green' href='/G'><span></span>Green</a> to turn the LCD Green.</p>");
  client.println("<p><a id='blue' href='/B'><span></span>Blue</a> to turn the LCD Blue.</p>");
  client.println("<p><a id='picture' href='/P'><span></span>Picture</a> display on the LCD.</p>");

  // JavaScript for handling link clicks
  client.println("<script>");
  client.println("document.querySelectorAll('a').forEach(link => {");
  client.println("  link.addEventListener('click', (event) => {");
  client.println("    event.preventDefault();");
  client.println("    fetch(link.getAttribute('href'));");
  client.println("  });");
  client.println("});");
  client.println("</script>");

  client.println("</div></body></html>");
}

void Web_Server_Page(WiFiClient client)
{
client.println("<html><head><meta charset='UTF-8'><style>");
client.println("body {font-family: 'Arial', sans-serif; text-align: center; background-color: #e6e6e6; margin: 20px; padding: 20px; border: 1px solid #ddd; border-radius: 10px;}");
client.println("h2 {color: #333;}");
client.println("input {padding: 10px; margin: 10px; border: 2px solid #ddd; border-radius: 5px;}");
client.println("button {padding: 10px 20px; margin: 10px; border: none; border-radius: 5px; background-color: #3498db; color: #fff; cursor: pointer;}");
client.println("button:hover {background-color: #2980b9;}");
client.println("</style></head><body>");

client.println("<div style='border: 1px solid #ddd; padding: 10px; border-radius: 5px; background-color: #fff;'>");
client.println("<h2>Web Message Sender</h2>");

// Form for entering a message
client.println("<form id='messageForm'>");
client.println("  <label for='message'>Enter Message:</label>");
client.println("  <input type='text' id='message' name='message' required>");
client.println("  <button type='button' onclick='sendMessage()'>Send Message</button>");
client.println("</form>");

// JavaScript for handling message sending
client.println("<script>");
client.println("function sendMessage() {");
client.println("  var message = document.getElementById('message').value;");
client.println("  fetch('/sendMessage?message=' + encodeURIComponent(message));");
client.println("}");
client.println("</script>");

client.println("</div></body></html>");

}

void WIFI_LCD_Control(WiFiClient client)
{
  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            LCD_Control_Page(client);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          LCD_Clear(WHITE);
        }
        if (currentLine.endsWith("GET /L")) {
          LCD_Clear(BLACK);
        }
        if (currentLine.endsWith("GET /R")) {
          LCD_Clear(RED);
        }
        if (currentLine.endsWith("GET /G")) {
          LCD_Clear(GREEN);
        }
        if (currentLine.endsWith("GET /B")) {
          LCD_Clear(BLUE);
        }
        if (currentLine.endsWith("GET /P")) {
          Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
          Paint_DrawImage(gImage_pic1, 0, 0, 135, 240);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  } 
}

void WIFI_Web_Server(WiFiClient client)
{
  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            Web_Server_Page(client);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Read the first line of the request
        String request = client.readStringUntil('\r');

        int messageStart1 = request.indexOf("message=");
        if (messageStart1 != -1) {
          String message = request.substring(messageStart1 + 8, request.indexOf(" ", messageStart1));
          Serial.println("Received message: " + message);
          const char* messageCStr = message.c_str();
          LCD_Clear(0x000f);
          Paint_DrawString_EN(0, 30, "Received messa", &Font24, 0x000f, 0xfff0);
          Paint_DrawString_EN(0, 60, "ge:", &Font24, 0x000f, 0xfff0);
          Paint_DrawString_EN(55, 63, messageCStr, &Font20, 0x000f, 0xff00);
        }
            }
          }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  } 
}






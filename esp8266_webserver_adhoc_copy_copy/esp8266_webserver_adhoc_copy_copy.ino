#define DEBUG true
 

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600); // your esp's baud rate might be different
  
 
  
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CWSAP=\"BittyBotxxxx\",\"\",5,0\r\n",1000,DEBUG); //Give your Bitty Bot a name all it's own
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
 
void loop()
{
	//startwww();
	listenwww();
}

void listenwww() {
	  if(Serial1.available()) // check if the esp is sending a message 
  {
 
    
    if(Serial1.find("+IPD,"))
    {
     delay(1000); // wait for the serial buffer to fill up (read all the serial data)
     // get the connection id so that we can then disconnect
     int connectionId = Serial1.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
          
     Serial1.find("drive="); // advance cursor to "pin="
     
     int pinNumber = (Serial1.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     pinNumber += (Serial1.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
     
     Serial.print("Found: ");
     Serial.println(pinNumber);
     
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,1000,DEBUG); // close connection
    }
  }
}

void startwww() {	
  if(Serial1.available()) // check if the esp is sending a message 
  {
    /*
    while(Serial1.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial1.read(); // read the next character.
      Serial.write(c);
    } */
    
    if(Serial1.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = Serial1.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String webpage = "<h1>Bitty Bot</h2><br>Ready<br><br><form method=\"get\"><input type=\"submit\" value=\"11\" name=\"drive\">FORWARD<br><br>";
 
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     
     webpage="<input type=\"submit\" value=\"12\" name=\"drive\">LEFT<br><br><input type=\"submit\" value=\"13\" name=\"drive\">RIGHT<br><br>";
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
  webpage="<input type=\"submit\" value=\"14\" name=\"drive\">BACKWARD<br><br></form>";
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
    
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);
    }
  }
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    Serial1.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial1.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}